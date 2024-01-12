#include "../include/game.h"
#include <curses.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Start function
void tic_tac_toe() {
  // Initialize the curses library
  initscr();
  keypad(stdscr, TRUE);
  mousemask(ALL_MOUSE_EVENTS, NULL);

  // ASCII Art for the game name
  mvprintw(0, 30, " _______ _          _______             _______         ");
  mvprintw(1, 30, " |__   __(_)        |__   __|           |__   __|        ");
  mvprintw(2, 30, "    | |   _  ___ ______| | __ _  ___ ______| | ___   ___ ");
  mvprintw(3, 30,
           "    | |  | |/ __|______| |/ _` |/ __|______| |/ _ \\ / _ \\");
  mvprintw(4, 30, "    | |  | | (__       | | (_| | (__       | | (_) |  __/");
  mvprintw(5, 30,
           "    |_|  |_|\\___|      |_|\\__,_|\\___|      |_|\\___/ \\___|");

  mvprintw(10, 30, "Click LeftClick to get to the menu!");

  while (true) {
    MEVENT event;
    int ch = getch();
    if (ch == KEY_MOUSE) {
      if (getmouse(&event) == OK) {
        if (event.bstate & BUTTON1_CLICKED) {
          menu();
          break;
        }
      }
    } else {
      mvprintw(11, 30, "ONLY LEFTCLICK");
    }
  }
}

// Menu function
void menu() {
  clear();
  MEVENT event;
  int ch;
  char oneoone[] = "1 vs 1";
  char oneocomputer[] = "1 vs computer";
  char bestenliste[] = "Tipps & Tricks";
  char exit[] = "Exit";
  bool selection = false;

  start_color();
  init_pair(2, COLOR_BLACK, COLOR_WHITE); // Initialize a color pair

  mvprintw(0, 40, "MENU");

  mvprintw(2, 40, "%s", oneoone);
  frame(oneoone, 2, selection);

  mvprintw(4, 40, "%s", oneocomputer);
  frame(oneocomputer, 4, selection);

  mvprintw(6, 40, "%s", bestenliste);
  frame(bestenliste, 6, selection);

  mvprintw(8, 40, "%s", exit);
  frame(exit, 8, selection);

  noecho();              // Disable input echo
  nodelay(stdscr, TRUE); // Make getch non-blocking

  mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION,
            NULL); // Register mouse events and report mouse position

  printf("\033[?1003h\n"); // Enable mouse motion reporting

  while (true) {
    ch = getch();
    if (ch == KEY_MOUSE) {
      if (getmouse(&event) == OK) {
        if (REPORT_MOUSE_POSITION) {
          if ((event.y == 2 || event.y == 1) &&
              (event.x >= 40 && event.x <= 45)) {
            frame(oneoone, 2, true);
            if (event.bstate & BUTTON1_DOUBLE_CLICKED) {
              disable();
              one_vs_one();
            }
          } else {
            frame(oneoone, 2, false);
          }

          if ((event.y == 4 || event.y == 3) &&
              (event.x >= 40 && event.x <= 52)) {
            frame(oneocomputer, 4, true);
            if (event.bstate & BUTTON1_DOUBLE_CLICKED) {
              disable();
              one_vs_computer();
            }
          } else {
            frame(oneocomputer, 4, false);
          }

          if ((event.y == 6 || event.y == 5) &&
              (event.x >= 40 && event.x <= 50)) {
            frame(bestenliste, 6, true);
            if (event.bstate & BUTTON1_DOUBLE_CLICKED) {
              disable();
              tipps_n_tricks();
            }
          } else {
            frame(bestenliste, 6, false);
          }

          if ((event.y == 8 || event.y == 7) &&
              (event.x >= 40 && event.x <= 43)) {
            frame(exit, 8, true);
            if (event.bstate & BUTTON1_DOUBLE_CLICKED) {
              disable();
              mvprintw(4, 40, "Thanks for playing!");
              quit();
            }
          } else {
            frame(exit, 8, false);
          }
        }
      }
    }
  }
}

