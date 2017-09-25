#include "smallsh.h"


static void handle(int sig)
{
  printf("it works\n");
}

int main(int argc, char *argv[])
{
  struct sigaction sighandle; 
  sigset_t old_set, new_set;
  sigemptyset(&old_set);
  sigemptyset(&new_set);
  sigaddset(&new_set,SIGCONT);

  /* signal handlerr for SIGCONT */
    sigemptyset(&sighandle.sa_mask);
    sighandle.sa_handler = handle;
    sighandle.sa_flags = 0;
    if (sigaction(SIGCONT, &sighandle, NULL) == -1){
      perror("sigchld");
      exit(EXIT_FAILURE);
    }

    /* block sigcont */
    if(sigprocmask(SIG_BLOCK, &new_set, &old_set)==-1){
      perror("signmask");
    }

    sleep(5);


               /*remove signal block */
							if(sigprocmask(SIG_UNBLOCK,&new_set,&old_set)==-1) {
							  perror("sigprocmask2");
							 }

              printf("peace out \n");

  exit(EXIT_SUCCESS);
} 
