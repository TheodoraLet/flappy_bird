#ifndef FUNCTIONS_H
#define FUNCTIONS_H

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


extern int H,W;

int kbhit(void);

void landscape();

int rand_number(int max,int min);

void game_over();


#endif 

