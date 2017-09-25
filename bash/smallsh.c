#include "smallsh.h"

int main(int argc, char *argv[])
{
  struct sigaction ignore_action;
  //struct sigaction ignore_action;
  struct sigaction SIGCHLD_action;
  char** args = NULL;
  char** args2 = NULL;
  int argcount = 0;
  int argcount2 = 0;
  int childExit = 0;
  pid_t childpid;
  int bg;
  int exitFlag=1;
  int s_stdout = dup(STDOUT_FILENO);
  int s_stdin = dup(STDIN_FILENO);
  int fin = -1;
  int fout = -1;
  int ci;
  int fileopen = 0;

  do {
    /* clear fileio and command args*/
    bg = 0;
    dup2(s_stdout, STDOUT_FILENO);
    dup2(s_stdin, STDIN_FILENO);
    if ( fin != -1 ) { close(fin); fin = -1;}
    if ( fout != -1 ) { close(fout); fout = -1;}
    if (args != NULL){
      for (ci = 0; ci < argcount-1; ci++){
        free(args[ci]);
      }
      free(args);
      args = NULL;
      argcount = 0;
    }
    fflush(stdout);
    fflush(stderr);

    //catch sigchld
    sigemptyset(&SIGCHLD_action.sa_mask);
    //SIGCHLD_action.sa_sigaction = catchSIGCHLD;
    //SIGCHLD_action.sa_flags = SA_SIGINFO;
    SIGCHLD_action.sa_handler = handle_SIGCHLD;
    SIGCHLD_action.sa_flags = 0;
    if (sigaction(SIGCHLD, &SIGCHLD_action, NULL) == -1){
      perror("sigchld");
      exit(EXIT_FAILURE);
    }

             /* ignore sigint */
             sigemptyset(&ignore_action.sa_mask);
             ignore_action.sa_flags = 0;
             ignore_action.sa_handler = SIG_IGN;
             if (sigaction(SIGINT, &ignore_action, NULL) == -1){
               perror("sigint");
               exit(EXIT_FAILURE);
             }

    args = read_command(&argcount, &bg);
    /*ignore empty input */
    if ( argcount == 1 ) {;}
    else if (argcount > 1){
      /*ignore comments */
      if (args[0][0] == '#') {;}
      /* built-in commands */
      else if (strcmp(args[0],"exit")==0) {
        /* kill all child processes */
        cleanup();
        exitFlag = 0;
      }
      else if (strcmp(args[0], "status")==0){
        status(&childExit);
      }
      else if (strcmp(args[0],"cd")==0){
        changewd(args);
      }
      /* begin fork() and execvp() */
      else { 

           if ((childpid=fork()) == -1){
             perror("fork");
             exit(EXIT_FAILURE);
           }
           /* execute child process */
           else if (childpid == 0){ 
             /* ignore sigint */
             /*
             sigemptyset(&ignore_action.sa_mask);
             ignore_action.sa_flags = 0;
             if (bg) {
               ignore_action.sa_handler = SIG_IGN;
             } else {
               ignore_action.sa_handler = SIG_DFL;
             }
             */
             if (!bg) {
               ignore_action.sa_handler = SIG_DFL;
             }
             if (sigaction(SIGINT, &ignore_action, NULL) == -1){
               perror("sigint");
               exit(EXIT_FAILURE);
             }
             argcount2 = argcount;
             args2 = openIO(args, &argcount2, &fileopen, &fin, &fout, &bg);
             
             if (fileopen == 0) {           /* if no redirection error */
               execvp(args2[0], args2);
               perror(args2[0]);
               exit(EXIT_FAILURE);
             } else {
               perror("fileio");
               exit(EXIT_FAILURE);
             }
             /* end of child process */
           }else{ /* parent process */

             if (bg) {
               printf("%s %d\n", "background pid is:", childpid);
             } else {

               /*block background signal*/
							 sigset_t old_set,new_set;
							 sigemptyset(&old_set);
							 sigemptyset(&new_set);
							 sigaddset(&new_set,SIGCHLD);
							 //sigprocmask(SIG_BLOCK,&new_set,&old_set); 
               if(sigprocmask(SIG_BLOCK,&new_set,&old_set)==-1) {
                 perror("sigprocmask1");
               }
              
               /* run as foreground process */
               waitpid(childpid, &childExit, 0);
               if (WIFSIGNALED(childExit)) {
                  char *msg = malloc(MAX_W*sizeof(char));
                  snprintf(msg, MAX_W*sizeof(char), "%s %d\n",
                      "terminated by signal", WTERMSIG(childExit));
                   write(2, msg, MAX_W*sizeof(char));
                   fflush(stdout);
                   free(msg);
               }
               /*remove signal block */
							if(sigprocmask(SIG_UNBLOCK,&new_set,&old_set)==-1) {
							  perror("sigprocmask2");
							 }
             }
             /* end of parent process */
           } 
      }
      /* run other commands if needed */
    }
  } while (exitFlag);
  exit(EXIT_SUCCESS);
} 
