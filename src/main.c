#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "globals.h"
#include "board.h"
#include "fonts.h"
#include "moves.h"
#include "pieces.h"
#include "utils.h"
#include "windows.h"

// --------------------DECLARACIÓN DE FUNCIONES--------------------

// Planilla
void mostrarNotacion(SDL_Renderer *renderer, TTF_Font *font, int *contadorJugadas, Tablero *jugadas, char *buffer);
void notacionAlgebraica(int *contadorJugadas, Tablero *jugadas, char *buffer);

int main(int argc, char *argv[]) {
    // Inicializa SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Error inicializando SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Inicializa SDL_ttf
    if (TTF_Init() != 0) {
        printf("Error inicializando SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Inicializa SDL_image
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("Error inicializando SDL_image: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    // Crea una ventana
    SDL_Window *window = CreateWindow("Ajedrez", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Crea un renderer
    SDL_Renderer *renderer = CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Crea una segunda ventana
    SDL_Window *window2 = CreateWindow("Planilla de Anotación", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window2) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Crea un renderer para la segunda ventana
    SDL_Renderer *renderer2 = CreateRenderer(window2, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer2) {
        SDL_DestroyWindow(window2);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Fuente para la planilla
    TTF_Font *font2 = TTF_OpenFont("../assets/fonts/static/OpenSans-Regular.ttf", 15);
    if (!font2){
        SDL_DestroyRenderer(renderer2);
        SDL_DestroyWindow(window2);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // --------------- LÓGICA JUEGO --------------- 

    // Variables
    bool running = true, continuar = true;
    int contadorJugadas = 1;
    char buffer[1024] = "";
    Tablero jugadas;
    bool juegoTerminado = false;

    // Cargar texturas y inicializar tablero
    loadTexture(renderer);
    initBoard();


    //--------------BUCLE PRINCIPAL----------------
    while (running) { 
        SDL_Event evento;

        // Procesa los eventos en la cola de eventos, obtienen el evento de la cola y lo almacena en evento, si no hay, se acaba el bucle
        while (SDL_PollEvent(&evento)) {
            // Si se cierra la ventana
            if (evento.type == SDL_QUIT) {
                printf("Evento de cierre detectado\n");
                running = false; // Se acaba el programa
                break;
            } else if (evento.type == SDL_MOUSEBUTTONDOWN && !juegoTerminado) { // Cuando se da Click y el juego no ha terminado
                // Obtiene la posición del mouse
                int x, y;
                SDL_GetMouseState(&x, &y); // Obtiene la posición del mouse y la almacena en x y
                int filaClick = y / CELL_SIZE - 1; // -1 para ajustar indíce
                int columnaClick = x / CELL_SIZE - 1;
    
                // Si la posición es válida
                if (filaClick >= 0 && filaClick < TAMANIO && columnaClick >= 0 && columnaClick < TAMANIO) {
                    if (!piezaSeleccionada) { // Si no hay una pieza seleccionada
                        filaActual = filaClick;
                        columnaActual = columnaClick;
                        pieza = tablero.tablero[filaActual][columnaActual]; // Obtiene la pieza en la posición actual
    
                        // Verificar el turno
                        if (confirmacionColor()) {
                            piezaSeleccionada = true; // Se selecciona la pieza
                            jugadasPosibles(&jugadas); // Se obtienen las jugadas posibles
                        }
                    } else { // Si hay una pieza seleccionada
                        fila = filaClick;
                        columna = columnaClick;
                        if (movePiece(&jugadas)) { // Se mueve la pieza
                            notacionAlgebraica(&contadorJugadas, &jugadas, buffer); // Actualiza la planilla
                            turno = !turno; // Después de mover la pieza se cambia el turno
                        }
                        piezaSeleccionada = false; // Se pone en false, para que se pueda seleccionar otra pieza
                        resetMoves(&jugadas); // Se reinician las jugadas posibles
                    }
                }
            }
        }

        // Verificar si hay jaque mate
        if (jaqueMate() && !juegoTerminado) {
            printf("Jaque mate!\n"); // Se muestra por consola
            juegoTerminado = true; // Se termina el juego
            notacionAlgebraica(&contadorJugadas, &jugadas, buffer); // Actualiza la planilla
            // Muestra el ganador en la planilla
            if (turno) {
                strcat(buffer, "\n1-0"); // Se concatena el resultado con el buffer, para que lo muestre al final en la planilla de anotación
            } else {
                strcat(buffer, "\n0-1");
            }
        }

        // Verificar si hay tablas
        if (tablas() && !juegoTerminado) {
            printf("La partida ha terminado en tablas\n");
            juegoTerminado = true; // Si hay tablas por alguna de las razones, se termina el juego
            notacionAlgebraica(&contadorJugadas, &jugadas, buffer); // Actualiza la planilla
            strcat(buffer, "\n1/2-1/2"); // Se concatena el resultado con el buffer, para que lo muestre al final en la planilla de anotación
        }

        // Renderiza el tablero
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer); // Limpia la pantalla 
        renderBoard(renderer, font2, &jugadas); // Dibuja el tableo de ajedrez
        SDL_RenderPresent(renderer); // Muestra el "dibujo" en la pantalla

        // SEGUNDA VENTANA
        // Renderiza la planilla de notación
        SDL_SetRenderDrawColor(renderer2, 255, 255, 255, 255);
        SDL_RenderClear(renderer2);
        mostrarNotacion(renderer2, font2, &contadorJugadas, &jugadas, buffer); // Lo mismo de arriba, pero para la planilla, y se le pasa el buffer, que es donde se van a almacenar las notaciones
        SDL_RenderPresent(renderer2);

        SDL_Delay(16); // Espera 16 milisegundos
    }

    if (juegoTerminado) {
        printf("El juego ha terminado.\n");
    }

    // Limpia y cierra SDL
    TTF_CloseFont(font2); // Se cierra y liberan los recursos de la fuente, para evitar fugas de memoria
    SDL_DestroyRenderer(renderer2); // Se destruye el renderer de la segunda ventana, tambien para liberar recursos
    SDL_DestroyWindow(window2); // Se destruye la segunda ventana
    SDL_DestroyRenderer(renderer); // Se destruye el renderer de la primera ventana
    SDL_DestroyWindow(window); // Se destruye la primera ventana
    TTF_Quit(); // Se cierra la librería que maneja las fuentes
    IMG_Quit(); // Lo mismo, pero con la librería de imágenes
    SDL_Quit(); // Se cierra SDL

    return 0;
}

// -----------------PLANILLA----------------
// Muestra la planilla de anotación con las jugadas, a medida que se van haciendo
void mostrarNotacion(SDL_Renderer *renderer, TTF_Font *font, int *contadorJugadas, Tablero *jugadas, char *buffer) {
    SDL_Color color = {0, 0, 0}; // Color negro

    // Crear una superficie para el texto
    SDL_Surface* surfaceTexto = TTF_RenderText_Blended_Wrapped(font, buffer, color, WINDOW_WIDTH); // El texto se toma del buffer, que es donde se va almacenando el texto de las jugadas con su respectiva notación algebráica

    // Crear la textura a partir de la superficie, para que se pueda renderizar
    SDL_Texture* texturaTexto = SDL_CreateTextureFromSurface(renderer, surfaceTexto);

    // Obtener las dimensiones de la textura, ancho y alto
    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texturaTexto, NULL, NULL, &texW, &texH);

    // Definir la posición y el rectángulo donde se dibujará la textura, que es la esquina superior izquierda, pero con un pequeño margen
    SDL_Rect dstTexto = {15, 15, texW, texH}; // Lo primero son las coordenadas donde se va a colocar el texto, yo puse 15, 15 para que se vea mejor

    // Renderizar el texto completo, es decir, lo muestra en la ventana
    SDL_RenderCopy(renderer, texturaTexto, NULL, &dstTexto);

    // Liberar recursos
    SDL_FreeSurface(surfaceTexto);
    SDL_DestroyTexture(texturaTexto);
}

// ---------- NOTACIÓN ALGEBRÁICA ------------
void notacionAlgebraica(int *contadorJugadas, Tablero *jugadas, char *buffer) {
    letraColumna = 'a' + columna;
    letraColumnaActual = 'a' + columnaActual;
    int ambiguedad = comprobarAmbiguedad();
    char temp[50]; // Buffer temporal para la notación de una jugada

    // Es para mostrar la primera fila, y saber cuáles son las blancas y cuáles las negras
    if(*contadorJugadas <= 1 && turno){
        sprintf(temp, "Blancas   Negras\n");
        strcat(buffer, temp);
    }

    // Si es turno de las blancas, muestra el número de la jugada
    if (turno) {
        // Añadir un salto de línea antes del contador de jugadas
        sprintf(temp, "\n%d. ", *contadorJugadas); // Se guarda el texto en temp
        strcat(buffer, temp); // Añade temp al final de la cadena buffer, para lueg omostrarlo en la pantalla
    }

    // Notación para tablas
    if(empate){
        sprintf(temp, "1/2 - 1/2");
        strcat(buffer, temp);
    }

    // Notación de enroque
    if (enroqueConf == 1) { // Si hay enroque corto en las blancas
        sprintf(temp, "0-0   \t"); // Se guarda el texto en temp
        strcat(buffer, temp); // Añade temp al final de la cadena buffer
        return; // Es para que no siga ejecutando el código, y no muestre las demás jugadas
    } else if (enroqueConf == 2) { // Si hay enroque largo en las blancas
        sprintf(temp, "0-0-0 \t");
        strcat(buffer, temp);
        return;
    } else if (enroqueConf == 3) { // Si hay enroque corto en las negras
        sprintf(temp, "0-0   \t");
        strcat(buffer, temp);
        return;
    } else if (enroqueConf == 4) { // Si hay enroque largo en las negras
        sprintf(temp, "0-0-0  \t");
        strcat(buffer, temp); // Se añade la jugada al buffer
        return;
    }

    // Notación de coronación
    if ((pieza == 'P' && fila == 7) || (pieza == 'p' && fila == 0)) { // Si alguno de los peones llega a la última fila
        sprintf(temp, "%c%d=%c\t", letraColumna, fila, nuevaPieza); // Muestra la notación, junto con la pieza que reclama
        strcat(buffer, temp); // Añadir la jugada al buffer
        return;
    }

    // Se pasa la pieza a mayúscula para que sea más facil de manejar
    pieza = toupper(pieza);

    // Notación para los peones
    if (pieza == 'P') {
        // Si captura una pieza
        if (jugadas->tablero[fila][columna] == '2') { // Las posibles capturas se marcan con 2
            sprintf(temp, "%cx%c%d     \t", letraColumnaActual, letraColumna, fila);
        } else {
            sprintf(temp, "%c%d       \t", letraColumna, fila); // Si no hay captura, solo se muestra a que celda se mueve
        }
    } else if (pieza == 'C' || pieza == 'A' || pieza == 'T') { // Piezas en las que pueden ocurrir ambigüedades
        // Si hay captura
        if (jugadas->tablero[fila][columna] == '2') {
            sprintf(temp, "%c%cx%c%d  \t", pieza, letraColumnaActual, letraColumna, fila);
        } else if (ambiguedad == 1) {
            sprintf(temp, "%c%d%c%d   \t", pieza, filaActual, letraColumna, fila);
        } else if (ambiguedad == 2) {
            sprintf(temp, "%c%c%c%d   \t", pieza, letraColumnaActual, letraColumna, fila);
        }
    } else {
        // Si hay captura
        if (jugadas->tablero[fila][columna] == '2') {
            sprintf(temp, "%cx%c%d   \t", pieza, letraColumna, fila);
        } else {
            sprintf(temp, "%c%c%d   \t", pieza, letraColumna, fila);
        }
    }

    strcat(buffer, temp); // Se añade la jugada al buffer

    if (!turno) { // Si es el turno de las negras, se añade un salto de línea
        (*contadorJugadas)++; // Y se aumenta el contador de jugadas
    }
}
