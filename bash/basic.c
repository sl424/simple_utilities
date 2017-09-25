#include "smallsh.h"

void cleanup(void)
{
  printf("waiting child process: \n");
  fflush(stdout);
  int childExit;
  pid_t wpid;
  while ((wpid = waitpid((pid_t)(-1),&childExit, 0)) > 0){
    char *msg = malloc(MAX_W*sizeof(char));
    memset(msg, '\0', MAX_W*sizeof(char));
    snprintf(msg, MAX_W*sizeof(char), "%s %d %s ", \
        "background pid", wpid, "is done:");
    size_t nw = strlen(msg);
    write(STDOUT_FILENO, msg, nw);
    status(&childExit);
    fflush(stdout);
    free(msg);
  }
  //printf("done.\n");
}

char** openIO(char** args, int *count, int *fileopen, 
                                       int *fin, int *fout, int *bg)
{
  int tmpc = *count -1;
  int i = tmpc-1;
  //while ( args[tmpc+1] == '<' || args[tmpc+1] == '>' ){
  while ( i > 0){
      if (args[i][0]=='<') {
        //printf("open file for input\n");
        //open file as stdin
        int pfdin;
        if ((pfdin = open(args[i+1], O_RDONLY)) == -1){
          *fileopen = 1;
          fprintf(stdout,"%s %s %s", "cannot open", args[i+1], "for input\n");
          fflush(stdout);
        } else{
          *fin = pfdin;
          dup2(pfdin, STDIN_FILENO);
          close(pfdin);
        }
        tmpc = i;
      } else if (args[i][0] == '>') {
        //printf("open file for output\n");
        int pfdout;
        if ((pfdout = open(args[i+1], O_WRONLY | O_CREAT|O_TRUNC, 0644)) == -1){
          *fileopen = 1;
          fprintf(stdout,"%s %s %s", "cannot open", args[i+1], "for output\n");
          fflush(stdout);
        } else {
          *fout = pfdout;
          dup2(pfdout, STDOUT_FILENO);
          close(pfdout);
        }
        tmpc = i;
      }
    i--;
  }
  if (*bg){
        int pfdin;
        if ((pfdin = open("/dev/null", O_RDONLY)) == -1){
          *fileopen = 1;
          fprintf(stdout,"%s %s %s", "cannot open", args[i+1], "for input\n");
          fflush(stdout);
        } else{
          *fin = pfdin;
          dup2(pfdin, STDIN_FILENO);
          close(pfdin);
        }
        int pfdout;
        if ((pfdout = open("/dev/null", O_WRONLY, 0644)) == -1){
          *fileopen = 1;
          fprintf(stdout,"%s %s %s", "cannot open", args[i+1], "for output\n");
          fflush(stdout);
        } else {
          *fout = pfdout;
          dup2(pfdout, STDOUT_FILENO);
          close(pfdout);
        }
  }
  //args = realloc(args, tmpc*sizeof(char*));
  *count = tmpc+1;
  char **args2 = malloc(*count*sizeof(char*));
  memcpy(args2, args, *count*sizeof(char*));
  args2[*count -1] = NULL;
  //args = realloc(args,tmpc*sizeof(char*));
  //args[tmpc-1] = NULL;
  return args2;
}

