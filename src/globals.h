#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h>
#include "config.h"

typedef struct {
    char board[TAMANIO][TAMANIO];
} Board;

extern Board board;
extern int fila, columna, filaActual, columnaActual, enroqueConf;
extern char letraColumna, letraColumnaActual, piece, nuevaPieza;
extern bool turno, piezaSeleccionada, enroqueBlancas, enroqueNegras, empate;

#endif