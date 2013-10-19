#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>

const int MAX_PATH = 1023;
int
main(void)
{
  
  if(NULL == getenv("MY_HOME"))
  {
    char* home = getenv("HOME");
    int len = strlen(home);
    char new_myhome[len + 4];
    strcat( strcpy( new_myhome, home ), "/.my" );
    setenv("MY_HOME", new_myhome, 0);
  }
  if( mkdir( getenv("MY_HOME"), S_IRWXU ) == -1 )
    {
      return errno==EEXIST;
    }
}
