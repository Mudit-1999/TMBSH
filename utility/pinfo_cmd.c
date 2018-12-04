#include "../header.h"

void pinfo(char ** tokens)
{

  char path[100]="/proc/";
  char path_exe[100]="/proc/";
  if (tokens[1]==nul)
  {
    char res[100];
    int cur_pid=getpid();
    sprintf(res, "%d",cur_pid);
    strcat(path,res);
    strcat(path_exe,res);

  }
  else
  {
    strcat(path,tokens[1]);
    strcat(path_exe,tokens[1]);
  }
  strcat(path,"/stat");
  strcat(path_exe,"/exe");
  FILE *f_open= fopen(path,"r");
  if (f_open== nul)
  {
    fprintf(stderr, "No process with PID %s\n",tokens[1]);
    fprintf(stderr,"Usage : pinfo <pid>\n");
		return ;
  }
  char link_exe[100];
  if (readlink(path_exe,link_exe, 100) == -1)
  {
		perror("Error:");
		return ;
	}
  char info[100][100];
	for (int i = 0; i < 100; i++)
		fscanf(f_open, "%s", info[i]);
  print_info(info,link_exe);
  fclose(f_open);
}

void print_info(char info[100][100],char link_exe[])
{
    printf("PID:%s\nProcess Status:%s\nMemory:%s\nExecutable:%s\n",info[0], info[2],info[22], link_exe);
}
