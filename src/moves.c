#include "moves.h"
#include <ctype.h>
#include <stdio.h>

void possibleMoves(Board *jugadas) { // Se pasan las jugadas posibles
    // Dependiendo de la pieza, se calculan las jugadas posibles
    if (piece == 'P' || piece == 'p') {
        pawnMoves(jugadas);
    } else if (piece == 'Q' || piece == 'q') {
        queenMoves(jugadas);
    } else if (piece == 'K' || piece == 'k') {
        kingMoves(jugadas);
    } else if (piece == 'N' || piece == 'n') {
        knightMoves(jugadas);
    } else if (piece == 'B' || piece == 'b') {
        bishopMoves(jugadas);
    } else if (piece == 'R' || piece == 'r') {
        rookMoves(jugadas);
    } else {
        printf("Seleccione una piece válida\n");
    }
}
// PEONES
void pawnMoves(Board *jugadas) {
    int sentido = (turn) ? -1 : 1;  // Si es turno de las blancas, el sentido es -1 para que vaya hacia arriba, si no, el sentido es 1 para que vayan hacia abajo
    int casilla = filaActual + sentido;     // Calcula la casilla hacia adelante según el sentido

    // Movimiento hacia adelante
    if (casilla >= 0 && casilla < TAMANIO && board.board[casilla][columnaActual] == '.') { // Si la casilla está vacía y dentro del rango
        jugadas->board[casilla][columnaActual] = '1';  // Casilla vacía, movimiento válido

        // Movimiento inicial de dos casillas
        if ((turn && filaActual == 6) || (!turn && filaActual == 1)) {  // Si es el turno de las blancas y está en fila 6, o turno el turno de las negras y está en la fila 1
            casilla = filaActual + 2 * sentido;  // Calcula la casilla dos espacios adelante
            if (casilla >= 0 && casilla < TAMANIO && board.board[casilla][columnaActual] == '.') { // Si la casilla está vacía y dentro del rango
                jugadas->board[casilla][columnaActual] = '1';  // Movimiento de dos casillas válido
            }
        }
    }

    if(turn){ // Si es el turno de las blancas

        // Captura en diagonal izquierda
        if (columnaActual - 1 >= 0 && casilla >= 0 && casilla < TAMANIO && board.board[casilla][columnaActual - 1] != '.' && isupper(board.board[casilla][columnaActual - 1])) { // Si la casilla no está vacía, está dentro del rango y es una pieza negra
            jugadas->board[casilla][columnaActual - 1] = '2';  // Lo marca como una captura posible con el 2
        }
    
        // Captura en diagonal derecha
        if (columnaActual + 1 < TAMANIO && casilla >= 0 && casilla < TAMANIO && board.board[casilla][columnaActual + 1] != '.' && isupper(board.board[casilla][columnaActual + 1])) { // Lo mismo, pero con la diagonal derecha
            jugadas->board[casilla][columnaActual + 1] = '2';  // Lo marca como una captura posible con el 2
        }

    } else { // Si es el turno de las negras
        // Captura en diagonal izquierda
        if (columnaActual - 1 >= 0 && casilla >= 0 && casilla < TAMANIO && board.board[casilla][columnaActual - 1] != '.' && islower(board.board[casilla][columnaActual - 1])) { // Si la casilla no está vacía, está dentro del rango y es una pieza blanca
            jugadas->board[casilla][columnaActual - 1] = '2';  // Lo marca como una captura posible con el 2
        }
    
        // Captura en diagonal derecha
        if (columnaActual + 1 < TAMANIO && casilla >= 0 && casilla < TAMANIO && board.board[casilla][columnaActual + 1] != '.' && islower(board.board[casilla][columnaActual + 1])) {
            jugadas->board[casilla][columnaActual + 1] = '2';  // Captura posible
        }

    }
}

