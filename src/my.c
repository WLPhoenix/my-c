/*
 author: Cory Hughes

 Compile w/ C99 minimum

 my is a tool for storing a number of small items (directory templates, shell commmands, notes, etc.),
 then recalling them by name. It also allows the grouping of these items, to allow naming overlaps
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/stat.h>
#include <errno.h>

const int MAX_PATH = 1023;

/***********************
        Utility
 **********************/
void
strToLower(char *str) 
{
  for( int i = 0; str[i]; i++) 
  {
    str[i] = tolower(str[i]);
  }
}



char *Readline(char *in) 
{ 
char *cptr;
if (cptr = fgets(in, MAX_LINE, stdin)) 
 /* kill preceding whitespace but leave \n so we're guaranteed to have something */
while(*cptr == ' ' || *cptr == '\t')
 {
 cptr++; 
  return cptr; 
 } 
else { return 0; } 
}


void
fCopy(const char * f1,const char * f2)
{
   FILE * toRead;
   FILE * toWrite;
   toRead = fopen(f1, "rb" );
   toWrite = fopen(f2, "wb" );
   while(1)
   {
    char stream[1];
    fread(stream,1,1,toRead);
    if(stream[1] == EOF)
    {
      break;
    }
    else
    {
      fwrite(stream,1,1,toWrite);
    }
   }
   fclose(toRead);
   fclose(toWrite);
}

/***********************
        Meta
 **********************/

void
print_help()
{
  printf("TODO: Write print_help\n");
}

int
setup_home()
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

/***********************
        Groups
 **********************/
int
list_groups() 
{
  const char* myhome = getenv("MY_HOME");

  int len = strlen(myhome);
  char filename[len + 7];
  strcat( strcpy( filename, myhome ), "/groups" );
  FILE *fp = fopen( filename, "rm" );
  if (NULL != fp) {
    int stream;
    while((stream = getc(fp)) != EOF) {
      putchar(stream);
    }
  }
}

int 
create_group(const char* name) 
{ 
  const char* myhome = getenv("MY_HOME");
  int homelen = strlen(myhome);
  int namelen = strlen(name);

  char filename[homelen + namelen + 1];
  char groupfile[homelen + 7];
  
  strcat( strcat( strcpy( filename, myhome ), "/"), name  );
  strcat( strcpy( groupfile, myhome ), "/groups" );

  if( mkdir( filename, S_IRWXU ) == 0)
  {
    FILE *fp = fopen( groupfile, "a" );
    if (NULL != fp) {
      fputs( name, fp );
      fputs( "\n", fp );
      fflush( fp );
      fclose( fp );
    }
    else 
    {
      return -1;
    }
  }
  else if (errno == EEXIST) 
  {
    printf("Group '%s' already exists.\n", name);
  } 
  else 
  {
    return -1;
  }

}

int
echo_group(char* name)
{ }

int
drop_group(char* name)
{ }

int
edit_group(char* name)
{ }

/***********************
        Notes
 **********************/
int
list_notes(const char * group) 
{ }

int 
create_note(const char * group, const char * name, const  char * content) 
{ 
  
}

int
echo_note(const char * group, const char* name)
{ }

int
drop_note(const char * group, const char* name)
{ }

int
edit_note(char* name)
{ }

/***********************
        Commands
 **********************/
int
list_commands() 
{ }

int 
create_command(char* name) 
{ }

int
echo_command(char* name)
{ }

int
drop_command(char* name)
{ }

int
edit_command(char* name)
{ }

/***********************
        Templates
 **********************/
int
list_templates() 
{ }

int 
create_template(char* name) 
{ }

int
echo_template(char* name)
{ }

int
drop_template(char* name)
{ }

int
edit_template(char* name)
{ }

/***********************
        Macros
 **********************/
int
list_macros() 
{ }

int 
create_macro(char* name) 
{ }

int
echo_macro(char* name)
{ }

int
drop_macro(char* name)
{ }

int
edit_macro(char* name)
{ }

/***********************
        Main
 **********************/
int
main(int argc, char *argv[])
{
  setup_home();
  list_groups();
  create_group("booze");
  list_groups();
  return 0;
}

