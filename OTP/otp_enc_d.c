#include<stdlib.h>
#include<string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include<sys/wait.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<errno.h>
#include "rdwrn.h"
#include<fcntl.h>

#define MAX_W 1024 

void handle_SIGCHLD(int signo);
void encrypt(char* plain, char* key, char* cipher);

int main(int argc, char* argv[])
{
  if (argc < 2) { 
    fprintf(stderr,"USAGE: %s port\n", argv[0]); exit(1);
  }
  int     fd[2], nbytes;
  pid_t   childpid, wpid;
  char    string[] = "Hello, world!\n";
  char    readbuffer[MAX_W];
  char    keybuffer[MAX_W];
  char    cipherbuffer[MAX_W];
  int     status;

  /* create server */
  int listenFd, connFd, port;
  struct sockaddr_in pAddr, cAddr;
  socklen_t cinfo;
  int retval;
  memset((char*)&pAddr, '\0', sizeof(pAddr));
  port = atoi(argv[1]);
  pAddr.sin_family = AF_INET;
  pAddr.sin_port = htons(port);
  pAddr.sin_addr.s_addr = INADDR_ANY;
  if ( (listenFd = socket(AF_INET, 
                          SOCK_STREAM,
                          0)) < 0 ) {
    perror("SOCKET ERROR");
  }
  if ( bind(listenFd, (struct sockaddr*)&pAddr, sizeof(pAddr)) < 0)
  {
    perror("BIND ERROR, port unavailable");
  }
  listen(listenFd, 5);

  /* create child signal handling */
  struct sigaction SIGCHLD_action;
  sigemptyset(&SIGCHLD_action.sa_mask);
  //SIGCHLD_action.sa_handler = handle_SIGCHLD;
  SIGCHLD_action.sa_handler = SIG_IGN;
  SIGCHLD_action.sa_flags = 0;
  if (sigaction(SIGCHLD, &SIGCHLD_action, NULL) == -1) {
    perror("sigchld");
    exit(EXIT_FAILURE);
  }

  while(1) {
    cinfo = sizeof(cAddr);
    connFd = accept(listenFd, 
                          (struct sockaddr*)&cAddr, 
                          &cinfo);
    if (connFd < 0){
      perror("ACCEPT ERROR");
    } else {
          memset(readbuffer, '\0', sizeof(readbuffer));
          /* fork created a child process of the program
           * and return the childpid to the process */
          if((childpid = fork()) == -1) { perror("fork"); exit(1); }

          /* child process receives zero */
          if(childpid == 0) { close(listenFd);
            /* client does not match */
            if ((retval=recv(connFd, readbuffer,
                             sizeof(readbuffer)-1, 0)) < 0 ||
                strcmp(readbuffer, "ID-otp_enc") != 0) {
              errno = ECONNREFUSED;
              perror("ID ERROR");
              fflush(stderr);
              exit(2);

            /* client match */
            } else { 
              /* start confirmation */
              if ((retval = send(connFd, "START", 5, 0)) < 0) {
                perror("SEND ERROR");
              }
              //fcntl(connFd, F_SETFL, O_NONBLOCK); 

              /* send and receive message*/
              size_t nbuf = WLEN;
              memset(readbuffer, '\0', MAX_W*sizeof(char));
              memset(keybuffer, '\0', MAX_W*sizeof(char));
              memset(cipherbuffer, '\0', MAX_W*sizeof(char));

              retval = recv(connFd, readbuffer, nbuf, MSG_PEEK);
              nbuf = strlen(readbuffer);
              /* @ not found */
              while (strstr(readbuffer, "@") == NULL && retval != 0) 
              {
                readn(connFd, readbuffer, nbuf);
                nbuf = strlen(readbuffer);
                //printf("%d %s\n", nbuf, readbuffer);
                readn(connFd, keybuffer, nbuf);
                //printf("%d %s\n", nbuf, keybuffer);
                encrypt(readbuffer, keybuffer, cipherbuffer);
                writen(connFd, cipherbuffer, nbuf);
                //printf("%d %s\n", nbuf, cipherbuffer);

                memset(readbuffer, '\0', MAX_W*sizeof(char));
                memset(keybuffer, '\0', MAX_W*sizeof(char));
                memset(cipherbuffer, '\0', MAX_W*sizeof(char));
                retval = recv(connFd, readbuffer, nbuf, MSG_PEEK);
                //printf("\n");
              }

              /* @ found */
              nbuf = strcspn(readbuffer, "@")+1;
              readn(connFd, keybuffer, nbuf);
              //printf("%d %s\n", nbuf, readbuffer);
              readbuffer[strcspn(readbuffer, "@")] = '\0';

              readn(connFd, keybuffer, nbuf);
              //readbuffer[strcspn(keybuffer, "@")] = '\0';
              //printf("%d %s\n", nbuf, keybuffer);
              
              encrypt(readbuffer, keybuffer, cipherbuffer);
              //strcat(cipherbuffer, "@");
              //printf("%d %s\n", nbuf-1, cipherbuffer);
              writen(connFd, cipherbuffer, nbuf-1);

              close(connFd);
              exit(0);
            }

          /* parent will execute this */ 
          } else {   
            close(connFd);
            //close(listenFd);
          }
    }
  }
  close(listenFd);
  return(0);
}

void encrypt(char* plain, char* key, char* cipher)
{
  size_t len;
  len = strlen(plain);
  int i, a, b, c;
  for( i = 0; i < len; i++){
    a = plain[i]; b = key[i];
    if ( a == ' ' ) { a = 26; } else { a -= 'A'; }
    if ( b == ' ' ) { b = 26; } else { b -= 'A'; }
    c = (a+b) % 27;
    if ( c == 26 ) { c = ' '; } else { c += 'A'; }
    cipher[i] = c;
  }
  //memcpy(cipher, plain, len);
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
    //status(&childExit);
    fflush(stdout);
    free(msg);
  }
}
