#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

#define TAMANIO 8
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400
#define CELL_SIZE (WINDOW_WIDTH / TAMANIO)

// Ajusta estos valores según el tamaño de las piezas en tu sprite sheet
#define PIECE_WIDTH 80
#define PIECE_HEIGHT 67

typedef struct {
    char tablero[TAMANIO][TAMANIO];
} Tablero;

Tablero tablero;

// Variables globales
int fila, columna, filaActual, columnaActual;
char letraColumna, pieza;
bool turno = true;  // true para blancas, false para negras

SDL_Texture *texturaReyBlanco, *texturaReinaBlanca, *texturaAlfilBlanco, *texturaCaballoBlanco, *texturaTorreBlanca, *texturaPeonBlanco;
SDL_Texture *texturaReyNegro, *texturaReinaNegra, *texturaAlfilNegro, *texturaCaballoNegro, *texturaTorreNegra, *texturaPeonNegro;

// Tableros
void inicializarTablero();
void mostrarTablero(SDL_Renderer *renderer);
void dibujarPieza(SDL_Renderer *renderer, char pieza, int x, int y);
bool moverPieza(Tablero *jugadas);

// Solicitar información
bool solicitarInfoActual(bool turno);
bool solicitarJugada(bool turno);

// --------------- Jugadas válidas ---------------
bool confirmacionColor(bool turno);
bool confirmacionPosicion();

// --------------- JUGADAS -----------------------
void jugadasPosibles(bool turno, Tablero *jugadas);
void jugadasPeon(bool turno, Tablero *jugadas);
void jugadasTorre(bool turno, Tablero *jugadas);
void jugadasReina(bool turno, Tablero *jugadas);
void jugadasCaballo(bool turno, Tablero *jugadas);

