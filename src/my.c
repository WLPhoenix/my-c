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
#include <sys/types.h>

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

//dirent *
//nextfile(char* filename, int size_filename, DIR* dirp)
//{
//  return (*filename = (char*) readdir(dirp)) != NULL;
//}

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
{ 
  
}

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
  const char* myhome = getenv("MY_HOME");
  int homelen = strlen(myhome);
  int grouplen = strlen(group);

  char listpath [ homelen + grouplen + 7 ];
  memset( listpath, 0, (homelen + grouplen + 6) * sizeof(char) );
  strcpy( listpath, myhome );
  strcat( listpath, "/");
  strcat( listpath, group);
  strcat( listpath, "/.list");

  FILE * fp;
  if ( fp = fopen(listpath, "r") ) {
    char line[1023];
    while( nextline(line, 1023, fp) ) {
      if ( line[0] == 'N' ) {
	puts( &line[2] );
      }
    }
  }
}

int 
create_note(const char * group, const char * name, const  char * content) 
{ 
  const char* myhome = getenv("MY_HOME");
  int homelen = strlen(myhome);
  int namelen = strlen(name);
  int grouplen = strlen(group);

  char filename[ namelen + 2];
  memset( filename, 0, (namelen + 2) * sizeof(char) );
  strcat( filename, "N.");
  strcat( filename, name);

  //printf( "filename: %s\n", filename );

  //char grouppath[ grouplen + namelen + 5];

  char fullgrouppath[ grouplen + homelen + 2];
  memset( fullgrouppath, 0, (grouplen + homelen + 2) * sizeof(char) );
  strcpy( fullgrouppath, myhome );
  strcat( fullgrouppath, "/");
  strcat( fullgrouppath, group);

  char fullpath[ homelen + grouplen + namelen + 5 ];
  memset( fullpath, 0, (homelen + grouplen + namelen + 5) * sizeof(char) );
  strcpy( fullpath, fullgrouppath);
  strcat( fullpath, "/");
  strcat( fullpath, filename);

  char listpath [ homelen + grouplen + 7 ];
  memset( listpath, 0, (homelen + grouplen + 6) * sizeof(char) );
  strcpy( listpath, fullgrouppath);
  strcat( listpath, "/.list");
  

  //printf( "fullgrouppath: %s\n", fullgrouppath );
  //printf( "fullpath: %s\n", fullpath );
  //printf( "listpath: %s\n", listpath );
  
  FILE *test = fopen(fullpath,"r");

  struct stat sb;
  if( stat(fullpath, &sb) == 0)
  {
    //printf("File name: %s\n", fullpath);
    //printf("File status: %i\n", S_ISREG(sb.st_mode) );
    puts("File already exists");
    fclose(test);
    return -1;
  }
  else 
  {
    FILE *list = fopen(listpath, "a");
    FILE *fp = fopen(fullpath, "a" );
    if (NULL != fp) 
    {
      fputs( content, fp );
      fputs( "\n", fp );
      fflush( fp );
      fclose( fp );

      if( list != NULL )
      {
	fputs( filename, list);
	fputs( "\n", list );
	fflush( list);
	fclose( list);
      }
      else 
      {
	return -1;
      }
    }
    else 
    {
      return -1;
    }
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
create_command(const char* name, const char* group, const char* content) 
{
  const char* myhome = getenv("MY_HOME");
  int homelen = strlen(myhome);
  int namelen = strlen(name);
  int grouplen = strlen(group);

  char filename[ namelen + 2];
  memset( filename, 0, (namelen + 2) * sizeof(char) );
  strcat( filename, "C.");
  strcat( filename, name);

  //printf( "filename: %s\n", filename );

  //char grouppath[ grouplen + namelen + 5];

  char fullgrouppath[ grouplen + homelen + 2];
  memset( fullgrouppath, 0, (grouplen + homelen + 2) * sizeof(char) );
  strcpy( fullgrouppath, myhome );
  strcat( fullgrouppath, "/");
  strcat( fullgrouppath, group);

  char fullpath[ homelen + grouplen + namelen + 5 ];
  memset( fullpath, 0, (homelen + grouplen + namelen + 5) * sizeof(char) );
  strcpy( fullpath, fullgrouppath);
  strcat( fullpath, "/");
  strcat( fullpath, filename);

  char listpath [ homelen + grouplen + 7 ];
  memset( listpath, 0, (homelen + grouplen + 6) * sizeof(char) );
  strcpy( listpath, fullgrouppath);
  strcat( listpath, "/.list");
  

  //printf( "fullgrouppath: %s\n", fullgrouppath );
  //printf( "fullpath: %s\n", fullpath );
  //printf( "listpath: %s\n", listpath );
  
  FILE *test = fopen(fullpath,"r");

  struct stat sb;
  if( stat(fullpath, &sb) == 0)
  {
    //printf("File name: %s\n", fullpath);
    //printf("File status: %i\n", S_ISREG(sb.st_mode) );
    puts("File already exists");
    fclose(test);
    return -1;
  }
  else 
  {
    FILE *list = fopen(listpath, "a");
    FILE *fp = fopen(fullpath, "a" );
    if (NULL != fp) 
    {
      fputs( content, fp );
      fputs( "\n", fp );
      fflush( fp );
      fclose( fp );

      if( list != NULL )
      {
	fputs( filename, list);
	fputs( "\n", list );
	fflush( list);
	fclose( list);
      }
      else 
      {
	return -1;
      }
    }
    else 
    {
      return -1;
    }
  }
}

int
execute_command(const char * group, const char * name)
{
    const char* myhome = getenv("MY_HOME");
  int homelen = strlen(myhome);
  int namelen = strlen(name);
  int grouplen = strlen(group);

  char filename[ namelen + 2];
  memset( filename, 0, (namelen + 2) * sizeof(char) );
  strcat( filename, "C.");
  strcat( filename, name);

  //printf( "filename: %s\n", filename );

  //char grouppath[ grouplen + namelen + 5];

  char fullgrouppath[ grouplen + homelen + 2];
  memset( fullgrouppath, 0, (grouplen + homelen + 2) * sizeof(char) );
  strcpy( fullgrouppath, myhome );
  strcat( fullgrouppath, "/");
  strcat( fullgrouppath, group);

  char fullpath[ homelen + grouplen + namelen + 5 ];
  memset( fullpath, 0, (homelen + grouplen + namelen + 5) * sizeof(char) );
  strcpy( fullpath, fullgrouppath);
  strcat( fullpath, "/");
  strcat( fullpath, filename);
  
  FILE * fp = fopen(fullpath,"r");
  char  buff[1024];
  fread(buff, sizeof(char),1023, fp);
  FILE * out;
  out = popen(buff,"r");
  if( out == NULL)
  {
     puts("Command failed to fork.");
     return -1;
  }
  else
  { 
     char output[1053];
     strcpy(output,"Command: ");
     strcat(output, buff);
     strcat(output, "started successfully");
     puts(output);
     int c;
     if(out)
     {
          while(( c = getc(out)) != EOF)
          {
             putchar(c);
          }  
     } 
     return 0;
  }
  
  
  
}

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
create_template(const char* name, const char * group) 
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
<<<<<<< HEAD
  DIR *dp;
  struct dirent *ep;     
  dp = opendir ("./");

  if (dp != NULL)
  {
    while (ep = readdir (dp))
      if(ep->d_name != ".." || ep -> d_name != ".")
      puts (ep->d_name);

    (void) closedir (dp);
  }
  else
    perror ("Couldn't open the directory");
=======
  create_group("test");
  create_note("test","adam","semen");
  create_note("test","beta","piss");
  create_note("test","gamma","poop");

  list_notes("test");
>>>>>>> 0117ee338d93b987ef1b17795407647f1c864a99
  return 0;
}

