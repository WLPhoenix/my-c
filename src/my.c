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

typedef enum MyType {
  GROUP,
  COMMAND,
  TEMPLATE,
  MACRO,
  NOTE,
  GROUP_CALL,
  INVALID
};
/*
enum GroupCommands {
  LIST,
  ADD,
  DELETE,
  ECHO,
  HELP
}
*/
  
//enum myfunctions {
//  add, /* all */
//  delete, /* all */
//  list, /* all */
//  echo, /* all */
//  help,  /* all */
  
//  run, /* command, macro */
//  exec, /* command, macro */ /* alias for run */
//  make, /* template */
//};


/**
Utility functions
**/
void
strToLower(char *str) {
  for( int i = 0; str[i]; i++) {
	str[i] = tolower(str[i]);
  }
}

/**
Main program
**/
void
print_help()
{
  printf("TODO: Write print_help\n");
}

int
setup_folders()
{
  *char myhome = secure_getenv("MY_HOME");
  if (myhome != NULL) {
    myhome = secure_getenv("HOME") + "/.my";
    setenv("MY_HOME", myhome);
  }
  return mkdir(myhome);
}

int
is_user_defined_group(char *cmd)
{
  return 0;
}

enum MyType
find_type(char *cmd)
{
  strToLower(cmd);
  //Valid: 'group', 'groups', 'grp'
  if( strcmp("group",cmd)== 0 || strcmp("groups",cmd)==0 || strcmp("grp", cmd)==0 )
  {
	return GROUP;
  }
  //Valid: 'command', 'commands', 'cmd'
  else if( strcmp("command",cmd)== 0 || strcmp("commands",cmd)==0 || strcmp("cmd", cmd)==0 )
  {
	return COMMAND;
  }
  //Valid: 'template', 'templates'
  else if( strcmp("template",cmd)== 0 || strcmp("templates",cmd)==0 )
  {
	return TEMPLATE;
  }
  //Valid: 'macro', 'macros'
  else if( strcmp("macro",cmd)== 0 || strcmp("macros",cmd)==0 )
  {
	return MACRO;
  }
  //Valid: 'note', 'notes'
  else if( strcmp("note",cmd)== 0 || strcmp("notes",cmd)==0 )
  {
	return NOTE;
  }
  else if( is_user_defined_group(cmd) )
  {
	return GROUP_CALL;
  }
  else
  {
	return INVALID;
  }
}

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