void cargarTexturas(SDL_Renderer *renderer) {
    texturaReyBlanco = IMG_LoadTexture(renderer, "piezas/rey_b.png");
    if (!texturaReyBlanco) {
        printf("Error loading piezas/rey_b.png: %s\n", IMG_GetError());
    }
    texturaReinaBlanca = IMG_LoadTexture(renderer, "piezas/q_b.png");
    if (!texturaReinaBlanca) {
        printf("Error loading piezas/q_b.png: %s\n", IMG_GetError());
    }
    texturaAlfilBlanco = IMG_LoadTexture(renderer, "piezas/a_b.png");
    if (!texturaAlfilBlanco) {
        printf("Error loading piezas/a_b.png: %s\n", IMG_GetError());
    }
    texturaCaballoBlanco = IMG_LoadTexture(renderer, "piezas/c_b.png");
    if (!texturaCaballoBlanco) {
        printf("Error loading piezas/c_b.png: %s\n", IMG_GetError());
    }
    texturaTorreBlanca = IMG_LoadTexture(renderer, "piezas/t_b.png");
    if (!texturaTorreBlanca) {
        printf("Error loading piezas/t_b.png: %s\n", IMG_GetError());
    }
    texturaPeonBlanco = IMG_LoadTexture(renderer, "piezas/p_b.png");
    if (!texturaPeonBlanco) {
        printf("Error loading piezas/p_b.png: %s\n", IMG_GetError());
    }

    texturaReyNegro = IMG_LoadTexture(renderer, "piezas/r_n.png");
    if (!texturaReyNegro) {
        printf("Error loading piezas/r_n.png: %s\n", IMG_GetError());
    }
    texturaReinaNegra = IMG_LoadTexture(renderer, "piezas/q_n.png");
    if (!texturaReinaNegra) {
        printf("Error loading piezas/q_n.png: %s\n", IMG_GetError());
    }
    texturaAlfilNegro = IMG_LoadTexture(renderer, "piezas/a_n.png");
    if (!texturaAlfilNegro) {
        printf("Error loading piezas/a_n.png: %s\n", IMG_GetError());
    }
    texturaCaballoNegro = IMG_LoadTexture(renderer, "piezas/c_n.png");
    if (!texturaCaballoNegro) {
        printf("Error loading piezas/c_n.png: %s\n", IMG_GetError());
    }
    texturaTorreNegra = IMG_LoadTexture(renderer, "piezas/t_n.png");
    if (!texturaTorreNegra) {
        printf("Error loading piezas/t_n.png: %s\n", IMG_GetError());
    }
    texturaPeonNegro = IMG_LoadTexture(renderer, "piezas/p_n.png");
    if (!texturaPeonNegro) {
        printf("Error loading piezas/p_n.png: %s\n", IMG_GetError());
    }
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("Error initializing SDL_image: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Ajedrez", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Error creating window: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    cargarTexturas(renderer);

    inicializarTablero();

    bool running = true;
    SDL_Event event;
    int filaOrigen = -1, colOrigen = -1, filaDestino = -1, colDestino = -1;
    bool seleccionandoOrigen = true;
    Tablero jugadas;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                int fila = y / CELL_SIZE;
                int col = x / CELL_SIZE;

                if (seleccionandoOrigen) {
                    filaOrigen = fila;
                    colOrigen = col;
                    seleccionandoOrigen = false;
                } else {
                    filaDestino = fila;
                    colDestino = col;
                    if (moverPieza(&jugadas)) {
                        turno = !turno;
                    }
                    seleccionandoOrigen = true;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        mostrarTablero(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texturaReyBlanco);
    SDL_DestroyTexture(texturaReinaBlanca);
    SDL_DestroyTexture(texturaAlfilBlanco);
    SDL_DestroyTexture(texturaCaballoBlanco);
    SDL_DestroyTexture(texturaTorreBlanca);
    SDL_DestroyTexture(texturaPeonBlanco);
    SDL_DestroyTexture(texturaReyNegro);
    SDL_DestroyTexture(texturaReinaNegra);
    SDL_DestroyTexture(texturaAlfilNegro);
    SDL_DestroyTexture(texturaCaballoNegro);
    SDL_DestroyTexture(texturaTorreNegra);
    SDL_DestroyTexture(texturaPeonNegro);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}

void inicializarTablero() {
    for (int i = 0; i < TAMANIO; i++) {
        for (int j = 0; j < TAMANIO; j++) {
            if (i == 0) {
                if (j == 0 || j == 7) {
                    tablero.tablero[i][j] = 'T';  // Torres negras
                } else if (j == 1 || j == 6) {
                    tablero.tablero[i][j] = 'C';  // Caballos negros
                } else if (j == 2 || j == 5) {
                    tablero.tablero[i][j] = 'A';  // Alfileres negros
                } else if (j == 3) {
                    tablero.tablero[i][j] = 'Q';  // Reina negra
                } else if (j == 4) {
                    tablero.tablero[i][j] = 'K';  // Rey negro
                }
            } else if (i == 1) {
                tablero.tablero[i][j] = 'P';  // Peones negros
            } else if (i == 6) {
                tablero.tablero[i][j] = 'p';  // Peones blancos
            } else if (i == 7) {
                if (j == 0 || j == 7) {
                    tablero.tablero[i][j] = 't';  // Torres blancas
                } else if (j == 1 || j == 6) {
                    tablero.tablero[i][j] = 'c';  // Caballos blancos
                } else if (j == 2 || j == 5) {
                    tablero.tablero[i][j] = 'a';  // Alfileres blancos
                } else if (j == 3) {
                    tablero.tablero[i][j] = 'q';  // Reina blanca
                } else if (j == 4) {
                    tablero.tablero[i][j] = 'k';  // Rey blanco
                }
            } else {
                tablero.tablero[i][j] = '.';  // Casillas vacías
            }
        }
    }
}

void mostrarTablero(SDL_Renderer *renderer) {
    SDL_Rect rect;
    rect.w = CELL_SIZE;
    rect.h = CELL_SIZE;

    for (int i = 0; i < TAMANIO; i++) {
        for (int j = 0; j < TAMANIO; j++) {
            rect.x = j * CELL_SIZE;
            rect.y = i * CELL_SIZE;

            if ((i + j) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Blanco
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Negro
            }
            SDL_RenderFillRect(renderer, &rect);

            if (tablero.tablero[i][j] != '.') {
                dibujarPieza(renderer, tablero.tablero[i][j], rect.x, rect.y);
            }
        }
    }
}

void dibujarPieza(SDL_Renderer *renderer, char pieza, int x, int y) {
    SDL_Texture *textura = NULL;

    switch (pieza) {
        case 'k': textura = texturaReyNegro; break;
        case 'q': textura = texturaReinaNegra; break;
        case 'a': textura = texturaAlfilNegro; break;
        case 'c': textura = texturaCaballoNegro; break;
        case 't': textura = texturaTorreNegra; break;
        case 'p': textura = texturaPeonNegro; break;
        case 'K': textura = texturaReyBlanco; break;
        case 'Q': textura = texturaReinaBlanca; break;
        case 'A': textura = texturaAlfilBlanco; break;
        case 'C': textura = texturaCaballoBlanco; break;
        case 'T': textura = texturaTorreBlanca; break;
        case 'P': textura = texturaPeonBlanco; break;
        default: return;  // No dibujar nada si la pieza es inválida
    }

    SDL_Rect dstRect = {x, y, CELL_SIZE, CELL_SIZE};
    SDL_RenderCopy(renderer, textura, NULL, &dstRect);
}

bool moverPieza(Tablero *jugadas) {
    // Verifica que la jugada sea válida
    if (jugadas->tablero[fila][columna] != '0') {
        // Verifica que el peón no se mueva hacia atrás
        if ((pieza == 'p' && fila >= filaActual) || (pieza == 'P' && fila <= filaActual)) {
            printf("Movimiento no válido para el peón\n");
            return false;
        }

        // Realiza el movimiento
        tablero.tablero[fila][columna] = pieza;
        tablero.tablero[filaActual][columnaActual] = '.';
        return true;
    } else {
        printf("Jugada no válida\n");
        return false;
    }
}