// Function to disable features not needed for the game
void disable() {
  nodelay(stdscr, false);
  mousemask(0, NULL);
  clear();
}

// Function for tipps and tricks
void tipps_n_tricks() {
  // Tipps für Tic-Tac-Toe anzeigen
  mvprintw(1, 1, "Coole Tic-Tac-Toe Tipps");
  mvprintw(3, 1,
           "1. Beginne in der Mitte: Das gibt dir die meisten Möglichkeiten.");
  mvprintw(4, 1,
           "2. Ecke als nächstes: Wenn die Mitte belegt ist, wähle eine Ecke.");
  mvprintw(
      5, 1,
      "3. Blockiere Gegner: Halte Ausschau und blockiere gegnerische Linien.");
  mvprintw(6, 1,
           "4. Zwei Wege zum Sieg: Versuche, zwei Gewinnmöglichkeiten "
           "gleichzeitig zu haben.");
  mvprintw(7, 1,
           "5. Achte auf Fallen: Pass auf, dass du deinem Gegner nicht den "
           "Sieg ermöglichst.");

  // Warten auf eine Eingabe
  getch();
  menu();
}

// Function to determine the larger of two numbers
int get_larger(int num1, int num2) { return (num1 > num2) ? num1 : num2; }

// Function to determine the smaller of two numbers
int get_smaller(int num1, int num2) { return (num1 < num2) ? num1 : num2; }

// Alpha-Beta Pruning algorithm for the Minimax algorithm
int alphabeta(char input[], bool Maximizing, int alpha, int beta) {
  // Check for win or draw
  if (!check_win(input)) {
    bool isFull = true;
    for (int x = 0; x < 9; x++) {
      if (input[x] == '\0') {
        isFull = false;
        break;
      }
    }
    if (isFull) {
      return 0; // Draw
    }
  } else {
    if (check_win(input) && Maximizing) {
      return -1; // Loss for the player
    } else {
      if (check_win(input) && !Maximizing) {
        return 1; // Win for the player
      }
    }
  }

  // Minimax algorithm with Alpha-Beta Pruning
  if (Maximizing) {
    int maxEval = INT_MIN;
    for (int j = 0; j < 9; j++) {
      if (input[j] == '\0') {
        input[j] = 'X';
        int eval = alphabeta(input, false, alpha, beta);
        maxEval = get_larger(eval, maxEval);
        input[j] = '\0';
        alpha = get_larger(alpha, eval);
        if (beta <= alpha) {
          break;
        }
      }
    }
    return maxEval;

  } else {
    int minEval = INT_MAX;
    for (int k = 0; k < 9; k++) {
      if (input[k] == '\0') {
        input[k] = 'C';
        int eval = alphabeta(input, true, alpha, beta);
        minEval = get_smaller(eval, minEval);
        input[k] = '\0';
        beta = get_smaller(beta, eval);
        if (beta <= alpha) {
          break;
        }
      }
    }
    return minEval;
  }
}

// Function to determine the best move for the computer using Alpha-Beta Pruning
int bestmove(char input[]) {
  int BestMove = 0;
  int bestScore = INT_MAX;

  // Iterate through all possible moves
  for (int i = 0; i < 9; i++) {
    if (input[i] == '\0') {
      input[i] = 'C';

      // Evaluate the move using Alpha-Beta Pruning
      int score = alphabeta(input, true, INT_MIN, INT_MAX);
      mvprintw(10 + i, 50, "%d", score);

      input[i] = '\0';

      // Update the best move if the current move is better rated
      if (score < bestScore) {
        bestScore = score;
        BestMove = i;
      }
    }
  }
  return BestMove + 49; // Return the best move (as character)
}

