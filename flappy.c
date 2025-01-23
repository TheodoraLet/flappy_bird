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


static volatile sig_atomic_t sig_var;


void handler_fun()
{
    sig_var=1;
}

int main()
{
    char bird='@';
    int ch;
    int h=0;
    int w=0;
    int background=2;
    
	initscr();			/* Start curses mode 		  */
    noecho();
    keypad(stdscr,TRUE);
    cbreak();
    getmaxyx(stdscr,H,W);
    h=H/2;
    mvaddch(h,w,bird);
    landscape();
    curs_set(0);
    if(has_colors()==FALSE)
    {
        endwin();
        printf("terminal does not support colors\n");
        exit(1);
    }
    start_color();
    init_pair(1,COLOR_YELLOW,0);
    init_pair(background,COLOR_RED,COLOR_BLUE);
    bkgd(COLOR_PAIR(background));
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
                if((mvinch(h-1,w+1) & A_CHARTEXT)=='#')
                {
                    game_over();
                    //break;
                }
                attron(COLOR_PAIR(1));
                mvaddch(--h,++w,bird & A_CHARTEXT);
                attroff(COLOR_PAIR(1));
                refresh();
            }else if(ch==KEY_DOWN)
            {
                mvaddch(h,w,' ');
                if((mvinch(h+1,w+1)& A_CHARTEXT)=='#')
                {
                    game_over();
                    //break;
                }
                attron(COLOR_PAIR(1));
                mvaddch(++h,++w,bird & A_CHARTEXT);
                attroff(COLOR_PAIR(1));
                refresh();
            }
            //printf("out of khbit\n");
        }

        if(h>=H || h<=0)
        game_over();

        if(w>=W)
        {
            w=0;
            clear();
            landscape();
        }

        if(sig_var)
        {
            sig_var=0;
            mvaddch(h,w,' ');
            if((mvinch(h+1,w+1) & A_CHARTEXT)=='#')
            {
                game_over();
                //break;
            }
            attron(COLOR_PAIR(1));
            mvaddch(++h,++w,bird & A_CHARTEXT);
            attroff(COLOR_PAIR(1));
            refresh();
        }


    }

    endwin();
    return 0;
}
