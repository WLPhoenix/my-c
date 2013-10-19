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

typedef enum MyType {
  GROUP,
  COMMAND,
  TEMPLATE,
  MACRO,
  NOTE,
  GROUP_CALL,
  INVALID
};

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

void
fCopy(const char * f1,const char * f2)
{
   FILE * toRead;
   FILE * toWrite;
   toRead = fopen(f1,rb);
   toWrite = fopen(f2,wb);
   while(1)
   {
    byte stream[1];
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

char*
strAppend(char *st1, char *st2)
{
  int l1 = strnlen(st1);
  int l2 = strnlen(st2);
  char new[l1 + l2];
  strncpy( new, st1 );
  strncpy(*new[l1], st2 );
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
  FILE *fp = fopen( getenv("MY_HOME") );
  while(
}

int 
create_group(char* name) 
{ }

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
list_notes() 
{ }

int 
create_note(const char * name, const  char * content) 
{ }

int
echo_note(*char name)
{ }

int
drop_note(*char name)
{ }

int
edit_note(*char name)
{ }

/***********************
        Commands
 **********************/
int
list_commands() 
{ }

int 
create_command(*char name) 
{ }

int
echo_command(*char name)
{ }

int
drop_command(*char name)
{ }

int
edit_command(*char name)
{ }

/***********************
        Templates
 **********************/
int
list_templates() 
{ }

int 
create_template(*char name) 
{ }

int
echo_template(*char name)
{ }

int
drop_template(*char name)
{ }

int
edit_template(*char name)
{ }

/***********************
        Macros
 **********************/
int
list_macros() 
{ }

int 
create_macro(*char name) 
{ }

int
echo_macro(*char name)
{ }

int
drop_macro(*char name)
{ }

int
edit_macro(*char name)
{ }

/***********************
        Main
 **********************/
int
main(int argc, char *argv[])
{
  if (1 == argc)
  {
    print_help();
  }
  else
  {
    setup_folders();
    enum MyType type = find_type(argv[1]);
    if (GROUP_CALL == type)
    {
      printf("TODO: Implement actions for GROUP_CALL\n");
    }
    else if (INVALID != type)
    {
      printf("I know what to do here!\n");
    }
    else
    {
      printf("Invalid type or group.\n");
    } 
  }
  return 0;
}

