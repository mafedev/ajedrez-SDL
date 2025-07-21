#include <stdbool.h>
#include "globals.h"

Board board;
int row = 0, column = 0, currentRow = 0, currentColumn = 0, castlingConfirmation = 0;
char columnLetter= 0, currentColumnLetter= 0, piece, newPiece;
bool turn = true, selectedPiece = false, whiteCastling= false, blackCastling= false, draw= false;