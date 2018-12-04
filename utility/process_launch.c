#include "../header.h"

bool bg_check(char** tokens)
{
  bool using_bg = false;
  int status;
  pid_t pid, wpid,ppid;
  for (int i = 0;tokens[i] != nul;i++)
  {
    if(strcmp(tokens[i],"&\0" )==0  ||  strcmp(tokens[i],"remindme\0" )==0 )
    {
      using_bg=true;
      if(strcmp(tokens[i],"&\0" )==0)
      tokens[i]=NULL;
      break;
    }
    else
    {
      for (int j = 0; tokens[i][j] != '\0'; j++)
      {
        if (tokens[i][j] == '&')
        {
          using_bg=true;
          tokens[i][j]='\0';
          break;
        }
      }
    }
  }
  return using_bg;
}
