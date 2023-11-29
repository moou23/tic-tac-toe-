#include <stdio.h>
#include <curses.h>
#include <stdlib.h>


void quit()
{
    endwin();
}
int main()
{
    int x,y;
initscr();
atexit(quit);
  curs_set(0);
  mvprintw(3, 5, "LINES: %d", LINES);
    mvaddstr(11, 2, "Taste drücken -> Ende");
getch();
  return(0);
}