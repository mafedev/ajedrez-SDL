#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h>
#include "config.h"

typedef struct {
    char tablero[TAMANIO][TAMANIO];
} Tablero;

extern Tablero tablero;
extern int fila, columna, filaActual, columnaActual, enroqueConf;
extern char letraColumna, letraColumnaActual, pieza, nuevaPieza;
extern bool turno, piezaSeleccionada, enroqueBlancas, enroqueNegras, empate;

#endif