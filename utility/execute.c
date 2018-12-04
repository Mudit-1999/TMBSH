#include "../header.h"

int execute(char** tokens,int cur_com,int tot_com)
{
  int file_to_read,file_to_write;

  for (int i = 0; tokens[i]!=nul; i++)
  {
    if(tokens[i][0]=='<')
    {
      file_to_read=open(tokens[i+1],O_RDONLY);
      if(dup2(file_to_read,0)==-1)
      {
        perror("Dup Error");
        return 1;
      }
      close(file_to_read);
      tokens[i]=nul;

    }
    else if(tokens[i][0]=='>')
    {
      if(tokens[i][1]=='>')
      {
        file_to_write=open(tokens[i+1],O_CREAT | O_WRONLY |O_APPEND, 0644);
      }
      else
      {
        file_to_write=open(tokens[i+1],O_CREAT | O_WRONLY | O_TRUNC, 0644);
      }
      if(dup2(file_to_write,1)==-1)
      {
        perror("Dup Error");
        return 1;
      }
      close(file_to_write);
      tokens[i]=nul;
    }
  }

  if (strcmp(tokens[0],"echo\0") == 0)
  {
    echo(tokens);
    return 0;
  }
  else if (strcmp(tokens[0],"pwd\0") == 0)
  {
    pwd();
    return 0;
  }
  else if (strcmp(tokens[0],"pinfo\0") == 0)
  {
    pinfo(tokens);
    return 0;
  }
  else if (strcmp(tokens[0],"ls\0") == 0)
  {
    ls(tokens);
    return 0;
  }
  else if(strcmp(tokens[0],"clock\0") == 0)
  {
    clock_build(tokens);
    return 0;
  }
  else if(strcmp(tokens[0],"jobs\0") == 0)
  {
    print_jobs();
    return 0;
  }
  else if (strcmp(tokens[0],"bg\0") == 0)
  {
    return 1;
  }
  else if (strcmp(tokens[0],"fg\0") == 0)
  {
    return 1;
  }

  else if (strcmp(tokens[0],"cd\0") == 0)
  {
    // cd(tokens);
    return 1;

  }
  else if(strcmp(tokens[0],"quit\0") == 0 || strcmp(tokens[0],"exit\0") == 0 )
  {
    // exit(EXIT_FAILURE);
    return 1;
  }
  else if(strcmp(tokens[0],"overkill\0") == 0)
  {
    if(test_mode)
    printf("killing all\n");
    return 1;
  }
  else if(strcmp(tokens[0],"kjob\0") == 0)
  {
    return 1;
  }
  else if(strcmp(tokens[0],"setenv\0") == 0)
  {
    return 1;
  }
  else if(strcmp(tokens[0],"unsetenv\0")==0)
  {
    return 1;
  }
  else if(strcmp(tokens[0],"getenv\0")==0)
  {
    return 1;
  }
  else
  return -1;
}
