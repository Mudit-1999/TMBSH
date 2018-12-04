#include "../header.h"

void select_operation_env(char **tokens)
{
  if(strcmp(tokens[0],"setenv\0") == 0)
  {
    fun_setenv(tokens);
  }
  else if(strcmp(tokens[0],"unsetenv\0")==0)
  {
    fun_unsetenv(tokens);
  }
  else if(strcmp(tokens[0],"getenv\0")==0)
  {
    fun_getenv(tokens);
  }
  else if(strcmp(tokens[0],"kjob\0") == 0)
  {
    kill_job(tokens);
  }
  else if (strcmp(tokens[0],"cd\0") == 0)
  {
    cd(tokens);
  }
  else if(strcmp(tokens[0],"quit\0") == 0 || strcmp(tokens[0],"exit\0") == 0 )
  {
    overkill();
    printf("Bye!!\n");
    exit(EXIT_FAILURE);
  }
  else if(strcmp(tokens[0],"overkill\0") == 0)
  {
    overkill();
  }
  else if (strcmp(tokens[0],"bg\0") == 0)
  {
    run_bg_process(tokens);
  }
  else if (strcmp(tokens[0],"fg\0") == 0)
  {
    run_fg_process(tokens);
  }
}


void fun_setenv(char ** tokens )
{
  char * name , *value;

  value = (char*)malloc(1000 * sizeof(char));
  if(value==nul)
  {
    fprintf(stderr, "Memory Allocation Error\n");
    return ;
  }
  if(tokens[1]==nul || tokens[2]==nul || tokens[3]!=nul)
  {
    fprintf(stderr,"Usage setenv var [value]\n" );
    return ;
  }
  name=tokens[1];
  int k=1;
  if(tokens[2][0]!='[')
  {
    fprintf(stderr,"Usage setenv var [value]\n" );
    return ;
  }
  while(tokens[2][k+1]!='\0')
  {
    // char a=tokens[2][k];
    strncat(value,&tokens[2][k],1);
    k++;
  }
  if(tokens[2][k]!=']')
  {
    fprintf(stderr,"Usage setenv var [value]\n");
    return ;
  }

  if(setenv(name,value,1)==-1)
  {
    perror("Error");
  }
  if(test_mode)
    printf("%s,%s\n",name,value);
}

void fun_unsetenv(char ** tokens )
{
  char *name;

  if(tokens[1]==nul ||  tokens[2]!=nul)
  {
    fprintf(stderr,"Usage unsetenv var\n" );
    return ;
  }
  name=tokens[1];
  if( unsetenv(name)==-1)
  {
    perror("Error");
  }
  if(test_mode)
    printf("%s\n",name);
}

void fun_getenv(char ** tokens )
{
  char *name ,*value;

  if(tokens[1]==nul ||  tokens[2]!=nul)
  {
    fprintf(stderr,"Usage getenv var\n");
    return ;
  }
  name=tokens[1];

  value = getenv(name);
  if(value==nul)
  {
    fprintf(stderr,"Variable %s does not exits\n",name);
  }
  else
  {
    printf("%s\n",value);
  }
  if(test_mode)
    printf("%s\n",name);
}
