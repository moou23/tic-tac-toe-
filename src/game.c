#include <curses.h>
#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
void drawBoard();
bool checkWin(char input[]);
void displayWinner(bool player);
void output(int ch, bool player);
void rahmen(char input[], int x, bool auswahl);
void quit();
void onevsone();
void menu();

void vorschau()
{
    initscr();
     keypad(stdscr, TRUE);
     mousemask(ALL_MOUSE_EVENTS, NULL);
mvprintw(0,30," _______ _          _______             _______         ");
mvprintw(1,30," |__   __(_)        |__   __|           |__   __|        ");
mvprintw(2,30,"    | |   _  ___ ______| | __ _  ___ ______| | ___   ___ ");
mvprintw(3,30,"    | |  | |/ __|______| |/ _` |/ __|______| |/ _ \\ / _ \\");
mvprintw(4,30,"    | |  | | (__       | | (_| | (__       | | (_) |  __/");
mvprintw(5,30,"    |_|  |_|\\___|      |_|\\__,_|\\___|      |_|\\___/ \\___|");
mvprintw(10,30,"Click LeftClick to get to the menu!");
while(true)
{  
  MEVENT event;
int ch = getch();
if (ch == KEY_MOUSE) {
      if (getmouse(&event) == OK)
      {
        if(event.bstate & BUTTON1_CLICKED)
        {
        menu();
        break;
        }
      }
}
else {
        mvprintw(11,30,"ONLY LEFTCLICK");
        }
}
}
void menu() {
  clear();
  bool menu;
  MEVENT event; // detects mouse events
  int ch;
  int switc = 0;
  char oneoone[] = "1 vs 1";
  char oneocomputer[] = "1 vs computer";
  char bestenliste[] = "Bestenliste";
  char exit[] = "Exit";
  bool auswahl = false;
  start_color();
  init_pair(2, COLOR_BLACK, COLOR_WHITE); //initializes a color pair
  mvprintw(0, 40, "MENU");
  mvprintw(2, 40, "%s", oneoone);
  rahmen(oneoone, 2, auswahl);
  mvprintw(4, 40, "%s", oneocomputer);
  rahmen(oneocomputer, 4, auswahl);
  mvprintw(6, 40, "%s", bestenliste);
  rahmen(bestenliste, 6, auswahl);
  mvprintw(8, 40, "%s", exit);
  rahmen(exit, 8, auswahl);
  noecho(); // deactivates the output of the input
  nodelay(stdscr, TRUE); // Make getch non-blocking
  mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL); // registers the mouse position
  printf("\033[?1003h\n"); // enables mouse movement detection

  while (true) {
    ch = getch();
    if (ch == KEY_MOUSE) {
      if (getmouse(&event) == OK) { // checks if the mouse interaction is registered 
        if (REPORT_MOUSE_POSITION) {
          if ((event.y == 2 || event.y == 1) &&
              (event.x >= 40 && event.y <= 45)) {
            rahmen(oneoone, 2, true);
            switc = 1;
            if (event.bstate & BUTTON1_DOUBLE_CLICKED) {
              nodelay(stdscr, false);
              mousemask(0, NULL);
              clear();
              onevsone();
            }
          } else {
            if (switc == 1) {
              rahmen(oneoone, 2, false);
              switc = 0;
            }
          }
          if ((event.y == 4 || event.y == 3) &&
              (event.x >= 40 && event.y <= 52)) {
            rahmen(oneocomputer, 4, true);
            switc = 2;
            if (event.bstate & BUTTON1_DOUBLE_CLICKED) {
              clear();
              mvprintw(4, 40, "Under construction");
            }
          } else {
            if (switc == 2) {
              rahmen(oneoone, 2, false);
              switc = 0;
            }
            rahmen(oneocomputer, 4, false);
          }
          if ((event.y == 6 || event.y == 5) &&
              (event.x >= 40 && event.y <= 50)) {
            switc = 3;
            rahmen(bestenliste, 6, true);
            if (event.bstate & BUTTON1_DOUBLE_CLICKED) {
              clear();
              mvprintw(4, 40, "Under construction");
            }
          } else {
            if (switc == 3) {
              rahmen(oneoone, 2, false);
              switc = 0;
            }
            rahmen(bestenliste, 6, false);
          }
          if ((event.y == 8 || event.y == 7) &&
              (event.x >= 40 && event.y <= 43)) {
            switc = 4;
            rahmen(exit, 8, true);
            if (event.bstate & BUTTON1_DOUBLE_CLICKED) {
              nodelay(stdscr, false);
              mousemask(0, NULL);
              clear();
              mvprintw(4, 40, "Thanks for playing!");
              quit();
            }

          } else {
            if (switc == 4) {
              rahmen(oneoone, 2, false);
              switc = 0;
            }
            rahmen(exit, 8, false);
          }
        }
      }
    }
  }
}

