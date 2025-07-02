#include "moves.h"
#include "board.h"
#include "globals.h"
#include <ctype.h>
#include <stdio.h>

void possibleMoves(Tablero *jugadas) { // Se pasan las jugadas posibles
    // Dependiendo de la pieza, se calculan las jugadas posibles
    if (pieza == 'P' || pieza == 'p') {
        pawnMoves(jugadas);
    } else if (pieza == 'D' || pieza == 'd') {
        queenMoves(jugadas);
    } else if (pieza == 'R' || pieza == 'r') {
        kingMoves(jugadas);
    } else if (pieza == 'C' || pieza == 'c') {
        knightMoves(jugadas);
    } else if (pieza == 'A' || pieza == 'a') {
        bishopMoves(jugadas);
    } else if (pieza == 'T' || pieza == 't') {
        rookMoves(jugadas);
    } else {
        printf("Seleccione una pieza válida\n");
    }
}
// PEONES
void pawnMoves(Tablero *jugadas) {
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
void rookMoves(Tablero *jugadas){
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
void knightMoves(Tablero *jugadas) {
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
void bishopMoves(Tablero *jugadas){
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
void queenMoves(Tablero *jugadas){
    // La reina puede moverse en todas las direcciones, y combina los movimientos del alfil y la torre
    rookMoves(jugadas);
    bishopMoves(jugadas);
}

// REY
void kingMoves(Tablero *jugadas) {
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
    castling(jugadas);
}

// ----- JUGADAS EXTRA --------

// Coronar un peón
bool promotion() {
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
bool castling(Tablero *jugadas) {
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


bool stalemate() {
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

bool checkmate(){
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