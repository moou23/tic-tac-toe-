#include <curses.h>
#include <stdbool.h>
#include <stdlib.h>

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
  mvprintw(10, 44, "%c", (player ? 'X' : 'O'));
}

// Function to update the game output
void output(int ch, bool player) {
  char out = (player) ? 'X' : 'O';
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
  mvaddch(1 + l, 5 + k * 7, out);
  refresh();
}

void ticTacToe() {
  // Initialize variables
  int x = 48;
  bool endGame = false;
  int xWinCount = 0;
  int oWinCount = 0;

  // Main game loop
  while (!endGame) {
    initscr();
    mvprintw(5, 50, "Highscore: X = %d Y = %d", xWinCount, oWinCount);

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
      endGame = true;
    }

    clear();
  }

  quit();
}