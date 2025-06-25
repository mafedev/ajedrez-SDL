#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#define TAMANIO 8
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define CELL_SIZE 50

typedef struct {
    char tablero[TAMANIO][TAMANIO];
} Tablero;


// Variables globales
Tablero tablero;
int fila = 0, columna = 0, filaActual = 0, columnaActual = 0, enroqueConf = 0;
char letraColumna = 0, letraColumnaActual = 0, pieza, nuevaPieza;
bool turno = true, piezaSeleccionada = false, enroqueBlancas = false, enroqueNegras =  false, empate = false;

// Texturas de las piezas
SDL_Texture *texturaReyNegro, *texturaReinaNegra, *texturaAlfilNegro, *texturaCaballoNegro, *texturaTorreNegra, *texturaPeonNegro;
SDL_Texture *texturaReyBlanco, *texturaReinaBlanca, *texturaAlfilBlanco, *texturaCaballoBlanco, *texturaTorreBlanca, *texturaPeonBlanco;


// --------------------DECLARACIÓN DE FUNCIONES--------------------

// Tableros
void inicializarTablero(); 
void mostrarTablero(SDL_Renderer *renderer, TTF_Font *font, Tablero *jugadas);
void reiniciarJugadas(Tablero *jugadas);
bool moverPieza(Tablero *jugadas);

// Planilla
void mostrarNotacion(SDL_Renderer *renderer, TTF_Font *font, int *contadorJugadas, Tablero *jugadas, char *buffer);
void notacionAlgebraica(int *contadorJugadas, Tablero *jugadas, char *buffer);

// --------------TEXTURAS / PIEZAS----------------
void cargarTexturas(SDL_Renderer *renderer);
void dibujarPieza(SDL_Renderer *renderer, char pieza, int x, int y);

