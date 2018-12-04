#include "../header.h"

char ** check_pipe(char* command_with_pipe,int *cnt)
{
  char* token;
  char ** tokens=malloc(100*sizeof(char*));
  if (tokens==nul)
  {
    fprintf(stderr, "Memory Allocation Error" );
    exit(EXIT_FAILURE);
  }
  token=strtok(command_with_pipe,"|");
  while(token!=nul)
  {
    tokens[(*cnt)++]=token;
    token=strtok(nul,"|");
  }
  tokens[*cnt]=nul;
  return tokens;
}

void pipe_implement(int cur_com,int tot_com,int in,int out)
{
  if(tot_com==1)
    return;
  if (in != 0 && cur_com !=1)
  {
    dup2(in,0);
    close(in);
  }
  if(tot_com!=cur_com)
  {
    if(out!=1)
    {
      dup2(out,1);
      close(out);
    }
  }
  // else if(tot_com==cur_com)
  // {
      // dup2(initial_std_out,1);
  // }
}
