#include "header.h"

int main(int argc, char const *argv[])
{
  int i=0,j=0;
  char path_exe[100]="/proc/";
  char res[100];
  char temp[100];
  int cur_pid=getpid();
  test_mode=true;
  initial_std_in = dup(0);
  initial_std_out =dup(1);
  sprintf(res, "%d",cur_pid);
  strcat(path_exe,res);
  strcat(path_exe,"/exe");
  while(1)
  {
    if (readlink(path_exe,temp,100) == -1)
    {
      perror("Error:");
    }
    break;
  }
  for (i =strlen(temp)-1; i >=0 ; i--)
  {
    if(temp[i]=='/')
      break;
  }
  for (j = 0; j < i; j++)
  {
    home_dir[j]=temp[j];
  }
  home_dir[j]='\0';
  handlesignal();
  while(1)
  {
    prompt();
    input_cmd();
    handlesignal();
  }
  return 0;
}
