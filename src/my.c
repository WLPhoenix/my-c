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

#include <unistd.h>
#include <uuid/uuid.h>

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


int
fCopy(const char * f1,const char * f2)
{
  printf("Copying '%s' to '%s'\n", f1, f2);
   FILE * toRead;
   FILE * toWrite;
   long lSize;
   char * buffer;
   size_t result;

   toRead = fopen ( f1 , "rb" );
   if (toRead==NULL) {fputs ("File error",stderr); exit (1);}

   toWrite = fopen( f2, "wb" );
   if (toWrite==NULL) {fputs ("File error",stderr); exit (1);}

   buffer = (char*) malloc (sizeof(char)*255);
   if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

   while(result != 0) {
     result = fread (buffer,1,255,toRead);
    // if (result != 255) {fputs ("Reading error",stderr); exit (3);}
     fputs(buffer, toWrite);
   }
   
   // terminate
   fclose (toRead);
   fclose (toWrite);
   free (buffer);
   return 0;
}

int
dirsize(char* dirname)
{
  DIR * dirp;
  if( NULL != (dirp = opendir(dirname)) ) {
    struct dirent * file;	
    int filecount = 0;
    while( NULL != (file = readdir(dirp)) ) {
      ++filecount;
    }
    return filecount;
    closedir(dirp);
  }
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
echo_group(char* group)
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
       line[1] = '\t';
       puts(line);
      }
    }
   

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
  


  struct stat sb;
  if( stat(fullpath, &sb) == 0)
  {
    int c;
    FILE *fp = fopen(fullpath, "r");
         if(fp)
     {
          while(( c = getc(fp)) != EOF)
          {
             putchar(c);
          }  
     } 
     else
     {
       puts("Error opening file");
     }
  }
  else 
  {
    puts("File does not exist.");
   
    return -1;
  }

  
}

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
list_commands(const char * group) 
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
      if ( line[0] == 'C' ) {
	puts( &line[2] );
      }
    }
  } 
}

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
echo_command(const char* name, const char * group)
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
  


  struct stat sb;
  if( stat(fullpath, &sb) == 0)
  {
    int c;
    FILE *fp = fopen(fullpath, "r");
         if(fp)
     {
          while(( c = getc(fp)) != EOF)
          {
             putchar(c);
          }  
     } 
     else
     {
       puts("Error opening file");
     }
  }
  else 
  {
    puts("File does not exist.");
   
    return -1;
  }
 
}

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
list_templates(const char * group) 
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
      if ( line[0] == 'T' ) {
	puts( &line[2] );
      }
    }
  }
}

int
__create_template__(char* labelpath, char* mappath, char* parent, char* filepath)
{
  puts( "__create_template__" );
  printf( "labelpath: %s\n", labelpath );
  printf( "mappath: %s\n", mappath );
  printf( "parent: %s\n", parent );
  printf( "filepath: %s\n", filepath );

  char* filename = strrchr( filepath, '/' );
  if(NULL == filename)
  {
    return -1;
  };
  filename = &filename[1];
  printf( "filename: %s\n", filename);

  struct stat sb;
  if( stat(filepath, &sb) == 0 )
  {
    if( S_ISDIR(sb.st_mode) ) 
    {
      //Append this file to the directory listing
      //d <permissions> <path>
      int parent_size = strlen(parent);
      int filename_size = strlen(filename);
      int record_size = parent_size + filename_size;
      char record[record_size];
      strcpy( record, parent );
      strcat( record, filename );

      FILE * fp;
      if( NULL != (fp = fopen(labelpath, "a")) )
      {
	fprintf( fp, "d\t%s\t%s\n", "|x|", record );
      } 

      // Since it's a directory, recurse on all files below this
      DIR * dirp;
      int files_size = dirsize(filepath);
      char* files[files_size];
      
      if ( NULL != (dirp = opendir(filepath)) ) 
      {
	struct dirent * file;
	int i = 0;
	
	while( NULL != (file = readdir(dirp)) ) 
	{
	  files[i++] = file->d_name;
	}
	closedir(dirp);
      }
      for( int i = 0; i < files_size; i++ )
      {
	int newfile_size = strlen( files[i] );
	// These checks prevent you from infinitely recursing on . and ..
	if( newfile_size <= 0 ) 
	{
	  continue;
	}
	else if( newfile_size == 1 ) 
	{
	  if ( files[i][0] == '.' )
	  {
	    continue;
	  }
	} 
	else if ( newfile_size == 2 ) 
	{
	  if ( files[i][0] == '.' && files[i][1] == '.' )
	  {
	    continue;
	  }
	}

	//Create the info for the recursive call
	char newparent[record_size + 1];
	memset( newparent, 0, sizeof(char) * (record_size + 1) );
	strcpy( newparent, record );
	strcat( newparent, "/" );

	char newfile[record_size + newfile_size + 1];
	memset( newfile, 0, sizeof(char) * (record_size + newfile_size + 1) );
	strcpy( newfile, newparent );
	strcat( newfile, files[i] );

	__create_template__(labelpath, mappath, newparent, newfile);
      }
    } 
    else if ( S_ISREG(sb.st_mode ) )
    {
      uuid_t uuid_key;
      uuid_generate( (unsigned char *) uuid_key);
      int key_size = 36;
      char key[key_size];
      uuid_unparse( uuid_key, key );

      //Append this file to the directory listing
      //d <permissions> <path>
      int parent_size = strlen(parent);
      int record_size = parent_size + key_size;
      char record[record_size];
      strcpy( record, parent );
      strcat( record, key );
      
      FILE * fp;
      if( NULL != (fp = fopen(labelpath, "a")) )
      {
	fprintf( fp, "f\t%s\t%s\n", "|x|", record );
      }

      //Because this is a file, we want to copy it to our directory to be restored later
      int labelpath_size = strlen(labelpath);
      char cpytarget[labelpath_size + key_size + 1];
      strcpy( cpytarget, labelpath );
      strcat( cpytarget, "." );
      strcat( cpytarget, key );

      fCopy( filepath, cpytarget );
      
      //<uuid> <filename>
      FILE * mapp;
      if( NULL != (mapp = fopen(mappath, "a")) )
      {
	fprintf( mapp, "%s\t%s\n", key, filename );
      }
    }
  }
}

