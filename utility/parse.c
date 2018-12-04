#include "../header.h"

char* separate(char * line )
{
  int cnt=0;
  char delimiters[] = ";\n";
  char* token;
  char** cmd_list =malloc(100*sizeof(char*));

  if (cmd_list==nul)
  {
    fprintf(stderr, "Memory Allocation Error" );
    exit(EXIT_FAILURE);
  }

  token=strtok(line,delimiters);
  while(token!=nul)
  {
    cmd_list[cnt++]=token;
    token=strtok(nul,delimiters);
  }
  cmd_list[cnt]=nul;  // separated by ; or \n

  for (int i = 0; cmd_list[i]!=nul; i++)
  {
    int tot_com=0;
    char** com_without_pipe = check_pipe(cmd_list[i] , &tot_com);
    int in =0,fd_p[2];

    for (int k = 0; com_without_pipe[k]!=nul; k++)
    {
      char ** tokens=parse(com_without_pipe[k]);
      if(pipe(fd_p) != 0)
      {
        perror("pipe failed");
      }
      pid_t pid,wpid;
      int status;
      bool using_bg=bg_check(tokens);
      if ( (pid = fork ()) == 0)
      {
        pipe_implement(k+1,tot_com,in,fd_p[1]);
        if( using_bg==true ||  execute(tokens,k+1,tot_com) == -1)
        {
//          if(using_bg)
          setpgid(0,0);
          if( execvp(tokens[0], tokens) == -1)
          {
            perror("ERROR:");
          }
        }
        exit(EXIT_FAILURE);
      }
      if(!using_bg)
      {
        add_to_fglist(pid,tokens[0]);
        do
        {
          wpid = waitpid (pid, &status, WUNTRACED);
        }
        while(!WIFEXITED(status) && !WIFSTOPPED(status) && !WIFSIGNALED(status));
        if(WIFSIGNALED(status))
        {
          fprintf(stderr,"Process with pid:%d interrupted by a signal\n",pid);
        }
        else if(WIFSTOPPED(status))
        {
          fprintf(stderr,"[%d]\tStopped\t\t%s\n",add_to_bglist(pid,tokens[0],0),tokens[0]);
        }

        free(cur_fg_process);
      }
      else
      {
        add_to_bglist(pid,tokens[0],1);
      }
      select_operation_env(tokens);
      in = fd_p[0];
      close (fd_p[1]);
    }
  }
}

char** parse(char *command)
{
  int cnt=0;
  char delimiters[] = "; \t\b\n";
  char* token=strtok(command,delimiters);
  char** tokens =malloc(100*sizeof(char*));
  if (tokens==nul)
  {
    fprintf(stderr, "Memory Allocation Error" );
    exit(EXIT_FAILURE);
  }
  while(token!=nul)
  {
    tokens[cnt++]=token;
    token=strtok(nul,delimiters);
  }
  tokens[cnt]=nul;
  return tokens;
}
