#include "fonts.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

TTF_Font *OpenFont(const char *path, int size) {
    TTF_Font *font = TTF_OpenFont(path, size);
    if (!font) {
        printf("Error al cargar la fuente: %s\n", TTF_GetError());
        return NULL;
    }
    return font;
}