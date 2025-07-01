#include "board.h"
#include <stdbool.h>
#include "globals.h"

Tablero tablero;
int fila = 0, columna = 0, filaActual = 0, columnaActual = 0, enroqueConf = 0;
char letraColumna = 0, letraColumnaActual = 0, pieza, nuevaPieza;
bool turno = true, piezaSeleccionada = false, enroqueBlancas = false, enroqueNegras = false, empate = false;