#ifndef MOVES_H
#define MOVES_H

#include <SDL2/SDL.h>
#include "board.h"
#include "globals.h"
#include "config.h"

// Falta cambiarlo a ingles

void possibleMoves(Board *moves);
void pawnMoves(Board *moves);
void rookMoves(Board *moves);
void knightMoves(Board *moves);
void bishopMoves(Board *moves);
void queenMoves(Board *moves);
void kingMoves(Board *moves);

// ------------JUGADAS EXTRA----------------
bool promotion(); // Coronación de un peón
bool castling(Board *moves); // Enroque
bool stalemate(); // Tablas
bool checkmate(); // Jaque mate

#endif