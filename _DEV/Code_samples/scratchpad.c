#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/stat.h>
#include <errno.h>

const int MAX_LINE = 1023;

int
readline(char *in, int in_size, FILE *from) 
{  
  char *cptr;
  memset(in, 0, in_size * sizeof(char));
  int cond = (fgets(in, in_size, from)) != NULL;
  if (cond) {
    char* tmp = strchr( in, '\n' );
    tmp[0] = '\0';
  }
  return cond;
}

int
main(void)
{
  FILE* fp;
  if ((fp = fopen( "/Users/CH/tappant/404.html", "r" )) != NULL) {
    char in[MAX_LINE];
    while (readline(in, MAX_LINE, fp)) {
      puts(in);
    }
  }
}
