#include "windows.h"
#include <stdio.h>

SDL_Window *CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags)
{
    SDL_Window *window = SDL_CreateWindow(title, x, y, w, h, flags);
    if (!window)
    {
        printf("Error creando la ventana: %s\n", SDL_GetError());
        return NULL;
    }
    return window;
}

SDL_Renderer *CreateRenderer(SDL_Window *window, int index, Uint32 flags)
{
    SDL_Renderer *renderer = SDL_CreateRenderer(window, index, flags);
    if (!renderer)
    {
        printf("Error al crear el renderer: %s\n", SDL_GetError());
        return NULL;
    }
    return renderer;
}