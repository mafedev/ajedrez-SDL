#ifndef PIECES_H
#define PIECES_H

#include <SDL2/SDL.h>
#include "globals.h"

extern SDL_Texture *texturaReyNegro, *texturaReinaNegra, *texturaAlfilNegro, *texturaCaballoNegro, *texturaTorreNegra, *texturaPeonNegro;
extern SDL_Texture *texturaReyBlanco, *texturaReinaBlanca, *texturaAlfilBlanco, *texturaCaballoBlanco, *texturaTorreBlanca, *texturaPeonBlanco;

void loadTexture(SDL_Renderer *renderer);
void freeTextures(void);
void drawPiece(SDL_Renderer *renderer, char piece, int x, int y);

#endif