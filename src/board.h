#ifndef BOARD_H
#define BOARD_H

#include "globals.h"
#include "pieces.h"
#include "moves.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

void initBoard(void);
void renderBoard(SDL_Renderer *renderer, TTF_Font *font, Board *jugadas);
void resetMoves(Board *jugadas);
bool movePiece(Board *jugadas);

#endif