void catchSIGCHLD(int signo, siginfo_t *sinfo, void *unused)
{
    int sav_errno = errno;
    int status;
    /*
     * Obtain status information for the child which
     * caused the SIGCHLD signal and write its exit code
     * to stdout.
    */
    if (sinfo->si_code == CLD_DUMPED) {
      ;
    }
    else if (waitpid(sinfo->si_pid, &status, 0) == -1) {
      if (WIFSIGNALED(status)) {
          char *msg = malloc(MAX_W*sizeof(char));
          snprintf(msg, MAX_W*sizeof(char), "%s %d\n",
              "terminated by signal", WTERMSIG(status));
          write(2, msg, MAX_W*sizeof(char));
          fflush(stdout);
          free(msg);
      }
    }
    else if (!WIFEXITED(status)) {
        static char msg[] = "WIFEXITED was false\n";
        write(2, msg, sizeof(msg)-1);
    }
    else {
        char *msg = malloc(MAX_W*sizeof(char));
        memset(msg, '\0', MAX_W*sizeof(char));
        snprintf(msg, MAX_W*sizeof(char), "\n%s %d %s %d\n", \
            "background pid", sinfo->si_pid, 
            "is done: exit value", WEXITSTATUS(status));
        write(STDOUT_FILENO, msg, MAX_W*sizeof(char));
        fflush(stdout);
        free(msg);
    }
    /*
    {
        int code = WEXITSTATUS(status);
        char buf[2];
        buf[0] = '0' + code;
        buf[1] = '\n';
        write(1, buf, 2);
    }
    */
    errno = sav_errno;
}

void handle_SIGCHLD(int signo)
{
  int childExit;
  pid_t wpid;
  if ((wpid = waitpid((pid_t)(-1),&childExit, WNOHANG)) > 0){
    char *msg = malloc(MAX_W*sizeof(char));
    memset(msg, '\0', MAX_W*sizeof(char));
    snprintf(msg, MAX_W*sizeof(char), "%s %d %s ", \
        "background pid", wpid, "is done:");
    size_t nw = strlen(msg);
    write(STDOUT_FILENO, msg, nw);
    status(&childExit);
    fflush(stdout);
    free(msg);
  }
}

void catchSIGINT(int signo)
{
  //status(&childExit);
  //char* message = "Caught SIGINT, sleeping for 5 seconds\n";
  //write(STDOUT_FILENO, message, 38);
  //raise(SIGUSR2);
  //sleep(5);
}

char** read_command(int* argcount, int* bg) 
{
  *bg = 0;
  fflush(stdin);
  //char **param = NULL;
  char **param = malloc(MAX_ARG*sizeof(char*));
  int bufsize = MAX_W*sizeof(char);
  printf(":");

  char* str = malloc(bufsize);
  memset(str, '\0', bufsize);
  fgets(str, bufsize, stdin);
  str[strcspn(str, "\n")] = '\0'; 

  char *temp = strtok(str, " ,");
  int num = 0;
  while (temp != NULL && num < MAX_ARG+1) {
      num++;
      //param = realloc(param, num * sizeof(char*));
      char* buffer = malloc(bufsize); 
      //param[num - 1] = malloc(bufsize); 
      //snprintf(param[num - 1], bufsize, "%s", temp);
      snprintf(buffer, bufsize, "%s", temp);
      param[num-1] = buffer;
      if ( num > 0 && strcmp(param[num-1], "&") == 0 ){
        free(param[num-1]);
        num--;
        *bg = 1;
        //param = realloc(param, num * sizeof(char*));
      }
      //free(temp);
      temp = strtok(NULL, " ,");
  }
  fflush(stdin);
  free(str);
  /* NULL argv */
  num++;
  //param = realloc(param, num*sizeof(char*));
  param[num - 1] = NULL;

  //cmd = param[0];
  //execvp(param[0],param);
  *argcount = num;
  return param;
}

void status(int *status)
{
  if (WIFEXITED(*status)) {
      printf("exit value %d\n", WEXITSTATUS(*status));
  } else if (WIFSIGNALED(*status)) {
      printf("terminated by signal %d\n", WTERMSIG(*status));
  } else if (WIFSTOPPED(*status)) {
      printf("stopped by signal %d\n", WSTOPSIG(*status));
  } else {    /* Non-standard case -- may never happen */
      printf("Unexpected status (0x%x)\n", *status);
  }
}

void changewd(char** args)
{
  //setenv("MYVAR", "Some text string 1234", 1);
  //printf("%s\n", getenv("HOME"));
  int ret;
  if (args[1] == NULL){
    ret=chdir(getenv("HOME"));
  }
  else{
    ret=chdir(args[1]);
  }
  if (ret == -1){
    //printf("error changing the working directory\n");
    perror("cd");
  }
}
