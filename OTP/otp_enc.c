#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "rdwrn.h"
#include <fcntl.h>

#define MAX_W 1024

void error(const char *msg) { perror(msg); exit(0); }
ssize_t checkchar(FILE *fd, int* badch);

int main(int argc, char *argv[])
{
	int socketFD, portNumber, charsWritten, charsRead;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	char buffer[MAX_W];
  char* hname = "localhost";
    
  /* check usage & args */
	if (argc < 4) { 
    fprintf(stderr,"USAGE: %s plaintext key port\n", argv[0]); 
    exit(0); 
  } 

  /* open plaintext */
  FILE *fdp, *fdk;
  if ((fdp = fopen(argv[1], "r")) == NULL) {
    error("CLIENT: error opening plaintext");
  }
  if ((fdk = fopen(argv[2], "r")) == NULL) {
    error("CLIENT: error opening key file");
  }

  /*char validation */
  int badch = 0;
  ssize_t plen = checkchar(fdp, &badch);
  if (badch) { fprintf(stderr, "Error, contains bad characters\n"); exit(1);}
  ssize_t klen = checkchar(fdk, &badch);
  if (badch) { fprintf(stderr, "Error, contains bad characters\n"); exit(1);}

  if (plen > klen){
    fprintf(stderr, "Error, key %s is too short\n", argv[2]); exit(1);
  }

	// Set up the server address struct
	memset((char*)&serverAddress, '\0', sizeof(serverAddress));
	portNumber = atoi(argv[3]); 
	serverAddress.sin_family = AF_INET; 
	serverAddress.sin_port = htons(portNumber);
	serverHostInfo = gethostbyname(hname);

	if (serverHostInfo == NULL) { 
    fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); 
  }
	memcpy((char*)serverHostInfo->h_addr, 
         (char*)&serverAddress.sin_addr.s_addr, 
         serverHostInfo->h_length); // Copy in the address

	// Set up the socket
	socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (socketFD < 0) error("CLIENT: ERROR opening socket");
	
	// Connect to server
	if (connect(socketFD, 
              (struct sockaddr*)&serverAddress, 
              sizeof(serverAddress)) < 0) {
		error("CLIENT: ERROR connecting");
    exit(2); /* invalid port */
  }

  /* send id */
	memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer array
	sprintf(buffer,"ID-otp_enc");
	charsWritten = send(socketFD, buffer, strlen(buffer), 0); 

	memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer array
	charsRead = recv(socketFD, buffer, WLEN, 0); 
  //fprintf(stdout, "CLIENT: %s\n", buffer);
  if ( strcmp(buffer, "START") == 0 ) {

    /* Send/receive message to/from server*/
  //fcntl(socketFD, F_SETFL, O_NONBLOCK); 

    size_t wbreak = WLEN;
    int status;
    size_t nbuf;
    memset(buffer, '\0', sizeof(buffer)); 
    nbuf = fread(buffer,sizeof(char), wbreak, fdp);
    /* continue reading tilL EOF */
    while ( nbuf == wbreak ) {
      /* send plaintext */
      writen(socketFD, buffer, nbuf);

      /* send key file */
      memset(buffer, '\0', sizeof(buffer)); 
      fread(buffer, sizeof(char), nbuf, fdk);
      //printf("key: %d %s\n", nbuf, buffer);
      writen(socketFD, buffer, nbuf);

      /* read cipher text */
      memset(buffer, '\0', sizeof(buffer)); 
      readn(socketFD, buffer, nbuf);
      printf("%s", buffer);

      /* clear buffer for next loop */
      memset(buffer, '\0', sizeof(buffer)); 
      nbuf = fread(buffer,sizeof(char), nbuf, fdp);
    }

    /* last bits of file */
    buffer[strcspn(buffer, "\n")] = '@';
    writen(socketFD, buffer, nbuf);

    memset(buffer, '\0', sizeof(buffer)); 
    fread(buffer, sizeof(char), nbuf, fdk);
    //buffer[strcspn(buffer, "\n")] = '\0';
    //strcat(buffer, "@");
    writen(socketFD, buffer, nbuf);

    memset(buffer, '\0', sizeof(buffer)); 
    readn(socketFD, buffer, nbuf-1);
    printf("%s\n", buffer);

      //buffer[strcspn(buffer, "\n")] = '\0';
      //status = fread(buffer,sizeof(char), nbuf, fdp);

  /* server refused connection */
  } else {
    printf("CLIENT:otp_enc cannot use the port\n"); close(socketFD);
    exit(2);
  }

  
	/* Get return message from server */
  /*
	memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer array
	charsRead = readn(socketFD, buffer, 4);
  fprintf(stdout, "CLIENT: %s\n", buffer);
  */

	close(socketFD); // Close the socket
	return 0;
}

ssize_t checkchar(FILE *fd, int* badch) {
  int ch;
  ssize_t total = 0;
  while ( (ch = fgetc(fd)) != '\n') {
    total++;
    if ( ch < 'A' || ch > 'Z'){
      if ( ch != ' '){
      //printf("%c\n", ch);
      *badch = 1;
      }
    }
  }
  fseek(fd, 0, SEEK_SET);
  return total;
}
