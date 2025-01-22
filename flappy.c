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


#define KEY_ESC 27

clock_t t_m;
clock_t difference=0;
int kbhit(void);

static volatile sig_atomic_t sig_var;
void handler_fun();

int main()
{

    char bird='@';
    int ch;
    int H,W;
    int h=0;
    int w=0;
    
	initscr();			/* Start curses mode 		  */
    noecho();
    keypad(stdscr,TRUE);
    cbreak();
    getmaxyx(stdscr,H,W);
    h=H/2;
    mvaddch(h,w,bird);
    refresh();


    struct itimerval value;
    value.it_interval.tv_sec=1;
    value.it_interval.tv_usec=0;
    value.it_value.tv_sec=2;
    value.it_value.tv_usec=0;
    setitimer(ITIMER_REAL,&value,0);

    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    act.sa_handler=(void*)handler_fun;
    sigaction(SIGALRM,&act,NULL);


    while(1)
    {
        if(kbhit())
        {
            ch=getch();

            if(ch==KEY_ESC)
            break;

            if(ch==KEY_UP)
            {
                mvaddch(h,w,' ');
                mvaddch(--h,++w,bird);
                refresh();
            }else if(ch==KEY_DOWN)
            {
                mvaddch(h,w,' ');
                mvaddch(++h,++w,bird);
                refresh();
            }
            //printf("out of khbit\n");
        }

        if(h>=H || h<=0 || w>=W)
        break;

        if(sig_var)
        {
            sig_var=0;
            mvaddch(h,w,' ');
            mvaddch(++h,++w,bird);
            refresh();

        }


    }

    endwin();
    return 0;
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