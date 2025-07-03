#ifndef MOVES_H
#define MOVES_H

#include <SDL2/SDL.h>
#include "board.h"
#include "globals.h"
#include "config.h"

// Falta cambiarlo a ingles

void possibleMoves(Tablero *jugadas);
void pawnMoves(Tablero *jugadas);
void rookMoves(Tablero *jugadas);
void knightMoves(Tablero *jugadas);
void bishopMoves(Tablero *jugadas);
void queenMoves(Tablero *jugadas);
void kingMoves(Tablero *jugadas);

// ------------JUGADAS EXTRA----------------
bool promotion(); // Coronación de un peón
bool castling(Tablero *jugadas); // Enroque
bool stalemate(); // Tablas
bool checkmate(); // Jaque mate

#endif