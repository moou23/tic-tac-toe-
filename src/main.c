#include <curses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void quit();
bool waagrecht(char input[])
{
    for (int row = 0; row < 3; row++) {
        int startIndex = row * 3; 

        if (input[startIndex] != '\0') {
            if (input[startIndex] == input[startIndex + 1] && input[startIndex + 1] == input[startIndex + 2]) {
                return true;
            }
        }
    }

    return false;
}
bool diagonal(char input[]) { 
    if (input[0] != '\0' && input[0] == input[4] && input[4] == input[8]) {
        return true;
    }
    if (input[2] != '\0' && input[2] == input[4] && input[4] == input[6]) {
        return true;
    }
    return false;
}


bool hoch_runter(char input[])
{

    for (int col = 0; col < 3; col++) {
        if (input[col] != '\0') {
            if (input[col] == input[col + 3] && input[col + 3] == input[col + 6]) {
                return true;
            }
        }
    }

    return false;
}
bool winning(char input[]) {
if(diagonal(input)== true || waagrecht(input) == true || hoch_runter(input) == true)
{
  return true;
}
return false;
}
void layout() {
  atexit(quit);
  curs_set(0);
  for (int j = 1; j < 3; j++) {
    for (int i = 0; i < 7; i++) {
      mvaddstr(i, j * 8, "|");
      if (i % 2 == 0 && i != 0 && i != 6) {
        for (int k = 3; k < 22; k++) {
          if (k % 8 != 0) {
            mvaddstr(i, k, "-");
          }
        }
      }
    }
  }
}
void quit() { endwin(); }
void output(int ch, bool player) {
  char out[1];
  if (player == true) {
    out[0] = 'X';
  } else {
    out[0] = 'O';
  }
  int x = 49;
  int i = 0;
  int k = 0;
  int l = 0;
  k = (ch - x);
  if (k >= 0 && k < 3) {
    l = 0;
  } else if (k >= 3 && k < 6) {
    l = 2;
  } else if (k >= 6) {
    l = 4;
  }
  k = k % 3;
  mvaddstr(1 + l, 5 + k * 7, out);
  refresh();
}
int main() {
  int x = 48;
  bool ende = false;
  int xwin = 0;
  int ywin = 0;
    while (ende == false) {
        initscr();
      mvaddstr(5, 50, "Highscore: X = ");
      mvprintw(5, 66,"%d ", xwin);
      mvaddstr(5, 68, "Y = ");
      mvprintw(5, 73, "%d", ywin);
    char input[9] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
  char play = 'X';
  bool player = true;
  int moves = 0;
  layout();
  for (int i = 0; i < 9; i++) {
    if (player == true) {
      mvaddstr(10, 0, "It is the player with the X turn");
    } else {
      mvaddstr(10, 0, "It is the player with the O turn");
    }
    int ch = getch();
    while (ch < 49 || ch > 57) {
      mvaddstr(10, 0, "Only numbers between 1 and 9!");
      ch = getch();
    }
    while(input[ch-x-1] != '\0')
    {
    mvaddstr(10, 0, "Pick a new filed ");
    ch = getch();
    }
     output(ch, player);
    input[ch - x-1] = play;
    moves++;
    if (winning(input) == true && moves >= 5) {
    mvaddstr(10, 0, "The winner of the whole game is the player:");
      if (player == true) {
        xwin++;
        mvaddstr(10, 44,"X");
      } else {
        ywin++;
        mvaddstr(10, 44,"O");
      }
      break;
    } else if (i == 8 && winning(input) == false) {
      mvaddstr(10, 0, "A draw :(");
    }
    if (player == true) {
      play = 'O';
      player = false;
    } else {
      play = 'X';
      player = true;
    }
    refresh();
  }
    mvaddstr(0, 10, "ende? 1 für ja, 0 für nochmal");
    if (getch() == 49) {
      ende = true;
    }
    clear();
  }
  quit();
  return (0);
}
