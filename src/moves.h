#ifndef MOVES_H
#define MOVES_H

#include <SDL2/SDL.h>
#include "board.h"
// Falta cambiarlo a ingles

void jugadasPosibles(Tablero *jugadas);
void jugadasPeon(Tablero *jugadas);
void jugadasTorre(Tablero *jugadas);
void jugadasCaballo(Tablero *jugadas);
void jugadasAlfil(Tablero *jugadas);
void jugadasReina(Tablero *jugadas);
void jugadasRey(Tablero *jugadas);

// ------------JUGADAS EXTRA----------------
bool coronacion();
bool enroque(Tablero *jugadas);
bool tablas();
bool jaqueMate();

#endif