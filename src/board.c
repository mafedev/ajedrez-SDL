#include "board.h"
#include <SDL2/SDL.h>
#include <stdio.h>

void initBoard(void) {
    for (int i = 0; i < TAMANIO; i++) {
        for (int j = 0; j < TAMANIO; j++) {
            if (i == 0) {
                // Piezas negras
                if (j == 0 || j == 7) {
                    tablero.tablero[i][j] = 'R'; 
                } else if (j == 1 || j == 6) {
                    tablero.tablero[i][j] = 'N'; 
                } else if (j == 2 || j == 5) {
                    tablero.tablero[i][j] = 'B'; 
                } else if (j == 3) {
                    tablero.tablero[i][j] = 'Q';  // Dama negra
                } else if (j == 4) {
                    tablero.tablero[i][j] = 'K';  // Rey negro
                }
            } else if (i == 1) {
                tablero.tablero[i][j] = 'P';
            } else if (i == 6) { // Piezas blancas
                tablero.tablero[i][j] = 'p';
            } else if (i == 7) {
                if (j == 0 || j == 7) {
                    tablero.tablero[i][j] = 'r';  
                } else if (j == 1 || j == 6) {
                    tablero.tablero[i][j] = 'n';  
                } else if (j == 2 || j == 5) {
                    tablero.tablero[i][j] = 'b'; 
                } else if (j == 3) {
                    tablero.tablero[i][j] = 'q';  // Dama blanca
                } else if (j == 4) {
                    tablero.tablero[i][j] = 'k';  // Rey blanco
                }
            } else {
                tablero.tablero[i][j] = '.';  // Otras casillas vacías
            }
        }
    }
}

void renderBoard(SDL_Renderer *renderer, TTF_Font *font, Tablero *jugadas) {
    SDL_Rect rect;
    rect.w = CELL_SIZE; // Ancho de la casilla
    rect.h = CELL_SIZE; // Alto de la casilla

    // Colores para las casillas del tablero
    SDL_Color blanco = {255, 255, 255, 255}; // Color balnco para el tablero
    SDL_Color negro = {66, 63, 63, 255}; // Color negro para el tablero
    SDL_Color verde = {51, 222, 62, 255};  // Verde para jugadas posibles
    SDL_Color rojo = {101, 0, 0, 255};    // Rojo para capturas posibles

    // Colores para los números y letras
    SDL_Color textColor = {0, 0, 0, 255};

    // Dibujar las casillas del tablero
    for (int i = 0; i < TAMANIO; i++) {
        for (int j = 0; j < TAMANIO; j++) {
            rect.x = (j + 1) * CELL_SIZE; // Posición x de la casilla
            rect.y = (i + 1) * CELL_SIZE; // Posición y de la casilla

            // Colorear según las jugadas posibles
            if (jugadas->tablero[i][j] == '1') {
                SDL_SetRenderDrawColor(renderer, verde.r, verde.g, verde.b, verde.a);  // Colorea de verde a donde se puede mover la pieza
            } else if (jugadas->tablero[i][j] == '2') {
                SDL_SetRenderDrawColor(renderer, rojo.r, rojo.g, rojo.b, rojo.a);  // Colorea de rojo las piezas que puede capturar
            } else {
                // Llena el tablero con casillas blancas y negras
                if (i % 2 == 0) { // Si la fila es par
                    if (j % 2 == 0) { // Y la columna es par
                        SDL_SetRenderDrawColor(renderer, blanco.r, blanco.g, blanco.b, blanco.a);  // Casilla blanca
                    } else {
                        SDL_SetRenderDrawColor(renderer, negro.r, negro.g, negro.b, negro.a);  // Casilla negra
                    }
                } else { // Si es impar
                    if (j % 2 == 0) {
                        SDL_SetRenderDrawColor(renderer, negro.r, negro.g, negro.b, negro.a);  // Casilla negra
                    } else {
                        SDL_SetRenderDrawColor(renderer, blanco.r, blanco.g, blanco.b, blanco.a);  // Casilla blanca
                    }
                }
            }

            SDL_RenderFillRect(renderer, &rect); // Rellena la casilla con el color

            if (tablero.tablero[i][j] != '.') { // Si en esa casilla hay una pieza, se dibuja
                drawPiece(renderer, tablero.tablero[i][j], rect.x, rect.y);
            }
        }
    }

    // Dibujar los números del tablero
    for (int i = 0; i < TAMANIO; i++) {
        char num[2]; // Buffer para alamcenar el número
        sprintf(num, "%d", TAMANIO - i); // Se muestran los números en orden descendente
        SDL_Surface *surface = TTF_RenderText_Solid(font, num, textColor);  // Se crea una superficie de texto
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface); // Convierte la superficie del texto en una textura
        SDL_Rect dstRect; // Rectángulo
        dstRect.x = 0; // Posición x, donde se va a dibujar el texto
        dstRect.y = (i + 1) * CELL_SIZE + CELL_SIZE / 4; // Posición y, que coloca los números en la parte izquierda, con un espacio
        // Se define el tamaño del rectángulo
        dstRect.w = CELL_SIZE / 2;
        dstRect.h = CELL_SIZE / 2;

        SDL_RenderCopy(renderer, texture, NULL, &dstRect); // Lo dibuja
        SDL_FreeSurface(surface); // Libera los recursos de la superficie
        SDL_DestroyTexture(texture); // Libera los recursos de la textura 
    }

    // Dibujar las letras en la parte superior
    for (int j = 0; j < TAMANIO; j++) {
        char letra[2]; // Buffer para almacenar una letra
        sprintf(letra, "%c", 'A' + j); // Cambie el índice por una letra, sumando lo que vale A en ASCII, más el índice j
        SDL_Surface *surface = TTF_RenderText_Solid(font, letra, textColor); // Genera la superficie del texto
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface); // Convierte la superficie en una textura
        SDL_Rect dstRect; // Rectángulo, y lo demás es igual que con los números
        dstRect.x = (j + 1) * CELL_SIZE + CELL_SIZE / 4;
        dstRect.y = 0;
        dstRect.w = CELL_SIZE / 2;
        dstRect.h = CELL_SIZE / 2;

        SDL_RenderCopy(renderer, texture, NULL, &dstRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}