void rahmen(char input[], int x, bool auswahl) {
  int length = strlen(input);
  if (auswahl == true) {
    start_color();
    init_pair(1, COLOR_GREEN, PAIR_NUMBER(2));
    attron(COLOR_PAIR(1));
  }
  for (int i = 0; i < length; i++) {
    mvprintw(x - 1, i + 40, "_");
  }
  refresh();
  attroff(COLOR_PAIR(1));
}
// Function to perform cleanup and exit
void quit() { endwin(); }

// Function to check for a horizontal win
bool checkHorizontal(char input[]) {
  for (int row = 0; row < 3; row++) {
    int startIndex = row * 3;
    if (input[startIndex] != '\0' &&
        input[startIndex] == input[startIndex + 1] &&
        input[startIndex + 1] == input[startIndex + 2]) {
      return true;
    }
  }
  return false;
}

// Function to check for a diagonal win
bool checkDiagonal(char input[]) {
  if (input[0] != '\0' && input[0] == input[4] && input[4] == input[8]) {
    return true;
  }
  if (input[2] != '\0' && input[2] == input[4] && input[4] == input[6]) {
    return true;
  }
  return false;
}

// Function to check for a vertical win
bool checkVertical(char input[]) {
  for (int col = 0; col < 3; col++) {
    if (input[col] != '\0' && input[col] == input[col + 3] &&
        input[col + 3] == input[col + 6]) {
      return true;
    }
  }
  return false;
}

// Function to check for a win
bool checkWin(char input[]) {
  return checkHorizontal(input) || checkVertical(input) || checkDiagonal(input);
}

// Function to draw the game board
void drawBoard() {
  atexit(quit);
  curs_set(0);
  for (int j = 1; j < 3; j++) {
    for (int i = 0; i < 7; i++) {
      mvaddch(i, j * 8, '|');
      if (i % 2 == 0 && i != 0 && i != 6) {
        for (int k = 3; k < 22; k++) {
          if (k % 8 != 0) {
            mvaddch(i, k, '-');
          }
        }
      }
    }
  }
}

// Function to display the winner
void displayWinner(bool player) {
  mvprintw(10, 0, "The winner of the game is the player: %c",
           (player ? 'X' : 'O'));
}

// Function to update the game output
void output(int ch, bool player) {
  char out = (player) ? 'X' : 'O';
  int x = 49;
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
  mvaddch(1 + l, 5 + k * 7, out);
  refresh();
}

void onevsone() {
  // Initialize variables
  int x = 48;
  bool endGame = false;
  int xWinCount = 0;
  int oWinCount = 0;

  // Main game loop
  while (!endGame) {
    initscr();
    mvprintw(5, 50, "The point count is: X = %d Y = %d", xWinCount, oWinCount);

    // Initialize game state
    char input[9] = {'\0'};
    char currentPlayer = 'X';
    bool isPlayerX = true;
    int moves = 0;

    // Draw the game board
    drawBoard();

    // Play the game
    for (int i = 0; i < 9; i++) {
      mvprintw(10, 0, "It is the player with the %c turn", currentPlayer);
      int ch = getch();

      // Validate user input
      while (ch < 49 || ch > 57 || input[ch - x - 1] != '\0') {
        mvprintw(10, 0, "Invalid input. Enter a number between 1 and 9:");
        ch = getch();
        mvhline(10,0,' ',COLS); 
      }

      // Update the game state
      output(ch, isPlayerX);
      input[ch - x - 1] = currentPlayer;
      moves++;

      // Check for a win or draw
      if (checkWin(input) && moves >= 5) {
        displayWinner(isPlayerX);
        if (isPlayerX) {
          xWinCount++;
        } else {
          oWinCount++;
        }
        break;
      } else if (i == 8 && !checkWin(input)) {
        mvhline(10,0,' ',COLS); 
        mvprintw(10, 0, "It's a draw :(");
      }

      // Switch player
      currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
      isPlayerX = !isPlayerX;
      refresh();
    }

    // Ask user if they want to play again
    mvprintw(0, 10, "End? Press 1 for yes, 0 for another game:");
    if (getch() == 49) {
      menu();
    }
    clear();
  }
}
int main() { vorschau(); }
