#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h>
#include "config.h"

typedef struct {
    char board[TAMANIO][TAMANIO];
} Board;

extern Board board;
extern int row, column, currentRow, currentColumn, castlingConfirmation;
extern char columnLetter, currentColumnLetter, piece, newPiece;
extern bool turn, selectedPiece, whiteCastling, blackCastling, draw;

#endif