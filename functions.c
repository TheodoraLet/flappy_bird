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
static volatile sig_atomic_t sig_var;

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

void handler_fun()
{
    sig_var=1;
}

void landscape()
{
    int pace=W/4;
    for(int i=pace;i<=W;i=i+pace)
    {
        int g=rand_number(3*H/4,0);
        int s=rand_number(W/7,W/10);
        mvvline(0,i,'#',g);
        mvhline(g,i,'#',s);
        mvvline(0,i+s,'#',g);

        int g_new=H/4 +g;
        mvvline(g_new,i,'#',H);
        mvhline(g_new,i,'#',s);
        mvvline(g_new,i+s,'#',H);
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