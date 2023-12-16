#include <stdbool.h>
#ifndef GAME_H
#define GAME_H

void quit();
bool checkHorizontal(char input[]);
bool checkDiagonal(char input[]);
bool checkVertical(char input[]);
bool checkWin(char input[]);
void drawBoard();
void displayWinner(bool player);
void output(int ch, bool player);

#endif