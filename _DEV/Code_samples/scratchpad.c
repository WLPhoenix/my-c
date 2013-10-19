#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char*
strAppend(const char *st1, int lmax1, const char *st2, int lmax2)
{
  int len1 = strlen(st1);
  int len2 = strlen(st2);
  len1 = (len1 < lmax1) ? len1 : lmax1;
  len2 = (len2 < lmax2) ? len2 : lmax2;

  printf("len1: %i\n", len1);
  printf("len2: %i\n", len2);
  
  char* new;
  new = (char*) malloc( len1 + len2 + 1 );
  strcpy( new, st1 );
  strcat( new, st2 );
  strcat( new, "\0");
  return new;
}

int
main(void)
{
  
  char* st1 = "abcd";
  char* st2 = "zyxw";  
  printf("%s\n", st1);
  printf("%s\n", st2);
  
  char* st3 = strAppend(st1, 100, st2, 100);
  printf("%s\n", st3);
  free(st3);
}
