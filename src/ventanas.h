#ifndef VENTANAS_H
#define VENTANAS_H

#include <SDL2/SDL.h>

SDL_Window *CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags);
SDL_Renderer *CreateRenderer(SDL_Window *window, int index, Uint32 flags);

#endif