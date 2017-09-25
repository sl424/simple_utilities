#include<stdlib.h>
#include<stdio.h>
#include<time.h>
int main(int argc, char *argv[])
{
  if (argc < 2) { fprintf(stderr, "USAGE: %s num\n", argv[0]); exit(1);}

  int i, ch;
  srand(time(NULL));

  int n = atoi(argv[1]);

  for(i=0; i<n; i++){
    ch = rand() % 27;
    if (ch==26){ printf("%c", ' ');
    } else { printf("%c", ch+'A'); }
  }

  printf("\n");
  exit(EXIT_SUCCESS);
} 
