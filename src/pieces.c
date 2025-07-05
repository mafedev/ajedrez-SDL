#include "pieces.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

// Texturas de las piezas
SDL_Texture *texturaReyBlanco, *texturaReinaBlanca, *texturaAlfilBlanco, *texturaCaballoBlanco, *texturaTorreBlanca, *texturaPeonBlanco;
SDL_Texture *texturaReyNegro, *texturaReinaNegra, *texturaAlfilNegro, *texturaCaballoNegro, *texturaTorreNegra, *texturaPeonNegro;

void loadTexture(SDL_Renderer *renderer) {
    // Piezas blancas
    texturaReyBlanco = IMG_LoadTexture(renderer, "../assets/images/pieces/rey-blanco.png"); // Se carga la textura de la piece, en este caso el rey blanco
    if (!texturaReyBlanco) { // Si no se pudo cargar la textura, se muestra un mensaje de error, y así con todas las piezas
        printf("Error cargando ../assets/images/pieces/rey-blanco.png: %s\n", IMG_GetError());
    }
    texturaReinaBlanca = IMG_LoadTexture(renderer, "../assets/images/pieces/dama-blanca.png");
    if (!texturaReinaBlanca) {
        printf("Error cargando ../assets/images/pieces/dama-blanca.png: %s\n", IMG_GetError());
    }
    texturaAlfilBlanco = IMG_LoadTexture(renderer, "../assets/images/pieces/alfil-blanco.png");
    if (!texturaAlfilBlanco) {
        printf("Error cargando ../assets/images/pieces/alfil-blanco.png: %s\n", IMG_GetError());
    }
    texturaCaballoBlanco = IMG_LoadTexture(renderer, "../assets/images/pieces/caballo-blanco.png");
    if (!texturaCaballoBlanco) {
        printf("Error cargando ../assets/images/pieces/caballo-blanco.png: %s\n", IMG_GetError());
    }
    texturaTorreBlanca = IMG_LoadTexture(renderer, "../assets/images/pieces/torre-blanca.png");
    if (!texturaTorreBlanca) {
        printf("Error cargando ../assets/images/pieces/torre-blanca.png: %s\n", IMG_GetError());
    }
    texturaPeonBlanco = IMG_LoadTexture(renderer, "../assets/images/pieces/peon-blanco.png");
    if (!texturaPeonBlanco) {
        printf("Error cargando ../assets/images/pieces/peon-blanco.png: %s\n", IMG_GetError());
    }

    // Piezas negras
    texturaReyNegro = IMG_LoadTexture(renderer, "../assets/images/pieces/rey-negro.png");
    if (!texturaReyNegro) {
        printf("Error cargando ../assets/images/pieces/rey-negro.png: %s\n", IMG_GetError());
    }
    texturaReinaNegra = IMG_LoadTexture(renderer, "../assets/images/pieces/dama-negra.png");
    if (!texturaReinaNegra) {
        printf("Error cargando ../assets/images/pieces/dama-negra.png: %s\n", IMG_GetError());
    }
    texturaAlfilNegro = IMG_LoadTexture(renderer, "../assets/images/pieces/alfil-negro.png");
    if (!texturaAlfilNegro) {
        printf("Error cargando ../assets/images/pieces/alfil-negro.png: %s\n", IMG_GetError());
    }
    texturaCaballoNegro = IMG_LoadTexture(renderer, "../assets/images/pieces/caballo-negro.png");
    if (!texturaCaballoNegro) {
        printf("Error cargando ../assets/images/pieces/caballo-negro.png: %s\n", IMG_GetError());
    }
    texturaTorreNegra = IMG_LoadTexture(renderer, "../assets/images/pieces/torre-negra.png");
    if (!texturaTorreNegra) {
        printf("Error cargando ../assets/images/pieces/torre-negra.png: %s\n", IMG_GetError());
    }
    texturaPeonNegro = IMG_LoadTexture(renderer, "../assets/images/pieces/peon-negro.png");
    if (!texturaPeonNegro) {
        printf("Error cargando ../assets/images/pieces/peon-negro.png: %s\n", IMG_GetError());
    }
}

void freeTextures(void) {
    // Libera todas las texturas si no son NULL
    #define LIBERA_TEX(t) if (t) { SDL_DestroyTexture(t); t = NULL; }
    LIBERA_TEX(texturaReyBlanco)
    LIBERA_TEX(texturaReinaBlanca)
    LIBERA_TEX(texturaAlfilBlanco)
    LIBERA_TEX(texturaCaballoBlanco)
    LIBERA_TEX(texturaTorreBlanca)
    LIBERA_TEX(texturaPeonBlanco)
    LIBERA_TEX(texturaReyNegro)
    LIBERA_TEX(texturaReinaNegra)
    LIBERA_TEX(texturaAlfilNegro)
    LIBERA_TEX(texturaCaballoNegro)
    LIBERA_TEX(texturaTorreNegra)
    LIBERA_TEX(texturaPeonNegro)
    #undef LIBERA_TEX
}

void drawPiece(SDL_Renderer *renderer, char piece, int x, int y) {
    SDL_Texture *textura = NULL;

    switch (piece) {
        case 'K': textura = texturaReyNegro; break;
        case 'Q': textura = texturaReinaNegra; break;
        case 'B': textura = texturaAlfilNegro; break;
        case 'N': textura = texturaCaballoNegro; break;
        case 'R': textura = texturaTorreNegra; break;
        case 'P': textura = texturaPeonNegro; break;
        case 'k': textura = texturaReyBlanco; break;
        case 'q': textura = texturaReinaBlanca; break;
        case 'b': textura = texturaAlfilBlanco; break;
        case 'n': textura = texturaCaballoBlanco; break;
        case 'r': textura = texturaTorreBlanca; break;
        case 'p': textura = texturaPeonBlanco; break;
        default: return;  // No dibujar nada si la pieza es inválida
    }

    SDL_Rect dstRect = {x, y, 50, 50};
    SDL_RenderCopy(renderer, textura, NULL, &dstRect);
}