#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<sys/wait.h>
#include<signal.h>
#include<errno.h>
#define MAX_W 2048
#define TRUE 1
#define FALSE 0
#define MAX_ARG 512
//#define bufsize MAX_W*sizeof(char)

char** read_command(int *argcount, int *bg);
void status(int * status);
void changewd(char **args);
void catchSIGCHLD(int signo, siginfo_t *sinfo, void *unused);
void handle_SIGCHLD(int signo);
void catchSIGINT(int signo);
char** openIO(char** args, int *count, int *fileopen, 
    int *fin, int *fout, int *bg);
void cleanup(void);