int 
create_template(char* group, const char* name, char* files[], int files_size) 
{
  char* myhome = getenv("MY_HOME");

  int homelen = strlen(myhome);
  int grouplen = strlen(group);
  int namelen = strlen(name);

  int grouppath_size = homelen + grouplen + 1;
  char grouppath[grouppath_size];
  memset( grouppath, 0, sizeof(char) * grouppath_size );
  strcpy( grouppath, myhome );
  strcat( grouppath, "/" );
  strcat( grouppath, group );

  int labelpath_size =  grouppath_size + namelen + 3;
  char labelpath[labelpath_size];
  memset( labelpath, 0, sizeof(char) * labelpath_size );
  strcpy(labelpath, grouppath);
  strcat(labelpath, "/T.");
  strcat(labelpath, name);

  int mappath_size =  grouppath_size + namelen + 3;
  char mappath[mappath_size];
  memset( mappath, 0, sizeof(char) * mappath_size );
  strcpy( mappath, grouppath);
  strcat( mappath, "/Z.");
  strcat( mappath, name);

  puts("About to call __create_template__");
  for( int i = 0; i < files_size; i++ )
  {
    puts("");
    puts(files[i]);
    __create_template__(labelpath, mappath, "./", files[i]);
  }
}

int
create_template_from_current(char* group, const char* name) {
  char cwd[1023];
  if( NULL != getcwd(cwd, 1023) ) {
    
    DIR * dirp;
    int files_size = dirsize(cwd) - 2;
    char* files[files_size];
    
    if ( NULL != (dirp = opendir(cwd)) ) {
      struct dirent * file;
      int i = 0;
	
      while( NULL != (file = readdir(dirp)) ) 
      {
	//	puts( file->d_name );
	if ( (strcmp( ".", file->d_name ) != 0) && (strcmp ( "..", file->d_name ) != 0) )
	{
	  //	  puts( "Adding file to list." );
	  int cwd_size = strlen(cwd);
	  int fname_size = strlen(file->d_name);
	  char* path = malloc(sizeof(char) * (cwd_size + fname_size + 2));
	  memset(path, 0, sizeof(char) * (cwd_size + fname_size + 2));

	  strcpy( path, cwd );
	  strcat( path, "/" );
	  strcat( path, file->d_name );
	  
	  puts( path );

	  files[i++] = path;
	}
      }
      closedir(dirp);
    }
    int out = create_template(group, name, files, files_size);
    for (int j = 0; j < files_size; j++) 
    {
      free(files[j]);
    }
  }
}

