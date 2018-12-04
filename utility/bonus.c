#include "../header.h"

void clock_build(char ** tokens)
{
  int gap ,dur;
  if(tokens[2]!=nul)
  {
     gap=atoi(tokens[2]);
  }
  else
  {
    printf("Usage Error:\n");
    return;

  }
  if(tokens[4]!=nul)
  {
     dur=atoi(tokens[4]);
  }
  else
  {
    printf("Usage Error:\n");
    return;
  }

  printf("%d   %d\n",gap,dur );

  while(dur > 0)
  {
    char clk[1000];
    strcpy(clk,"/sys/class/rtc/rtc0");
    char date[1000];
    strcpy(date,clk);
    strcat(date,"/date");
    FILE * fin=fopen(date,"r");
    char Da[20];
    fscanf(fin,"%s",Da);
    printf("%s ",Da );

    strcpy(date,clk);
    strcat(date,"/time");

    FILE * fin_tm=fopen(date,"r");
    char Ti[20];
    fscanf(fin_tm,"%s",Ti);
    printf("%s\n",Ti);
    dur-=gap;
    if(dur<=0)
    break;
    sleep(gap);
  }
}
