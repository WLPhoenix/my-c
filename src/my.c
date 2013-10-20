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
#include <dirent.h>

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

int
nextline(char *in, int in_size, FILE *from) 
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

dirent *
nextfile(char* filename, int size_filename, DIR* dirp)
{
  return (*filename = (char*) readdir(dirp)) != NULL;
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
{ 
  
}

int
edit_group(char* name)
{ }

/***********************
        Notes
 **********************/
int
list_notes(const char * group) 
{
   
}

int 
create_note(const char * group, const char * name, const  char * content) 
{ 
  const char* myhome = getenv("MY_HOME");
  int homelen = strlen(myhome);
  int namelen = strlen(name);
  int grouplen = strlen(group);

  char filename[ 2 + namelen];
  char listpath [ homelen+ grouplen+ 6];
  char grouppath[ 5+ grouplen + namelen];
  char fullgrouppath[2+ grouplen + homelen];
  char fullpath[homelen+grouplen+namelen+5];

   
strcpy( fullpath, myhome );
strcat( fullpath, "/");
strcat( fullpath, group);
strcat( fullpath, "/");
strcpy( fullgrouppath, fullpath);
strcat( filename,"N.");
strcat( filename,name);
strcat( fullpath,filename);
  
strcpy(listpath, myhome);
strcat(listpath, "/");
strcat(listpath, group);
strcat(listpath, "/list");
 
  puts(filename);
  puts(fullpath);
  FILE *test = fopen(fullpath,"r");
  if( test == NULL)
  {
    
    FILE *list = fopen(listpath, "a");
    FILE *fp = fopen(fullpath, "a" );
    if (NULL != fp) 
    {
      fputs( content, fp );
      fputs( filename,list);
      fflush( fp );
      fflush( list);
      fclose( fp );
      fclose( list);
    }
    else 
    {
      return -1;
    }
  }
  else 
  {
    puts("File already exists");
    fclose(test);
    return -1;
  }
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
  create_group("test");
  create_note("test","testicles","semen");
  return 0;
}