// TORRE
void rookMoves(Board *jugadas){
    // Mover hacia arriba
    for(int i = filaActual - 1; i >= 0; i--){
        if(board.board[i][columnaActual] == '.'){ // Si la casilla está vacía
            jugadas->board[i][columnaActual] = '1'; // Se marca como movimiento válido con el 1
        } else if(turn && isupper(board.board[i][columnaActual])){ // Si es el turno de las blancas y hay una pieza negra
            jugadas->board[i][columnaActual] = '2'; // Se marca como captura posible con el 2
            break;
        } else if (!turn && islower(board.board[i][columnaActual])){ // Si es el turno de las negras y hay una pieza blanca
            jugadas->board[i][columnaActual] = '2'; // Se marca como captura posible con el 2
            break;
        } else { // Si hay una pieza del mismo color, sale del bucle, porque no puede saltar otra piezas
            break;
        }
    }

    // Mover hacia abajo
    for(int i = filaActual + 1; i < TAMANIO; i++){
        if(board.board[i][columnaActual] == '.'){ // Si la casilla está vacía
            jugadas->board[i][columnaActual] = '1'; // Se marca como movimiento válido con el 1
        } else if(turn && isupper(board.board[i][columnaActual])){ // Si es el turno de las blancas y hay una pieza negra
            jugadas->board[i][columnaActual] = '2'; // Se marca como captura posible con el 2
            break;
        } else if (!turn && islower(board.board[i][columnaActual])){ // Si es el turno de las negras y hay una pieza blanca
            jugadas->board[i][columnaActual] = '2';
            break;
        } else { // Si hay una pieza del mismo color, sale del bucle, porque no puede saltar otra piezas
            break;
        }
    }

    // Mover hacia la izquierda
    for(int j = columnaActual - 1; j >= 0; j--){  // Lo mismo, pero hacia la izquierda
        if(board.board[filaActual][j] == '.'){
            jugadas->board[filaActual][j] = '1';
        } else if(turn && isupper(board.board[filaActual][j])){
            jugadas->board[filaActual][j] = '2';
            break;
        } else if(!turn && islower(board.board[filaActual][j])){
            jugadas->board[filaActual][j] = '2';
            break;
        } else {
            break;
        }
    }

    // Mover hacia la derecha
    for(int j = columnaActual + 1; j < TAMANIO; j++){
        if(board.board[filaActual][j] == '.'){
            jugadas->board[filaActual][j] = '1';
        } else if(turn && isupper(board.board[filaActual][j])){
            jugadas->board[filaActual][j] = '2';
            break;
        } else if (!turn && islower(board.board[filaActual][j])){
            jugadas->board[filaActual][j] = '2';
            break;
        } else {
            break;
        }
    }
}

// CABALLO
void knightMoves(Board *jugadas) {
    // Se hace un array con los movimientos posibles del caballo, que son en forma de L
    int movimientos[TAMANIO][2] = {{-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}}; // Es de 8 x 2, porque son 8 movimientos posibles
    
    for (int i = 0; i < TAMANIO; i++) {
        int x = filaActual + movimientos[i][0]; // Se calcula la fila a la que se moverá
        int y = columnaActual + movimientos[i][1]; // Se calcula la columna a la que se moverá
        
        if (x >= 0 && x < TAMANIO && y >= 0 && y < TAMANIO) { // Si esta dentro del rango el movimiento
            if (turn) { // Si es el turno de las blancas
                if (board.board[x][y] == '.' || isupper(board.board[x][y])) { // Si la casilla está vacía o hay una pieza negra
                    jugadas->board[x][y] = board.board[x][y] == '.' ? '1' : '2'; // Si está vacía, es un movimiento válido, si no, es una captura posible
                }
            } else { // Si es el turno de las negras
                if (board.board[x][y] == '.' || islower(board.board[x][y])) { // Lo mismo, pero con las negras
                    jugadas->board[x][y] = board.board[x][y] == '.' ? '1' : '2';
                }
            }
        }
    }
}

// ALFIL
void bishopMoves(Board *jugadas){
    // Mover hacia arriba e izquierda
    for(int i = filaActual - 1, j = columnaActual - 1; i >= 0 && j >= 0; i--, j--){ // Se mueve en diagonal hacia arriba y hacia la izquierda
        if(board.board[i][j] == '.'){ // Si la casilla está vacía
            jugadas->board[i][j] = '1'; // Se marca como movimiento válido con el 1
        } else if(turn && isupper(board.board[i][j])){ // Si es el turno de las blancas y hay una pieza negra
            jugadas->board[i][j] = '2'; // Es una posible captura
            break;
        } else if(!turn && islower(board.board[i][j])){ // Si es el turno de las negras y hay una pieza blanca
            jugadas->board[i][j] = '2'; // Es una posible captura
            break;
        } else { // Si hay una pieza del mismo color, sale del bucle, porque no puede saltar otra piezas
            break;
        }
    }

    // Después es lo mismo, pero en las otras tres direcciones
    // Mover hacia arriba y derecha
    for(int i = filaActual - 1, j = columnaActual + 1; i >= 0 && j < TAMANIO; i--, j++){
        if(board.board[i][j] == '.'){
            jugadas->board[i][j] = '1';
        } else if(turn && isupper(board.board[i][j])){ // Turno de blancas y pieza negra
            jugadas->board[i][j] = '2';
            break;
        } else if(!turn && islower(board.board[i][j])){ // Turno de negras y pieza blanca
            jugadas->board[i][j] = '2';
            break;
        } else {
            break;
        }
    }

    // Mover hacia abajo e izquierda
    for(int i = filaActual + 1, j = columnaActual - 1; i < TAMANIO && j >= 0; i++, j--){
        if(board.board[i][j] == '.'){
            jugadas->board[i][j] = '1';
        } else if(turn && isupper(board.board[i][j])){ // Turno de blancas y pieza negra
            jugadas->board[i][j] = '2';
            break;
        } else if(!turn && islower(board.board[i][j])){ // Turno de negras y pieza blanca
            jugadas->board[i][j] = '2';
            break;
        } else {
            break;
        }
    }

    // Mover hacia abajo y derecha
    for(int i = filaActual + 1, j = columnaActual + 1; i < TAMANIO && j < TAMANIO; i++, j++){
        if(board.board[i][j] == '.'){
            jugadas->board[i][j] = '1';
        } else if(turn && isupper(board.board[i][j])){ // Turno de blancas y pieza negra
            jugadas->board[i][j] = '2';
            break;
        } else if(!turn && islower(board.board[i][j])){ // Turno de negras y pieza blanca
            jugadas->board[i][j] = '2';
            break;
        } else {
            break;
        }
    }
}

