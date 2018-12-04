#include "../header.h"

void pwd()
{
  char curr_dir[1000];
  getcwd(curr_dir,sizeof(curr_dir));
  printf("%s\n",curr_dir);
}

void echo(char **tokens)
{
  for (int i = 1; tokens[i]!=nul ; i++)
  printf("%s ",tokens[i]);
  printf("\n");
}

void cd(char ** tokens)
{
  char destiny[1000];
  if ( tokens[1] == nul  || strcmp(tokens[1],"~\0")==0 )
    strcpy(destiny,home_dir);
  else if( tokens[1][0]=='~')
  {
    char temp[100];
    strncpy(temp,tokens[1] +1,strlen(tokens[1]) -1);
    strcpy(destiny,home_dir);
    strcat(destiny,temp);
  }
  else
    strcpy(destiny,tokens[1]);

  int ch = chdir(destiny);
  if (ch==-1)
    perror("Error");
}
struct flags
{
  bool using_a;
  bool using_l;
} flgs;

char curr_path[100];

void ls (char **tokens)
{
  getcwd(curr_path,sizeof(curr_path));
  char a[]="/\0";
  strcat(curr_path,a);
  initialize_flg();
  char ** path_dir= malloc(100*sizeof(char*));
  if (reading_path(path_dir,tokens,1) == 0 )
  return;
  read_dir(path_dir);
}

void read_dir(char ** path_dir)
{
  for (int i = 0; path_dir[i]!=nul; i++)
  {
    // check whether the given filename is dir or file
    int test =is_dir(path_dir[i]);
    if(test==1)
    {
      printf("%s:\n",path_dir[i]);
      struct dirent *de;
      DIR *dr_open = opendir(path_dir[i]);
      if (dr_open == NULL)
      {
        perror("Error");
        continue;
      }

      char current_dir[1000];
      strcpy(current_dir,curr_path);
      strcat(current_dir,path_dir[i]);
      while ((de = readdir(dr_open)) != NULL)
      {
        if(de->d_name[0] != '.' || flgs.using_a)
        {
          if(flgs.using_l)
          {
            // printf("%s\n",current_dir);
            struct stat sb =  {0};
            char path_buffer[1000];
            int bytes_written =snprintf(path_buffer, sizeof(path_buffer),
            "%s/%s", current_dir, de->d_name);
            if (bytes_written <= 0)
            {
              fprintf(stderr, "Error\n");
              continue;
            }
          stat(path_buffer, &sb);
          print_filetype(sb.st_mode);
          print_permissions(sb.st_mode);
          printf("%jd ", (intmax_t)sb.st_nlink);
          printf("%5s ", getpwuid(sb.st_uid)->pw_name);
          printf("%5s ", getgrgid(sb.st_gid)->gr_name);
          printf("%7jd ", (intmax_t)sb.st_size);
          print_timestamp(sb.st_mtime);
          //  free(current_dir);
        }
        printf("%s\n", de->d_name);
      }

    }
    closedir(dr_open);
  }
  else if(test==0)
  {
    printf("%s IS a file\n",path_dir[i]);
  }
  else
  {
    continue;
  }
}
}

void print_timestamp(time_t mod_time)
{
  time_t curr_time;
  time(&curr_time);
  struct tm*t = localtime(&curr_time);
  int curr_mon = t->tm_mon;
  int curr_yr = 1970 + t->tm_year;

  t = localtime(&mod_time);
  int mod_mon = t->tm_mon;
  int mod_yr = 1970 + t->tm_year;

  char* format = (mod_yr == curr_yr) && (mod_mon >= (curr_mon - 6))? "%b %e %H:%M" : "%b %e  %Y";
  char time_buf[1000];
  const size_t bytes_written =strftime(time_buf, sizeof(time_buf), format, t);
  printf("%s ",time_buf);
}

void print_permissions(mode_t mode)
{
  putchar((mode & S_IRUSR) ? 'r' : '-');
  putchar((mode & S_IWUSR) ? 'w' : '-');
  putchar((mode & S_IXUSR) ? 'x' : '-');
  putchar((mode & S_IRGRP) ? 'r' : '-');
  putchar((mode & S_IWGRP) ? 'w' : '-');
  putchar((mode & S_IXGRP) ? 'x' : '-');
  putchar((mode & S_IROTH) ? 'r' : '-');
  putchar((mode & S_IWOTH) ? 'w' : '-');
  putchar((mode & S_IXOTH) ? 'x' : '-');
  putchar(' ');
}
void print_filetype(mode_t mode)
{
  switch (mode & S_IFMT)
  {
    case S_IFREG: putchar('-'); break;
    case S_IFDIR: putchar('d'); break;
    case S_IFLNK: putchar('l'); break;
    case S_IFCHR: putchar('c'); break;
    case S_IFBLK: putchar('b'); break;
    case S_IFSOCK: putchar('s'); break;
    case S_IFIFO: putchar('f'); break;
  }
}

int reading_path(char ** path_dir,char ** tokens,int cnt)
{
  // printf("dfsffffdfs");
  int i=0;
  while(tokens[cnt]!=nul && tokens[cnt][0]!='-')
  {
    path_dir[i++]=tokens[cnt++];
  }
  cnt=read_flags(tokens,cnt);
  if (cnt==0)
  {
    return 0;
  }
  while(tokens[cnt]!=nul && tokens[cnt][0]!='-')
  {
    path_dir[i++]=tokens[cnt++];
  }

  if (i==0)
  {
    char a[] = ".\0";
    path_dir[i++]=a;
  }
  path_dir[i]=nul;
  return i;
}

void initialize_flg()
{
  flgs.using_a=false;
  flgs.using_l=false;
}

int read_flags(char **tokens,int curr)
{
  while(tokens[curr] != nul && tokens[curr][0]=='-')
  {
    if (tokens[curr][1]=='\0')
    {
      fprintf(stderr,"Usage Error\n");
      return 0;
    }
    for (size_t i = 1; tokens[curr][i]!='\0'; i++)
    {
      switch(tokens[curr][i])
      {
        case 'a':
        flgs.using_a = true; break;
        case 'l':
        flgs.using_l = true; break;
        default:
        fprintf(stderr,"Usage :ls,ls -a,ls -l ,ls -la(Only l and a flags are allowed)\n");
        return 0;
      }
    }
    curr++;
  }
  return curr;
}

int is_dir(char *dir_name)
{
  struct stat sb = {0};
  if (stat(dir_name, &sb) < 0)
  {
    perror(dir_name);
    return -1;
  }
  return S_ISDIR(sb.st_mode);
}
