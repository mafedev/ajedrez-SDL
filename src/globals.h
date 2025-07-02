#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h>
#include "config.h"
#include "board.h"

extern Tablero tablero;
extern int fila, columna, filaActual, columnaActual, enroqueConf;
extern char letraColumna, letraColumnaActual, pieza, nuevaPieza;
extern bool turno, piezaSeleccionada, enroqueBlancas, enroqueNegras, empate;

#endif