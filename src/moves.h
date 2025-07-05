#ifndef MOVES_H
#define MOVES_H

#include <SDL2/SDL.h>
#include "board.h"
#include "globals.h"
#include "config.h"

// Falta cambiarlo a ingles

void possibleMoves(Board *jugadas);
void pawnMoves(Board *jugadas);
void rookMoves(Board *jugadas);
void knightMoves(Board *jugadas);
void bishopMoves(Board *jugadas);
void queenMoves(Board *jugadas);
void kingMoves(Board *jugadas);

// ------------JUGADAS EXTRA----------------
bool promotion(); // Coronación de un peón
bool castling(Board *jugadas); // Enroque
bool stalemate(); // Tablas
bool checkmate(); // Jaque mate

#endif