// Function for the game "Player vs Computer"
void one_vs_computer() {
  mvprintw(10, 10, "The term 'C' stands for Computer");
  int x = 48;
  int xWinCount = 0;

  // Main game loop
  while (true) {
    initscr();
    mvprintw(5, 50, "You won %d times against the computer!", xWinCount);

    // Initialize game state
    char input[9] = {'\0'};
    char currentPlayer = 'X';
    bool isPlayerX = true;
    int moves = 0;

    // Draw the game board
    draw_board();

    // Play the game
    for (int i = 0; i < 9; i++) {
      mvprintw(10, 0, "It is the player with the %c turn", currentPlayer);

      // Display the current game state
      for (int i = 0; i < 9; i++) {
        mvprintw(13 + i, i + 2, "%d ", input[i]);
      }

      if (isPlayerX == true) {
        int ch = getch();

        // Validate user input
        while (ch < 49 || ch > 57 || input[ch - x - 1] != '\0') {
          mvprintw(10, 0, "Invalid input. Enter a number between 1 and 9:");
          ch = getch();
          mvhline(10, 0, ' ', COLS);
        }

        // Update the game state for the player
        output(ch, isPlayerX, 'X');
        input[ch - x - 1] = currentPlayer;
        moves++;

      } else {
        int ch = bestmove(input);

        // Output computer move
        output(ch, isPlayerX, 'C');
        input[ch - x - 1] = currentPlayer;
        moves++;
      }

      // Check for a win or draw
      if (check_win(input) && moves >= 5) {
        display_winner(isPlayerX);

        if (isPlayerX) {
          xWinCount++;
          break;
        } else if (i == 8 && !check_win(input)) {
          mvhline(10, 0, ' ', COLS);
          mvprintw(10, 0, "It's a draw :(");
        } else {
          break;
        }
      }

      // Switch player
      currentPlayer = (currentPlayer == 'X') ? 'C' : 'X';
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

// Function to draw a frame using ncurses
void frame(char input[], int x, bool selection) {
  int length = strlen(input);

  // If selection is true, set the color to green
  if (selection == true) {
    start_color();
    init_pair(1, COLOR_GREEN, PAIR_NUMBER(2));
    attron(COLOR_PAIR(1));
  }

  // Draw the frame
  for (int i = 0; i < length; i++) {
    mvprintw(x - 1, i + 40, "_");
  }
  refresh();
  attroff(COLOR_PAIR(1));
}

// Function for cleanup and program exit
void quit() { endwin(); }

// Function to check for a horizontal win
bool check_horizontal(char input[]) {
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
bool check_diagonal(char input[]) {
  if (input[0] != '\0' && input[0] == input[4] && input[4] == input[8]) {
    return true;
  }
  if (input[2] != '\0' && input[2] == input[4] && input[4] == input[6]) {
    return true;
  }
  return false;
}

// Function to check for a vertical win
bool check_vertical(char input[]) {
  for (int col = 0; col < 3; col++) {
    if (input[col] != '\0' && input[col] == input[col + 3] &&
        input[col + 3] == input[col + 6]) {
      return true;
    }
  }
  return false;
}

// Function to check for a win
bool check_win(char input[]) {
  return check_horizontal(input) || check_vertical(input) ||
         check_diagonal(input);
}

// Function to draw the game board
void draw_board() {
  atexit(quit);
  curs_set(0);

  // Draw the game board frame
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

  // Label the game board squares with numbers
  int number = 1;
  for (int row = 1; row <= 5; row += 2) {
    for (int col = 3; col <= 17; col += 7) {
      mvprintw(row, col, "%d", number++);
    }
  }
}

// Function to display the winner
void display_winner(bool player) {
  mvprintw(10, 0, "The winner of the game is the player: %c",
           (player ? 'X' : 'O'));
}

// Function to update the game output
void output(int ch, bool player, char symbol) {
  char out = (player) ? 'X' : symbol;
  start_color();
  init_pair(3, COLOR_RED, COLOR_BLACK);
  attron(COLOR_PAIR(3));
  int x = 49;
  int k = 0;
  int l = 0;
  k = (ch - x);

  // Calculate position based on user input
  if (k >= 0 && k < 3) {
    l = 0;
  } else if (k >= 3 && k < 6) {
    l = 2;
  } else if (k >= 6) {
    l = 4;
  }
  k = k % 3;

  // Draw the symbol at the calculated position
  mvaddch(1 + l, 5 + k * 7, out);
  refresh();
  attroff(COLOR_PAIR(3));
}

// Function for the "Player vs Player" game
void one_vs_one() {
  // Initialize variables
  int x = 48;
  int xWinCount = 0;
  int oWinCount = 0;

  // Main game loop
  while (true) {
    // Initialize the screen for ncurses
    initscr();

    // Display the point count
    mvprintw(5, 50, "The point count is: X = %d O = %d", xWinCount, oWinCount);

    // Initialize the game state
    char input[9] = {'\0'};
    char currentPlayer = 'X';
    bool isPlayerX = true;
    int moves = 0;

    // Draw the game board
    draw_board();

    // Play the game
    for (int i = 0; i < 9; i++) {
      // Display which player's turn it is
      mvprintw(10, 0, "It is the player with the %c turn", currentPlayer);

      // Wait for user input
      int ch = getch();

      // Validate user input
      while (ch < 49 || ch > 57 || input[ch - x - 1] != '\0') {
        mvprintw(10, 0, "Invalid input. Enter a number between 1 and 9:");
        ch = getch();
        mvhline(10, 0, ' ', COLS);
      }

      // Update the game state
      output(ch, isPlayerX, 'O');
      input[ch - x - 1] = currentPlayer;
      moves++;

      // Check for a win or draw
      if (check_win(input) && moves >= 5) {
        // Display the winner and update the point count
        display_winner(isPlayerX);
        if (isPlayerX) {
          xWinCount++;
        } else {
          oWinCount++;
        }
        frage_bestenliste_speichern(currentPlayer, xWinCount, oWinCount);
        break;
      } else if (i == 8 && !check_win(input)) {
        // In case of a draw
        mvhline(10, 0, ' ', COLS);
        mvprintw(10, 0, "It's a draw :(");
      }

      // Switch player
      currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
      isPlayerX = !isPlayerX;
      refresh();
    }

    // Ask the user if they want to play again
    mvprintw(0, 10, "End? Press 1 for yes, 0 for another game:");
    if (getch() == 49) {
      menu();
    }
    clear();
  }
}

void speichern_bestenliste(char spieler, int gewonneneSpiele) {
  FILE *datei = fopen("bestenliste.txt", "a");
  time_t jetzt;
  struct tm *lokaleZeit;

  // Aktuelles Datum ermitteln
  time(&jetzt);
  lokaleZeit = localtime(&jetzt);

  if (datei != NULL) {
    fprintf(datei, "Spieler %c hat %d Spiele gewonnen - Datum: %02d-%02d-%d\n",
            spieler, gewonneneSpiele, lokaleZeit->tm_mday,
            lokaleZeit->tm_mon + 1, lokaleZeit->tm_year + 1900);
    fclose(datei);
  } else {
    mvprintw(22, 40, "Fehler beim Speichern in der Bestenliste!");
  }
}

void frage_bestenliste_speichern(char gewinner, int xWinCount, int oWinCount) {
    char antwort;
    mvprintw(20, 40, "Möchte Spieler %c seinen Sieg speichern? (j/n): ", gewinner);
    echo();
    antwort = getch();
    noecho();

    if (antwort == 'j' || antwort == 'J') {
        if (gewinner == 'X') {
            speichern_bestenliste('X', xWinCount);
        } else if (gewinner == 'O') {
            speichern_bestenliste('O', oWinCount);
        }
    }
}