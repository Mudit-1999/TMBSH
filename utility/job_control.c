#include "../header.h"

bg_process* head_process=nul;

int add_to_bglist(pid_t pid,char *p_name,int flag)
{
  bg_process* cur_process= (bg_process*)malloc(sizeof(bg_process));

  cur_process->nxt_process=head_process;
  cur_process->pid=pid;
  strcpy(cur_process->p_name,p_name);
  if(head_process!=nul)
  {
    cur_process->p_number=head_process->p_number + 1;
  }
  else
  {
    cur_process->p_number=1;
  }
  head_process=cur_process;
  if(flag)
  printf("[%d]\t%s\n",pid,p_name);
  return cur_process->p_number;
}

void add_to_fglist(pid_t pid ,char *p_name)
{
  cur_fg_process=(bg_process*)malloc(sizeof(bg_process));
  cur_fg_process->p_number=-1;
  cur_fg_process->nxt_process=nul;
  cur_fg_process->pid=pid;
  strcpy(cur_fg_process->p_name,p_name);
}

void print_jobs()
{
  bg_process*temp=head_process;
  if(test_mode)
  printf("printing jobs\n" );
  while(temp!=nul)
  {
    printf("[%d]\t",temp->p_number);
    char path[100];
    sprintf(path, "/proc/%d/wchan", temp->pid);
    FILE *f_open= fopen(path,"r");
    if(f_open != nul)
    {
      char status[100];
      fscanf(f_open, "%s", status);
      if(strcmp(status, "do_signal_stop") == 0)
      {
        printf("Stopped\t\t");
      }
      else
      {
        printf("Running\t\t");
      }
    }
    fclose(f_open);
    printf("%s [%d]\n",temp->p_name,temp->pid);
    temp=temp->nxt_process;
  }
}

bg_process * find_job_pid(pid_t wpid)
{
  bg_process *temp1=head_process,*prev=nul;
  while(temp1!=nul)
  {
    if(temp1->pid==wpid)
    {
      if(prev!=nul)
      {
        prev->nxt_process=temp1->nxt_process;
      }
      else
      {
        head_process=temp1->nxt_process;
      }
      break;
    }
    prev=temp1;
    temp1=temp1->nxt_process;
  }
  return temp1;
}


bg_process * find_job_p_num(int p_number)
{
  bg_process *temp1=head_process;
  while(temp1!=nul)
  {
    if(temp1->p_number==p_number)
    {
      break;
    }
    temp1=temp1->nxt_process;
  }
  return temp1;
}


void child_handler(int sig)
{
  int status;
  pid_t wpid = waitpid(-1, &status, WNOHANG| WUNTRACED);

  if (wpid > 0 && WIFEXITED(status) == 1)
  {
    fprintf(stderr, "\nProcess with pid: %d exited normally\n", wpid);
    bg_process* temp=find_job_pid(wpid);
    free(temp);
  }

  else if (wpid > 0 && WIFSTOPPED(status) == 1)
  {
    fprintf(stderr, "\nProcess with pid: %d stopped\n", wpid);
  }

  else if (wpid > 0 && WIFSIGNALED(status) == 1)
  {
    fprintf(stderr, "\nProcess with pid: %d interrupted by signal %d\n", wpid, sig);
    bg_process* temp=find_job_pid(wpid);
    free(temp);
  }
}


void kill_job(char **tokens)
{
  if(tokens[1]==nul || tokens[2]==nul || tokens[3]!=nul)
  {
    printf("Usage <jobNumber> <signalNumber>\n" );
    return ;
  }

  bg_process* temp=find_job_p_num(atoi(tokens[1]));

  if(test_mode)
    printf("killing jobs %d %d\n",atoi(tokens[1]),atoi(tokens[2]));

  if(temp==nul)
  {
    fprintf(stderr, "Error: No job with job number %d\n", atoi(tokens[1]));
    return;
  }
  if(kill( temp->pid,  atoi(tokens[2]) ) == -1)
  {
    perror("Kill Error");
    return;
  }
  return ;
}

void overkill()
{
  bg_process *temp=head_process;
  while(temp!=nul)
  {
    if(kill(temp->pid,SIGKILL) ==-1)
    {
      perror("Error");
      head_process=temp;
      return;
    }
    temp=temp->nxt_process;
  }
  head_process=temp;
  return ;
}

void handlesignal()
{
  signal(SIGCHLD,child_handler);
  signal(SIGINT, sigint_handler);
	signal(SIGTSTP, sigtstp_handler);
	// signal(SIGTTOU, SIG_IGN);
}

void sigint_handler(int sig)
{
	if(cur_fg_process!=nul)
  {
		kill(cur_fg_process->pid, SIGINT);
	}
}

void sigtstp_handler(int sig)
{
  if(cur_fg_process!=nul)
  {
		kill(cur_fg_process->pid, SIGTSTP);
	}
}


void run_bg_process(char **tokens)
{
  if(test_mode)
    printf("IN background\n");
  if(tokens[1]==nul ||  tokens[2]!=nul)
  {
    printf("Usage bg <jobnumber>\n" );
    return ;
  }
  bg_process* temp=find_job_p_num(atoi(tokens[1]));
  if(temp==nul)
  {
    fprintf(stderr, "Error: No job with job number %d\n", atoi(tokens[1]));
    return;
  }
  if(kill( temp->pid, SIGCONT) == -1)
  {
    perror("Kill Error");
    return;
  }
  return ;
}
void run_fg_process(char **tokens)
{
  int status;
  if(test_mode)
    printf("Bringing foreground\n");
  if(tokens[1]==nul ||  tokens[2]!=nul)
  {
    fprintf(stderr,"Usage fg <jobnumber>\n" );
    return ;
  }
  waitpid(-1, NULL, WNOHANG);
  bg_process* temp=find_job_p_num(atoi(tokens[1]));
  if(temp==nul)
  {
    fprintf(stderr, "Error: No job with job number %d\n", atoi(tokens[1]));
    return;
  }
  add_to_fglist(temp->pid,temp->p_name);
	int initial_in = tcgetpgrp(0), intial_out = tcgetpgrp(1),intial_err=tcgetpgrp(2);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	tcsetpgrp(0, temp->pid);
	tcsetpgrp(1, temp->pid);
	tcsetpgrp(2, temp->pid);
  printf("%s\n",temp->p_name);
  if(kill( temp->pid, SIGCONT) == -1)
  {
    perror("Kill Error");
    return;
  }
	if( waitpid(temp->pid, &status, WUNTRACED)==-1)
  {
    perror("Wait Error");
    return ;
  }
  if(WIFSIGNALED(status))
  {
    fprintf(stderr,"Process with pid:%d interrupted by a signal\n",temp->pid);
    temp=find_job_pid(temp->pid);
    free(temp);
  }
  else if(WIFSTOPPED(status))
  {
    fprintf(stderr,"[%d]\tStopped\t\t%s\n",temp->p_number,temp->p_name);
  }

  if(test_mode)
    printf(" test signal  %d %d\n",WIFSIGNALED(status), WIFSTOPPED(status));

	tcsetpgrp(0, initial_in);
	tcsetpgrp(1, intial_out);
	tcsetpgrp(2, intial_err);
	signal(SIGTTOU, SIG_DFL);
	signal(SIGTTIN, SIG_DFL);
	return ;
}
