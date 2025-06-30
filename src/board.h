#ifndef BOARD_H
#define BOARD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define TAMANIO = 8;

typedef struct {
    char tablero[TAMANIO][TAMANIO];
} Tablero;

extern Tablero tablero;

void initBoard(void);
void renderBoard(SDL_Renderer *renderer, TTF_Font *font, Tablero *jugadas);
void resetMoves(Tablero *jugadas);
bool movePiece(Tablero *jugadas);

#endif