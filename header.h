#ifndef func_dec
#define func_dec

#include<stdio.h>
#include<sys/utsname.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include <dirent.h>
#include<fcntl.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <inttypes.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>


#define nul NULL

void pwd();
void prompt();
void input_cmd();
char* separate(char* line);
char ** parse(char *command);
int execute(char** tokens,int cur_com,int tot_com);
void echo(char **tokens);
void cd(char **tokens);
void ls(char **tokens);
void pinfo(char **tokens);
void print_info(char info[100][100],char link_exe[]);
void initialize_flg();
int read_flags(char **tokens,int curr);
int reading_path(char ** path_dir,char ** tokens,int cnt);
void read_dir(char **path_dir);
int is_dir(char *dir_name);
void print_filetype(mode_t mode);
void print_permissions(mode_t mode);
void print_timestamp(time_t mod_time);
void child_handler (int sig) ;
void clock_build(char ** tokens);
void pipe_implement(int cur_com,int tot_com,int in,int out);
char ** check_pipe(char* command_with_pipe,int *cnt);
void fun_setenv(char ** tokens );
void fun_unsetenv(char ** tokens );
void fun_getenv(char ** tokens );
bool bg_check(char** tokens);
void select_operation_env(char **tokens);
void print_jobs();
int add_to_bglist(pid_t pid,char *p_name ,int flag);
void handlesignal();
void kill_job(char **tokens);
void overkill();
void sigint_handler(int sig);
void sigtstp_handler(int sig);
void add_to_fglist(pid_t pid ,char *p_name);
void run_bg_process(char **tokens);
void run_fg_process(char **tokens);


typedef struct bg_process
{
  pid_t pid;
  int p_number;
  char p_name[1000];
  struct bg_process* nxt_process;
}bg_process;

bg_process * find_job_pid(pid_t wpid);
bg_process * find_job_p_num(int p_number);
bg_process* cur_fg_process;

// void launch_process (char** tokens,int cur_com,int tot_com,int in ,int out);
// void launch_process (char** tokens);
// int execute(char** tokens,int cur_com,int tot_com,int in ,int out);
// taking care of home executable file location is home
/// taking care of qoutes ,space in between commands
char home_dir[1000];
int initial_std_in,initial_std_out;
int test_mode;


#endif