int
echo_template(const char* name,const char * group)
{
  const char* myhome = getenv("MY_HOME");
  int homelen = strlen(myhome);
  int namelen = strlen(name);
  int grouplen = strlen(group);

  char filename[ namelen + 2];
  memset( filename, 0, (namelen + 2) * sizeof(char) );
  strcat( filename, "T.");
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
  


  struct stat sb;
  if( stat(fullpath, &sb) == 0)
  {
    int c;
    FILE *fp = fopen(fullpath, "r");
         if(fp)
     {
          while(( c = getc(fp)) != EOF)
          {
             putchar(c);
          }  
     } 
     else
     {
       puts("Error opening file");
     }
  }
  else 
  {
    puts("File does not exist.");
   
    return -1;
  }
}

int
execute_template(const char* group, const char* name)
{
  char* myhome = getenv("MY_HOME");

  int homelen = strlen(myhome);
  int grouplen = strlen(group);
  int namelen = strlen(name);

  int grouppath_size = homelen + grouplen + 1;
  char grouppath[grouppath_size];
  memset( grouppath, 0, sizeof(char) * grouppath_size );
  strcpy( grouppath, myhome );
  strcat( grouppath, "/" );
  strcat( grouppath, group );

  int labelpath_size =  grouppath_size + namelen + 3;
  char labelpath[labelpath_size];
  memset( labelpath, 0, sizeof(char) * labelpath_size );
  strcpy(labelpath, grouppath);
  strcat(labelpath, "/T.");
  strcat(labelpath, name);

  int mappath_size =  grouppath_size + namelen + 3;
  char mappath[mappath_size];
  memset( mappath, 0, sizeof(char) * mappath_size );
  strcpy( mappath, grouppath);
  strcat( mappath, "/Z.");
  strcat( mappath, name);


  FILE * recordp;
  FILE * mapp;

  if( NULL != (recordp = fopen(labelpath, "r")) )
  {
    if( NULL != (mapp = fopen(mappath, "r" )) )
    {
      char line[1023];
      while( nextline(line, 1023, recordp) )
      {
	puts(line);
	if( 'd' == line[0] ) 
	{
	  char* path = strrchr( line, '\t');
	  if( NULL == path) { continue; } 
	  path = &path[1];
	  mkdir( path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
	}
	else if( 'f' == line[0] )
	{
	  char* uuid_key = strrchr( line, '/');
	  if( NULL == uuid_key) { continue; }
	  uuid_key = &uuid_key[1];

	  int key_size = strlen(uuid_key);
	  char key[key_size];
	  strcpy( key, uuid_key );
	  uuid_key[0] = '\0';
	  
	  char* trgtpath = strrchr( line, '\t' );
	  if( NULL == trgtpath) { continue; }
	  trgtpath = &trgtpath[1];

	  //At this point we have 'trgtpath' and 'key'
	  char* mapline = malloc( sizeof(char) * (255 + 37) );
	  memset( mapline, 0, sizeof(char) * (255 + 37) );

	  char* filename;
	  while( nextline( mapline, (255 + 37), mapp ) )
	  {
	    char* map_key = mapline;
	    char* separator = strrchr(mapline, '\t');
	    separator[0] = '\0';
	    puts("Seeking file.");
	    printf("Key 1: %s\n", key);
	    printf("Key 2: %s\n", map_key);
	    if (strcmp( key, map_key ) == 0)
	    {
	      filename = &separator[1];
	      printf("Located file: %s\n", filename);
	      break;
	    }
	  }
	  int trgtpath_size = strlen( trgtpath );
	  int filename_size = strlen( filename );

	  int frompath_size = labelpath_size + 37 ;
	  char frompath[frompath_size];
	  strcpy( frompath, labelpath );
	  strcat( frompath, "." );
	  strcat( frompath, key );
	  
	  int topath_size = trgtpath_size + filename_size;
	  char topath[topath_size];
	  strcpy( topath, trgtpath );
	  strcat( topath, filename );

	  fCopy( frompath, topath );
	  free( mapline );
	}
      }

      fclose( mapp );
    }

    fclose( recordp );
  }
}

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
             create_template_from_current(group,label);
             //break;
           }
           else
           {
              puts("Invalid Command");
              return -1;
           }
        }
         
        if(e == 1)
        {
           if(N == 1)
           {
              echo_note(group,label);
           }
           else if( C == 1)
           {
               echo_command(label, group);
           }
           else if (T == 1)
           {
               echo_template(label, group);
           }
        }
        else if(l == 1)
        {
           if(N == 1)
           {
              list_notes(group);
              //break;
           }
           else if(C == 1)
           {
              list_commands(group);
              //break;
           }
           else if(T == 1)
           {
              list_templates(group);
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
 	     execute_template(group,label);
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
     else if(e == 1)
        {
           if(G == 1)
           {
              echo_group(label);
           }
        }

     
 

  return 0;
}