void resetMoves(Tablero *jugadas){
    for(int i = 0; i < TAMANIO; i++){
        for(int j = 0; j < TAMANIO; j++){
            jugadas->tablero[i][j] = '.'; // Se llena todo el tablero con puntos, para que no acumulen las jugadas pasadas
        }
    }
}

bool movePiece(Tablero *jugadas) {
    enroqueConf = 0; // Se pone en 0, para que no hayan problemas
    if (jugadas->tablero[fila][columna] == '1' || jugadas->tablero[fila][columna] == '2') {
        // Se verifica si se está intentando hacer un enroque

        // BLANCAS
        // Enroque corto blanco
        if (pieza == 'k' && fila == 7 && columna == 6 && tablero.tablero[7][4] == 'k' && tablero.tablero[7][7] == 'r' && !enroqueBlancas) { // Si las piezas están en las posiciones correctas, sin piezas en el medio de ellas
            // Se realiza el cambio
            tablero.tablero[7][6] = 'k';
            tablero.tablero[7][5] = 'r';
            tablero.tablero[7][4] = '.';
            tablero.tablero[7][7] = '.';
            enroqueConf = 1; // Se marca como enroque corto
            enroqueBlancas = true; // Y se indica que ya se realizó un enroque en las blancas, para que no se pueda hacer otro
        } // Enroque largo blanco 
        else if (pieza == 'k' && fila == 7 && columna == 2 && tablero.tablero[7][4] == 'k' && tablero.tablero[7][0] == 'r' && !enroqueBlancas) { // Lo mismo, pero con el enroque largo
            // Se realiza el cambio
            tablero.tablero[7][2] = 'k';
            tablero.tablero[7][3] = 'r';
            tablero.tablero[7][4] = '.';
            tablero.tablero[7][0] = '.';
            enroqueConf = 2; // Se marca como enroque largo
            enroqueBlancas = true; // Y se indica que ya se realizó un enroque en las blancas, para que no se pueda hacer otro
        } // NEGRAS
        else if (pieza == 'K' && fila == 0 && columna == 6 && tablero.tablero[0][4] == 'K' && tablero.tablero[0][7] == 'R' && !enroqueNegras) { // Lo mismo que con las blancas, pero con las negras
            // Enroque corto negro
            tablero.tablero[0][6] = 'K';
            tablero.tablero[0][5] = 'R';
            tablero.tablero[0][4] = '.';
            tablero.tablero[0][7] = '.';
            enroqueConf = 3; // Se marca como enroque corto, pero de negras
            enroqueNegras = true; // Y se indica que ya se realizo enroque en las negras, para que no se pueda hacer otro
        } // Enroque largo negro
        else if (pieza == 'K' && fila == 0 && columna == 2 && tablero.tablero[0][4] == 'K' && tablero.tablero[0][0] == 'R' && !enroqueNegras) { // Lo mismo que con las blancas
            tablero.tablero[0][2] = 'K';
            tablero.tablero[0][3] = 'R';
            tablero.tablero[0][4] = '.';
            tablero.tablero[0][0] = '.';
            enroqueConf = 4;
            enroqueNegras = true;
        } else {
            // Movimiento normal
             // Se intercambian las piezas, para que simule el movimiento, dependiendo de la pieza
            tablero.tablero[fila][columna] = tablero.tablero[filaActual][columnaActual];
            tablero.tablero[filaActual][columnaActual] = '.';
            if (promotion()) {
                // Si se realiza una coronación, se cambia el peón por la pieza indicada
                tablero.tablero[fila][columna] = nuevaPieza;
            }
            return true;
        }
    } else {
        return false;
    }
    return true;
}