// REINA
void queenMoves(Board *jugadas){
    // La reina puede moverse en todas las direcciones, y combina los movimientos del alfil y la torre
    rookMoves(jugadas);
    bishopMoves(jugadas);
}

// REY
void kingMoves(Board *jugadas) {
    // Se hace un array con los movimientos posibles del rey, que es solo una casilla a su alrededor
    int movimientos[TAMANIO][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}; // Es de 8 x 2, porque son 8 movimientos posibles

    for (int i = 0; i < TAMANIO; i++) {
        int x = filaActual + movimientos[i][0]; // Se calcula la fila a la que se moverá
        int y = columnaActual + movimientos[i][1]; // Se calcula la columna a la que se moverá

        if (x >= 0 && x < TAMANIO && y >= 0 && y < TAMANIO) {
            if (turn) { // Si es el turno de las blancas
                if (board.board[x][y] == '.' || isupper(board.board[x][y])) { // Si la casilla está vacía o hay una pieza negra
                    jugadas->board[x][y] = board.board[x][y] == '.' ? '1' : '2'; // Si está vacía, es un movimiento válido, si no, es una captura posible
                }
            } else { // Si es el turno de las negras
                if (board.board[x][y] == '.' || islower(board.board[x][y])) { // Lo mismo, pero con las negras
                    jugadas->board[x][y] = board.board[x][y] == '.' ? '1' : '2';
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
    if (piece == 'P' && fila == 7) { // Si un peón blanco llega a la última fila
        printf("Peón negro coronado, seleccione que piece quiere reclamar (Q, R, B, N): "); // Ingresa por teclado que pieza quiere reclamar
        scanf(" %c", &nuevaPieza);
        nuevaPieza = toupper(nuevaPieza); // Se pasa a minúscula, porque son piezas blancas
        board.board[fila][columna] = nuevaPieza; // Y se sustituye
        return true;
    } else if (piece == 'p' && fila == 0) { // Lo mismo, pero con el peón negro
        printf("Peón blanco coronado, seleccione que piece quiere reclamar (q, r, b, n): ");
        scanf(" %c", &nuevaPieza);
        nuevaPieza = tolower(nuevaPieza);
        board.board[fila][columna] = nuevaPieza;
        return true;
    }
    return false;
}

// Hacer enroque, largo o corto
bool castling(Board *jugadas) {
    if (turn) { // Enroque para las blancas
        if (filaActual == 7 && columnaActual == 4) { // Si es la casilla del rey blanco
            // Enroque corto
            if (board.board[7][5] == '.' && board.board[7][6] == '.' && board.board[7][7] == 'r' && !enroqueBlancas) { // Y las demás piezas están en su lugar, es decir, la torre, y no hay anda entre las dos piezas, y las blancas no hay realizado un enroque
                jugadas->board[7][6] = '1'; // Se marca como una posible jugada
            }
            // Enroque largo
            if (board.board[7][1] == '.' && board.board[7][2] == '.' && board.board[7][3] == '.' && board.board[7][0] == 'r' && !enroqueBlancas) { // Lo mismo, pero con el enroque largo
                jugadas->board[7][2] = '1';
            }
        }
    } else { // Enroque para las negras
        if (filaActual == 0 && columnaActual == 4) {
            // Enroque corto
            if (board.board[0][5] == '.' && board.board[0][6] == '.' && board.board[0][7] == 'R' && !enroqueNegras) {
                jugadas->board[0][6] = '1';
            }
            // Enroque largo
            if (board.board[0][1] == '.' && board.board[0][2] == '.' && board.board[0][3] == '.' && board.board[0][0] == 'R' && !enroqueNegras) {
                jugadas->board[0][2] = '1';
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
            char piece = board.board[i][j];
            if (piece != '.') { 
                if (piece == 'K' || piece == 'k') {
                    reyes++; 
                } else if (piece == 'B' || piece == 'b') {
                    alfiles++;
                } else if (piece == 'N' || piece == 'n') {
                    caballos++; 
                } else if (piece == 'P' || piece == 'p') {
                    peones++;
                } else if (piece == 'Q' || piece == 'q') {
                    reinas++;
                } else if (piece == 'R' || piece == 'r') {
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
            if(board.board[i][j] == 'k' || board.board[i][j] == 'K'){ // Se cuentan la cantidad de reyes que hay
                cont++;
            }
        }
    }
    if(cont <= 1){ // Si solo hay un rey, se acaba el juego
        return true;
    }
    return false;
}