// --------------- JUGADAS -----------------------
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
bool confirmacionColor();
int comprobarAmbiguedad();


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
    SDL_Window *window = SDL_CreateWindow("Ajedrez", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Error creando la ventana: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Crea un renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("Error al crear el renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Crea una segunda ventana
    SDL_Window *window2 = SDL_CreateWindow("Planilla de Anotación", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window2) {
        printf("Error creando la segunda ventana: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Crea un renderer para la segunda ventana
    SDL_Renderer *renderer2 = SDL_CreateRenderer(window2, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer2) {
        printf("Error al crear el renderer para la segunda ventana: %s\n", SDL_GetError());
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
        printf("Error al cargar la fuente para la ventana 2: %s\n", TTF_GetError());
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
    cargarTexturas(renderer);
    inicializarTablero();


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
                        if (moverPieza(&jugadas)) { // Se mueve la pieza
                            notacionAlgebraica(&contadorJugadas, &jugadas, buffer); // Actualiza la planilla
                            turno = !turno; // Después de mover la pieza se cambia el turno
                        }
                        piezaSeleccionada = false; // Se pone en false, para que se pueda seleccionar otra pieza
                        reiniciarJugadas(&jugadas); // Se reinician las jugadas posibles
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
        mostrarTablero(renderer, font2, &jugadas); // Dibuja el tableo de ajedrez
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

//----------------TABLEROS----------------

// Inicializar el tablero con las piezas
void inicializarTablero() {
    for (int i = 0; i < TAMANIO; i++) {
        for (int j = 0; j < TAMANIO; j++) {
            if (i == 0) {
                // Piezas negras
                if (j == 0 || j == 7) {
                    tablero.tablero[i][j] = 'T'; 
                } else if (j == 1 || j == 6) {
                    tablero.tablero[i][j] = 'C'; 
                } else if (j == 2 || j == 5) {
                    tablero.tablero[i][j] = 'A'; 
                } else if (j == 3) {
                    tablero.tablero[i][j] = 'D';  // Reina/dama negra
                } else if (j == 4) {
                    tablero.tablero[i][j] = 'R';  // Rey negro
                }
            } else if (i == 1) {
                tablero.tablero[i][j] = 'P';
            } else if (i == 6) { // Piezas blancas
                tablero.tablero[i][j] = 'p';
            } else if (i == 7) {
                if (j == 0 || j == 7) {
                    tablero.tablero[i][j] = 't';  
                } else if (j == 1 || j == 6) {
                    tablero.tablero[i][j] = 'c';  
                } else if (j == 2 || j == 5) {
                    tablero.tablero[i][j] = 'a'; 
                } else if (j == 3) {
                    tablero.tablero[i][j] = 'd';  // Reina/dama blanca
                } else if (j == 4) {
                    tablero.tablero[i][j] = 'r';  // Rey blanco
                }
            } else {
                tablero.tablero[i][j] = '.';  // Otras casillas vacías
            }
        }
    }
}

// Mostrar tablero de ajedrez
void mostrarTablero(SDL_Renderer *renderer, TTF_Font *font, Tablero *jugadas) {
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
                dibujarPieza(renderer, tablero.tablero[i][j], rect.x, rect.y);
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

void reiniciarJugadas(Tablero *jugadas){
    for(int i = 0; i < TAMANIO; i++){
        for(int j = 0; j < TAMANIO; j++){
            jugadas->tablero[i][j] = '.'; // Se llena todo el tablero con puntos, para que no acumulen las jugadas pasadas
        }
    }
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


//-----------------------TEXTURAS PIEZAS-----------------------
void cargarTexturas(SDL_Renderer *renderer) {
    // Piezas blancas
    texturaReyBlanco = IMG_LoadTexture(renderer, "piezas/rey_b.png"); // Se carga la textura de la pieza, en este caso el rey blanco
    if (!texturaReyBlanco) { // Si no se pudo cargar la textura, se muestra un mensaje de error, y así con todas las piezas
        printf("Error cargando piezas/rey_b.png: %s\n", IMG_GetError());
    }
    texturaReinaBlanca = IMG_LoadTexture(renderer, "piezas/q_b.png");
    if (!texturaReinaBlanca) {
        printf("Error cargando piezas/q_b.png: %s\n", IMG_GetError());
    }
    texturaAlfilBlanco = IMG_LoadTexture(renderer, "piezas/a_b.png");
    if (!texturaAlfilBlanco) {
        printf("Error cargando piezas/a_b.png: %s\n", IMG_GetError());
    }
    texturaCaballoBlanco = IMG_LoadTexture(renderer, "piezas/c_b.png");
    if (!texturaCaballoBlanco) {
        printf("Error cargando piezas/c_b.png: %s\n", IMG_GetError());
    }
    texturaTorreBlanca = IMG_LoadTexture(renderer, "piezas/t_b.png");
    if (!texturaTorreBlanca) {
        printf("Error cargando piezas/t_b.png: %s\n", IMG_GetError());
    }
    texturaPeonBlanco = IMG_LoadTexture(renderer, "piezas/p_b.png");
    if (!texturaPeonBlanco) {
        printf("Error cargando piezas/p_b.png: %s\n", IMG_GetError());
    }

    // Piezas negras
    texturaReyNegro = IMG_LoadTexture(renderer, "piezas/r_n.png");
    if (!texturaReyNegro) {
        printf("Error cargando piezas/r_n.png: %s\n", IMG_GetError());
    }
    texturaReinaNegra = IMG_LoadTexture(renderer, "piezas/q_n.png");
    if (!texturaReinaNegra) {
        printf("Error cargando piezas/q_n.png: %s\n", IMG_GetError());
    }
    texturaAlfilNegro = IMG_LoadTexture(renderer, "piezas/a_n.png");
    if (!texturaAlfilNegro) {
        printf("Error cargando piezas/a_n.png: %s\n", IMG_GetError());
    }
    texturaCaballoNegro = IMG_LoadTexture(renderer, "piezas/c_n.png");
    if (!texturaCaballoNegro) {
        printf("Error cargando piezas/c_n.png: %s\n", IMG_GetError());
    }
    texturaTorreNegra = IMG_LoadTexture(renderer, "piezas/t_n.png");
    if (!texturaTorreNegra) {
        printf("Error cargando piezas/t_n.png: %s\n", IMG_GetError());
    }
    texturaPeonNegro = IMG_LoadTexture(renderer, "piezas/p_n.png");
    if (!texturaPeonNegro) {
        printf("Error cargando piezas/p_n.png: %s\n", IMG_GetError());
    }
}

void dibujarPieza(SDL_Renderer *renderer, char pieza, int x, int y) {
    SDL_Texture *textura = NULL;

    switch (pieza) {
        case 'R': textura = texturaReyNegro; break;
        case 'D': textura = texturaReinaNegra; break;
        case 'A': textura = texturaAlfilNegro; break;
        case 'C': textura = texturaCaballoNegro; break;
        case 'T': textura = texturaTorreNegra; break;
        case 'P': textura = texturaPeonNegro; break;
        case 'r': textura = texturaReyBlanco; break;
        case 'd': textura = texturaReinaBlanca; break;
        case 'a': textura = texturaAlfilBlanco; break;
        case 'c': textura = texturaCaballoBlanco; break;
        case 't': textura = texturaTorreBlanca; break;
        case 'p': textura = texturaPeonBlanco; break;
        default: return;  // No dibujar nada si la pieza es inválida
    }

    SDL_Rect dstRect = {x, y, CELL_SIZE, CELL_SIZE}; ///////////////////
    SDL_RenderCopy(renderer, textura, NULL, &dstRect);
}


// --------------- JUGADAS -----------------------
void jugadasPosibles(Tablero *jugadas) { // Se pasan las jugadas posibles
    // Dependiendo de la pieza, se calculan las jugadas posibles
    if (pieza == 'P' || pieza == 'p') {
        jugadasPeon(jugadas);
    } else if (pieza == 'D' || pieza == 'd') {
        jugadasReina(jugadas);
    } else if (pieza == 'R' || pieza == 'r') {
        jugadasRey(jugadas);
    } else if (pieza == 'C' || pieza == 'c') {
        jugadasCaballo(jugadas);
    } else if (pieza == 'A' || pieza == 'a') {
        jugadasAlfil(jugadas);
    } else if (pieza == 'T' || pieza == 't') {
        jugadasTorre(jugadas);
    } else {
        printf("Seleccione una pieza válida\n");
    }
}

// PEONES
void jugadasPeon(Tablero *jugadas) {
    int sentido = (turno) ? -1 : 1;  // Si es turno de las blancas, el sentido es -1 para que vaya hacia arriba, si no, el sentido es 1 para que vayan hacia abajo
    int casilla = filaActual + sentido;     // Calcula la casilla hacia adelante según el sentido

    // Movimiento hacia adelante
    if (casilla >= 0 && casilla < TAMANIO && tablero.tablero[casilla][columnaActual] == '.') { // Si la casilla está vacía y dentro del rango
        jugadas->tablero[casilla][columnaActual] = '1';  // Casilla vacía, movimiento válido

        // Movimiento inicial de dos casillas
        if ((turno && filaActual == 6) || (!turno && filaActual == 1)) {  // Si es el turno de las blancas y está en fila 6, o turno el turno de las negras y está en la fila 1
            casilla = filaActual + 2 * sentido;  // Calcula la casilla dos espacios adelante
            if (casilla >= 0 && casilla < TAMANIO && tablero.tablero[casilla][columnaActual] == '.') { // Si la casilla está vacía y dentro del rango
                jugadas->tablero[casilla][columnaActual] = '1';  // Movimiento de dos casillas válido
            }
        }
    }

    if(turno){ // Si es el turno de las blancas

        // Captura en diagonal izquierda
        if (columnaActual - 1 >= 0 && casilla >= 0 && casilla < TAMANIO && tablero.tablero[casilla][columnaActual - 1] != '.' && isupper(tablero.tablero[casilla][columnaActual - 1])) { // Si la casilla no está vacía, está dentro del rango y es una pieza negra
            jugadas->tablero[casilla][columnaActual - 1] = '2';  // Lo marca como una captura posible con el 2
        }
    
        // Captura en diagonal derecha
        if (columnaActual + 1 < TAMANIO && casilla >= 0 && casilla < TAMANIO && tablero.tablero[casilla][columnaActual + 1] != '.' && isupper(tablero.tablero[casilla][columnaActual + 1])) { // Lo mismo, pero con la diagonal derecha
            jugadas->tablero[casilla][columnaActual + 1] = '2';  // Lo marca como una captura posible con el 2
        }

    } else { // Si es el turno de las negras
        // Captura en diagonal izquierda
        if (columnaActual - 1 >= 0 && casilla >= 0 && casilla < TAMANIO && tablero.tablero[casilla][columnaActual - 1] != '.' && islower(tablero.tablero[casilla][columnaActual - 1])) { // Si la casilla no está vacía, está dentro del rango y es una pieza blanca
            jugadas->tablero[casilla][columnaActual - 1] = '2';  // Lo marca como una captura posible con el 2
        }
    
        // Captura en diagonal derecha
        if (columnaActual + 1 < TAMANIO && casilla >= 0 && casilla < TAMANIO && tablero.tablero[casilla][columnaActual + 1] != '.' && islower(tablero.tablero[casilla][columnaActual + 1])) {
            jugadas->tablero[casilla][columnaActual + 1] = '2';  // Captura posible
        }

    }
}

// TORRE
void jugadasTorre(Tablero *jugadas){
    // Mover hacia arriba
    for(int i = filaActual - 1; i >= 0; i--){
        if(tablero.tablero[i][columnaActual] == '.'){ // Si la casilla está vacía
            jugadas->tablero[i][columnaActual] = '1'; // Se marca como movimiento válido con el 1
        } else if(turno && isupper(tablero.tablero[i][columnaActual])){ // Si es el turno de las blancas y hay una pieza negra
            jugadas->tablero[i][columnaActual] = '2'; // Se marca como captura posible con el 2
            break;
        } else if (!turno && islower(tablero.tablero[i][columnaActual])){ // Si es el turno de las negras y hay una pieza blanca
            jugadas->tablero[i][columnaActual] = '2'; // Se marca como captura posible con el 2
            break;
        } else { // Si hay una pieza del mismo color, sale del bucle, porque no puede saltar otra piezas
            break;
        }
    }

    // Mover hacia abajo
    for(int i = filaActual + 1; i < TAMANIO; i++){
        if(tablero.tablero[i][columnaActual] == '.'){ // Si la casilla está vacía
            jugadas->tablero[i][columnaActual] = '1'; // Se marca como movimiento válido con el 1
        } else if(turno && isupper(tablero.tablero[i][columnaActual])){ // Si es el turno de las blancas y hay una pieza negra
            jugadas->tablero[i][columnaActual] = '2'; // Se marca como captura posible con el 2
            break;
        } else if (!turno && islower(tablero.tablero[i][columnaActual])){ // Si es el turno de las negras y hay una pieza blanca
            jugadas->tablero[i][columnaActual] = '2';
            break;
        } else { // Si hay una pieza del mismo color, sale del bucle, porque no puede saltar otra piezas
            break;
        }
    }

    // Mover hacia la izquierda
    for(int j = columnaActual - 1; j >= 0; j--){  // Lo mismo, pero hacia la izquierda
        if(tablero.tablero[filaActual][j] == '.'){
            jugadas->tablero[filaActual][j] = '1';
        } else if(turno && isupper(tablero.tablero[filaActual][j])){
            jugadas->tablero[filaActual][j] = '2';
            break;
        } else if(!turno && islower(tablero.tablero[filaActual][j])){
            jugadas->tablero[filaActual][j] = '2';
            break;
        } else {
            break;
        }
    }

    // Mover hacia la derecha
    for(int j = columnaActual + 1; j < TAMANIO; j++){
        if(tablero.tablero[filaActual][j] == '.'){
            jugadas->tablero[filaActual][j] = '1';
        } else if(turno && isupper(tablero.tablero[filaActual][j])){
            jugadas->tablero[filaActual][j] = '2';
            break;
        } else if (!turno && islower(tablero.tablero[filaActual][j])){
            jugadas->tablero[filaActual][j] = '2';
            break;
        } else {
            break;
        }
    }
}

// CABALLO
void jugadasCaballo(Tablero *jugadas) {
    // Se hace un array con los movimientos posibles del caballo, que son en forma de L
    int movimientos[TAMANIO][2] = {{-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}}; // Es de 8 x 2, porque son 8 movimientos posibles
    
    for (int i = 0; i < TAMANIO; i++) {
        int x = filaActual + movimientos[i][0]; // Se calcula la fila a la que se moverá
        int y = columnaActual + movimientos[i][1]; // Se calcula la columna a la que se moverá
        
        if (x >= 0 && x < TAMANIO && y >= 0 && y < TAMANIO) { // Si esta dentro del rango el movimiento
            if (turno) { // Si es el turno de las blancas
                if (tablero.tablero[x][y] == '.' || isupper(tablero.tablero[x][y])) { // Si la casilla está vacía o hay una pieza negra
                    jugadas->tablero[x][y] = tablero.tablero[x][y] == '.' ? '1' : '2'; // Si está vacía, es un movimiento válido, si no, es una captura posible
                }
            } else { // Si es el turno de las negras
                if (tablero.tablero[x][y] == '.' || islower(tablero.tablero[x][y])) { // Lo mismo, pero con las negras
                    jugadas->tablero[x][y] = tablero.tablero[x][y] == '.' ? '1' : '2';
                }
            }
        }
    }
}

// ALFIL
void jugadasAlfil(Tablero *jugadas){
    // Mover hacia arriba e izquierda
    for(int i = filaActual - 1, j = columnaActual - 1; i >= 0 && j >= 0; i--, j--){ // Se mueve en diagonal hacia arriba y hacia la izquierda
        if(tablero.tablero[i][j] == '.'){ // Si la casilla está vacía
            jugadas->tablero[i][j] = '1'; // Se marca como movimiento válido con el 1
        } else if(turno && isupper(tablero.tablero[i][j])){ // Si es el turno de las blancas y hay una pieza negra
            jugadas->tablero[i][j] = '2'; // Es una posible captura
            break;
        } else if(!turno && islower(tablero.tablero[i][j])){ // Si es el turno de las negras y hay una pieza blanca
            jugadas->tablero[i][j] = '2'; // Es una posible captura
            break;
        } else { // Si hay una pieza del mismo color, sale del bucle, porque no puede saltar otra piezas
            break;
        }
    }

    // Después es lo mismo, pero en las otras tres direcciones
    // Mover hacia arriba y derecha
    for(int i = filaActual - 1, j = columnaActual + 1; i >= 0 && j < TAMANIO; i--, j++){
        if(tablero.tablero[i][j] == '.'){
            jugadas->tablero[i][j] = '1';
        } else if(turno && isupper(tablero.tablero[i][j])){ // Turno de blancas y pieza negra
            jugadas->tablero[i][j] = '2';
            break;
        } else if(!turno && islower(tablero.tablero[i][j])){ // Turno de negras y pieza blanca
            jugadas->tablero[i][j] = '2';
            break;
        } else {
            break;
        }
    }

    // Mover hacia abajo e izquierda
    for(int i = filaActual + 1, j = columnaActual - 1; i < TAMANIO && j >= 0; i++, j--){
        if(tablero.tablero[i][j] == '.'){
            jugadas->tablero[i][j] = '1';
        } else if(turno && isupper(tablero.tablero[i][j])){ // Turno de blancas y pieza negra
            jugadas->tablero[i][j] = '2';
            break;
        } else if(!turno && islower(tablero.tablero[i][j])){ // Turno de negras y pieza blanca
            jugadas->tablero[i][j] = '2';
            break;
        } else {
            break;
        }
    }

    // Mover hacia abajo y derecha
    for(int i = filaActual + 1, j = columnaActual + 1; i < TAMANIO && j < TAMANIO; i++, j++){
        if(tablero.tablero[i][j] == '.'){
            jugadas->tablero[i][j] = '1';
        } else if(turno && isupper(tablero.tablero[i][j])){ // Turno de blancas y pieza negra
            jugadas->tablero[i][j] = '2';
            break;
        } else if(!turno && islower(tablero.tablero[i][j])){ // Turno de negras y pieza blanca
            jugadas->tablero[i][j] = '2';
            break;
        } else {
            break;
        }
    }
}

// REINA
void jugadasReina(Tablero *jugadas){
    // La reina puede moverse en todas las direcciones, y combina los movimientos del alfil y la torre
    jugadasTorre(jugadas);
    jugadasAlfil(jugadas);
}

// REY
void jugadasRey(Tablero *jugadas) {
    // Se hace un array con los movimientos posibles del rey, que es solo una casilla a su alrededor
    int movimientos[TAMANIO][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}; // Es de 8 x 2, porque son 8 movimientos posibles

    for (int i = 0; i < TAMANIO; i++) {
        int x = filaActual + movimientos[i][0]; // Se calcula la fila a la que se moverá
        int y = columnaActual + movimientos[i][1]; // Se calcula la columna a la que se moverá

        if (x >= 0 && x < TAMANIO && y >= 0 && y < TAMANIO) {
            if (turno) { // Si es el turno de las blancas
                if (tablero.tablero[x][y] == '.' || isupper(tablero.tablero[x][y])) { // Si la casilla está vacía o hay una pieza negra
                    jugadas->tablero[x][y] = tablero.tablero[x][y] == '.' ? '1' : '2'; // Si está vacía, es un movimiento válido, si no, es una captura posible
                }
            } else { // Si es el turno de las negras
                if (tablero.tablero[x][y] == '.' || islower(tablero.tablero[x][y])) { // Lo mismo, pero con las negras
                    jugadas->tablero[x][y] = tablero.tablero[x][y] == '.' ? '1' : '2';
                }
            }
        }
    }

    // Se llama a la función enroque, para que se puedan hacer los movimientos de enroque, si es el caso
    enroque(jugadas);
}


bool moverPieza(Tablero *jugadas) {
    enroqueConf = 0; // Se pone en 0, para que no hayan problemas
    if (jugadas->tablero[fila][columna] == '1' || jugadas->tablero[fila][columna] == '2') {
        // Se verifica si se está intentando hacer un enroque

        // BLANCAS
        // Enroque corto blanco
        if (pieza == 'r' && fila == 7 && columna == 6 && tablero.tablero[7][4] == 'r' && tablero.tablero[7][7] == 't' && !enroqueBlancas) { // Si las piezas están en las posiciones correctas, sin piezas en el medio de ellas
            // Se realiza el cambio
            tablero.tablero[7][6] = 'r';
            tablero.tablero[7][5] = 't';
            tablero.tablero[7][4] = '.';
            tablero.tablero[7][7] = '.';
            enroqueConf = 1; // Se marca como enroque corto
            enroqueBlancas = true; // Y se indica que ya se realizó un enroque en las blancas, para que no se pueda hacer otro
        } // Enroque largo blanco 
        else if (pieza == 'r' && fila == 7 && columna == 2 && tablero.tablero[7][4] == 'r' && tablero.tablero[7][0] == 't' && !enroqueBlancas) { // Lo mismo, pero con el enroque largo
            // Se realiza el cambio
            tablero.tablero[7][2] = 'r';
            tablero.tablero[7][3] = 't';
            tablero.tablero[7][4] = '.';
            tablero.tablero[7][0] = '.';
            enroqueConf = 2; // Se marca como enroque largo
            enroqueBlancas = true; // Y se indica que ya se realizó un enroque en las blancas, para que no se pueda hacer otro
        } // NEGRAS
        else if (pieza == 'R' && fila == 0 && columna == 6 && tablero.tablero[0][4] == 'R' && tablero.tablero[0][7] == 'T' && !enroqueNegras) { // Lo mismo que con las blancas, pero con las negras
            // Enroque corto negro
            tablero.tablero[0][6] = 'R';
            tablero.tablero[0][5] = 'T';
            tablero.tablero[0][4] = '.';
            tablero.tablero[0][7] = '.';
            enroqueConf = 3; // Se marca como enroque corto, pero de negras
            enroqueNegras = true; // Y se indica que ya se realizo enroque en las negras, para que no se pueda hacer otro
        } // Enroque largo negro
        else if (pieza == 'R' && fila == 0 && columna == 2 && tablero.tablero[0][4] == 'R' && tablero.tablero[0][0] == 'T' && !enroqueNegras) { // Lo mismo que con las blancas
            tablero.tablero[0][2] = 'R';
            tablero.tablero[0][3] = 'T';
            tablero.tablero[0][4] = '.';
            tablero.tablero[0][0] = '.';
            enroqueConf = 4;
            enroqueNegras = true;
        } else {
            // Movimiento normal
             // Se intercambian las piezas, para que simule el movimiento, dependiendo de la pieza
            tablero.tablero[fila][columna] = tablero.tablero[filaActual][columnaActual];
            tablero.tablero[filaActual][columnaActual] = '.';
            if (coronacion()) {
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

// ----- JUGADAS EXTRA --------

// Coronar un peón
bool coronacion() {
    if (pieza == 'P' && fila == 7) { // Si un peón blanco llega a la última fila
        printf("Peón negro coronado, seleccione que pieza quiere reclamar (D, T, A, C): "); // Ingresa por teclado que pieza quiere reclamar
        scanf(" %c", &nuevaPieza);
        nuevaPieza = toupper(nuevaPieza); // Se pasa a minúscula, porque son piezas blancas
        tablero.tablero[fila][columna] = nuevaPieza; // Y se sustituye
        return true;
    } else if (pieza == 'p' && fila == 0) { // Lo mismo, pero con el peón negro
        printf("Peón blanco coronado, seleccione que pieza quiere reclamar (d, t, a, c): ");
        scanf(" %c", &nuevaPieza);
        nuevaPieza = tolower(nuevaPieza);
        tablero.tablero[fila][columna] = nuevaPieza;
        return true;
    }
    return false;
}

// Hacer enroque, largo o corto
bool enroque(Tablero *jugadas) {
    if (turno) { // Enroque para las blancas
        if (filaActual == 7 && columnaActual == 4) { // Si es la casilla del rey blanco
            // Enroque corto
            if (tablero.tablero[7][5] == '.' && tablero.tablero[7][6] == '.' && tablero.tablero[7][7] == 't' && !enroqueBlancas) { // Y las demás piezas están en su lugar, es decir, la torre, y no hay anda entre las dos piezas, y las blancas no hay realizado un enroque
                jugadas->tablero[7][6] = '1'; // Se marca como una posible jugada
            }
            // Enroque largo
            if (tablero.tablero[7][1] == '.' && tablero.tablero[7][2] == '.' && tablero.tablero[7][3] == '.' && tablero.tablero[7][0] == 't' && !enroqueBlancas) { // Lo mismo, pero con el enroque largo
                jugadas->tablero[7][2] = '1';
            }
        }
    } else { // Enroque para las negras
        if (filaActual == 0 && columnaActual == 4) {
            // Enroque corto
            if (tablero.tablero[0][5] == '.' && tablero.tablero[0][6] == '.' && tablero.tablero[0][7] == 'T' && !enroqueNegras) {
                jugadas->tablero[0][6] = '1';
            }
            // Enroque largo
            if (tablero.tablero[0][1] == '.' && tablero.tablero[0][2] == '.' && tablero.tablero[0][3] == '.' && tablero.tablero[0][0] == 'T' && !enroqueNegras) {
                jugadas->tablero[0][2] = '1';
            }
        }
    }
    return false;
}


bool tablas() {
    // Se evaluan tablas en caso de que no haya suficiente material para hacer jaque mate
    int reyes = 0, alfiles = 0, caballos = 0, peones = 0, reinas = 0, torres = 0;

    for (int i = 0; i < TAMANIO; i++) {
        for (int j = 0; j < TAMANIO; j++) {
            char pieza = tablero.tablero[i][j];
            if (pieza != '.') { 
                if (pieza == 'R' || pieza == 'r') {
                    reyes++; 
                } else if (pieza == 'A' || pieza == 'a') {
                    alfiles++;
                } else if (pieza == 'C' || pieza == 'c') {
                    caballos++; 
                } else if (pieza == 'P' || pieza == 'p') {
                    peones++;
                } else if (pieza == 'D' || pieza == 'd') {
                    reinas++;
                } else if (pieza == 'T' || pieza == 't') {
                    torres++;
                }
            }
        }
    }

    // Si solo quedan los reyes y ningún peón, torre, reina, alfil o caballo
    if (reyes == 2 && reinas == 0 && alfiles == 0 && caballos == 0 && peones == 0 && torres == 0) {
        printf("Tablas por insuficiencia de material: Rey contra Rey.\n");
        return true; 
    }

    // Si hay un alfil y dos reyes (uno blanco y uno negro)
    if (reyes == 2 && reinas == 0 && alfiles == 1 && caballos == 0 && peones == 0 && torres == 0) {
        printf("Tablas por insuficiencia de material: Rey y Alfil contra Rey.\n");
        return true; 
    }

    // Si hay un caballo y dos reyes
    if (reyes == 2 && reinas == 0 && alfiles == 0 && caballos == 1 && peones == 0 && torres == 0) {
        printf("Tablas por insuficiencia de material: Rey y Caballo contra Rey.\n");
        return true; 
    }

    return false;
}

bool jaqueMate(){
    int cont = 0;
    for(int i = 0; i < TAMANIO; i++){
        for(int j = 0; j < TAMANIO; j++){
            if(tablero.tablero[i][j] == 'r' || tablero.tablero[i][j] == 'R'){ // Se cuentan la cantidad de reyes que hay
                cont++;
            }
        }
    }
    if(cont <= 1){ // Si solo hay un rey, se acaba el juego
        return true;
    }
    return false;
}


//-------------Confirmaciones----------------
bool confirmacionColor(){
    // Se asegura que solo pueda mover las piezas de su color
    if(turno){
        if(isupper(pieza)){
            return false;
        } else {
            return true;
        }
    } else {
        if(islower(pieza)){
            return false;
        } else {
            return true;
        }
    }
}

int comprobarAmbiguedad() {
    // Se comprueba si hay ambigüedad en las piezas dobles
    int numPiezas = 2;
    // Se crean arrays para guardar las posiciones de las piezas, y luego comparar si estan en la misma fila o columna, ya que en esos casos la notación algebráica es distinta
    int fc[numPiezas];
    int cc[numPiezas];
    int ac[numPiezas];
    int af[numPiezas];
    int tc[numPiezas];
    int tf[numPiezas];
    int contcf = 0, contcc = 0, contaf = 0, contac = 0, conttf = 0, conttc = 0;
    char temp;

    for(int i = 0; i < TAMANIO; i++){
        for(int j = 0; j < TAMANIO; j++){
            temp = tablero.tablero[i][j];
            if(islower(temp)){
                if(temp == 'c'){
                    fc[contcf++] = i;
                    cc[contcc++] = j; 
                } else if (temp == 'a'){
                    ac[contaf++] = i;
                    af[contac++] = j;
                } else if (temp == 't'){
                    tc[conttf++] = i;
                    tf[conttc++] = j;
                }
            }
        }
    }

    // Comprobar si los caballos están en la misma fila o columna
    for(int i = 0; i < contcf; i++){
        for(int j = i + 1; j < contcf; j++){
            if(fc[i] == fc[j]){
                return 2; // Misma fila
            } else if(cc[i] == cc[j]){
                return 1; // Misma columna
            }
        }
    }

    // Comprobar si los alfiles están en la misma fila o columna
    for(int i = 0; i < contaf; i++){
        for(int j = i + 1; j < contaf; j++){
            if(ac[i] == ac[j]){
                return 2; // Misma fila
            } else if(af[i] == af[j]){
                return 1; // Misma columna
            }
        }
    }

    // Comprobar si las torres están en la misma fila o columna
    for(int i = 0; i < conttf; i++){
        for(int j = i + 1; j < conttf; j++){
            if(tc[i] == tc[j]){
                return 2; // Misma fila
            } else if(tf[i] == tf[j]){
                return 1; // Misma columna
            }
        }
    }

    return 0;
}



