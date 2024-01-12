/*
 * game.h
 *
 * This header file declares functions necessary for the game. The functions
 * are further explained in more detail in src/game.c.
 */

#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

// Function declarations
void tipps_n_tricks();
void draw_board();
bool check_win(char input[]);
void display_winner(bool player);
void frame(char input[], int x, bool auswahl);
void quit();
void one_vs_one();
void menu();
void output(int ch, bool player, char ausgabe);
void one_vs_computer();
void disable();
void tic_tac_toe();
int get_larger(int num1, int num2);
int get_smaller(int num1, int num2);
void speichern_bestenliste(char spieler, int gewonneneSpiele);
void frage_bestenliste_speichern(char gewinner, int xWinCount, int oWinCount);

#endif
