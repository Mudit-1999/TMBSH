#include "../header.h"


void prompt()
{
  const char* usr_name;
  char hst_name[100];
  char dir[100];

  usr_name=getenv("USER");
  int chk1=gethostname(hst_name,sizeof(hst_name));
  getcwd(dir,sizeof(dir));
  char curr_dirr[1000];

  int i=0,j=1;
  for (i = 0; home_dir[i]!='\0' && dir[i]!='\0'; i++)
  {
    if(home_dir[i]!=dir[i])
      break;
  }
  if ( i >= strlen(home_dir)  )
  {
    curr_dirr[0]='~';
    for (i ; dir[i]!='\0'; i++)
    curr_dirr[j++]=dir[i];
    curr_dirr[j++]='$';
    curr_dirr[j++]=' ';
    curr_dirr[j]='\0';
  }
  else
  {
    strcpy(curr_dirr,dir);
    strcat(curr_dirr,"$ ");
  }
  printf("%s@%s:%s",usr_name,hst_name,curr_dirr);
}
