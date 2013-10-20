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
#include <unistd.h>
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
{
  
}

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
  int g = 0;
  int x = 0;
  int V = 0;
  int c = 0;
  int l = 0;
  int e = 0;
  int d = 0;
  int u = 0;
  int h = 0;
  int G = 0;
  int T = 0;
  int C = 0;
  int M = 0;
  int N = 0;
  char * group;
  char * label;
  char * content;
  int opt ;
  while((opt= getopt(argc,argv, "V(verbose)g:x(execute)c(create)l(list)e(echo):d(drop):u(edit):h(help)G(group):T(template):C(command)::M(macro)::N(note)::"))!= -1)
  {
    switch(opt)
    {
     case 'g':
          g=1;
          group = optarg;
          break;
     case 'V':
          V = 1;
          break;
     case 'x':
          x = 1;
          break;
     case 'c':
          c = 1;
          break; 
     case 'l':
          l = 1;
          break;
     case 'e':
          e = 1;
          label = argv[optind];
          break;
     case 'd':
          d = 1;
          label = argv[optind];
          break;
     case 'u':
          u = 1;
          label = argv[optind];
          break;
     case 'h':
          h = 1;
          break;
     case 'G':
          G = 1;
          label = argv[optind];
          break;
     case 'T':
          T = 1;
          label = argv[optind];
          break;
     case 'C':
          C = 1;
          label = argv[optind];
          content = argv[optind+1];
          break;
     case 'N':
          N = 1;
          label = argv[optind];
          content = argv[optind+1];
          break;       
     default:
        puts("test");
        break;


    }
 }
     if(g == 1)
     {
        if(c == 1)
        {
           if(G ==1)
           {
             create_group(label);
             //break;
           }
           else if(N == 1)
           {
              create_note(group, label, content);
              //break;
           }
           
           else if(C == 1)
           {
             create_command(label,group,content);
             //break;
           }
           else if(T == 1)
           {
             create_template(group,label);
             //break;
           }
           else
           {
              puts("Invalid Command");
              return -1;
           }
        }
        else if(l == 1)
        {
           if(N == 1)
           {
              list_notes(group);
              //break;
           }
           else if(G == 1)
           {
              list_groups();
              //break;
           }
           else
           {
              puts("Invalid Command");
              return -1;   
           }
        }
        else if(x == 1)
        {
           if(C == 1)
           {
              execute_command(group,label);
              //break;
           }  
           else if(T == 1)
           {
           }
           else
           {
              puts("Invalid Command");
              return -1;
           }
        }

     }
     else if(c == 1)
     {
          if(G == 1)
          {
             create_group(label);
             //break;
          }
          else
          {
              puts("Invalid Command");
              return -1;
          }
     } 
   
 //   puts(argv[1]);
 //  puts(argv[2]);
 //   puts(argv[3]);
 //   puts(argv[4]);
 //   puts(argv[5]);
 //   puts(argv[6]);
    

  return 0;
}

