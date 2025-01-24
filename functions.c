#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <signal.h>
#include <sys/time.h>
#include "functions.h"

int H,W;

int rand_number(int max,int min)
{
  return rand() %(max+1 -min) +min;
}


int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}

void add_colors()
{
  if(has_colors()==FALSE)
  {
    endwin();
    printf("terminal does not support colors\n");
    exit(1);
  }
  start_color();
  init_pair(1,COLOR_YELLOW,0);
  init_pair(2,COLOR_RED,COLOR_BLUE);
  init_pair(3,COLOR_YELLOW,COLOR_MAGENTA);
  bkgd(COLOR_PAIR(2));
}

void landscape()
{
  int pace=W/4;
  for(int i=pace;i<=W;i=i+pace)
  {
    int g=rand_number(3*H/4,0);
    int s=rand_number(W/8,W/10);
    mvvline(0,i,'#',g);
    mvhline(g,i,'#',s);
    mvvline(0,i+s,'#',g);

    int g_new=H/4 +g;
    mvvline(g_new,i,'#',H);
    mvhline(g_new,i,'#',s);
    mvvline(g_new,i+s,'#',H);
    
    attron(COLOR_PAIR(3));
    mvaddch((H/8)+g,i+(s/2),'0');
    attroff(COLOR_PAIR(3));
  }
  refresh();
}


void game_over()
{
  erase();
  mvvline(0,0,'#',H);
  mvvline(0,W-1,'#',H);
  mvhline(0,0,'#',W);
  mvhline(H-1,0,'#',W);
  mvprintw(H/2,W/2,"GAME OVER");
  refresh();
}


void lives_init()
{
  mvprintw(0,0,"<3");
  mvprintw(0,2,"<3");
  mvprintw(0,4,"<3");
}

void lives_count(int lives_index)
{
  if(lives_index==0)
  {
    game_over();
  }else if(lives_index==2)
  {
    mvprintw(0,4,"  ");
  }else if(lives_index==1)
  {
    mvprintw(0,2,"  ");
    mvprintw(0,4,"  ");
  }
}

void count_points(int points)
{
  mvprintw(1,0,"points: %d",points);
}


void move_up(int* h,int* w,int* lives_index,int* points,char bird)
{
  mvaddch(*h,*w,' ');
  if((mvinch(*h-1,*w+1) & A_CHARTEXT)=='#')
  {
      lives_count(--(*lives_index));
  }else if((mvinch(*h-1,*w+1) & A_CHARTEXT)=='0')
  {
      count_points(++(*points));
  }
  attron(COLOR_PAIR(1));
  mvaddch(--(*h),++(*w),bird & A_CHARTEXT);
  attroff(COLOR_PAIR(1));
  refresh();
}

void move_down(int* h,int* w,int* lives_index,int* points,char bird)
{
  mvaddch(*h,*w,' ');
  if((mvinch(*h+1,*w+1)& A_CHARTEXT)=='#')
  {
      lives_count(--(*lives_index));
  }else if((mvinch(*h+1,*w+1) & A_CHARTEXT)=='0')
  {
      count_points(++(*points));
  }
  attron(COLOR_PAIR(1));
  mvaddch(++(*h),++(*w),bird & A_CHARTEXT);
  attroff(COLOR_PAIR(1));
  refresh();
}