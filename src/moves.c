#include "moves.h"
#include <ctype.h>
#include <stdio.h>

void possibleMoves(Board *moves) { // Se pasan las moves posibles
    // Dependiendo de la pieza, se calculan las moves posibles
    if (piece == 'P' || piece == 'p') {
        pawnMoves(moves);
    } else if (piece == 'Q' || piece == 'q') {
        queenMoves(moves);
    } else if (piece == 'K' || piece == 'k') {
        kingMoves(moves);
    } else if (piece == 'N' || piece == 'n') {
        knightMoves(moves);
    } else if (piece == 'B' || piece == 'b') {
        bishopMoves(moves);
    } else if (piece == 'R' || piece == 'r') {
        rookMoves(moves);
    } else {
        printf("Seleccione una piece válida\n");
    }
}
// PEONES
void pawnMoves(Board *moves) {
    int sentido = (turn) ? -1 : 1;  // Si es turno de las blancas, el sentido es -1 para que vaya hacia arriba, si no, el sentido es 1 para que vayan hacia abajo
    int casilla = currentRow + sentido;     // Calcula la casilla hacia adelante según el sentido

    // Movimiento hacia adelante
    if (casilla >= 0 && casilla < TAMANIO && board.board[casilla][currentColumn] == '.') { // Si la casilla está vacía y dentro del rango
        moves->board[casilla][currentColumn] = '1';  // Casilla vacía, movimiento válido

        // Movimiento inicial de dos casillas
        if ((turn && currentRow == 6) || (!turn && currentRow == 1)) {  // Si es el turno de las blancas y está en fila 6, o turno el turno de las negras y está en la fila 1
            casilla = currentRow + 2 * sentido;  // Calcula la casilla dos espacios adelante
            if (casilla >= 0 && casilla < TAMANIO && board.board[casilla][currentColumn] == '.') { // Si la casilla está vacía y dentro del rango
                moves->board[casilla][currentColumn] = '1';  // Movimiento de dos casillas válido
            }
        }
    }

    if(turn){ // Si es el turno de las blancas

        // Captura en diagonal izquierda
        if (currentColumn - 1 >= 0 && casilla >= 0 && casilla < TAMANIO && board.board[casilla][currentColumn - 1] != '.' && isupper(board.board[casilla][currentColumn - 1])) { // Si la casilla no está vacía, está dentro del rango y es una pieza negra
            moves->board[casilla][currentColumn - 1] = '2';  // Lo marca como una captura posible con el 2
        }
    
        // Captura en diagonal derecha
        if (currentColumn + 1 < TAMANIO && casilla >= 0 && casilla < TAMANIO && board.board[casilla][currentColumn + 1] != '.' && isupper(board.board[casilla][currentColumn + 1])) { // Lo mismo, pero con la diagonal derecha
            moves->board[casilla][currentColumn + 1] = '2';  // Lo marca como una captura posible con el 2
        }

    } else { // Si es el turno de las negras
        // Captura en diagonal izquierda
        if (currentColumn - 1 >= 0 && casilla >= 0 && casilla < TAMANIO && board.board[casilla][currentColumn - 1] != '.' && islower(board.board[casilla][currentColumn - 1])) { // Si la casilla no está vacía, está dentro del rango y es una pieza blanca
            moves->board[casilla][currentColumn - 1] = '2';  // Lo marca como una captura posible con el 2
        }
    
        // Captura en diagonal derecha
        if (currentColumn + 1 < TAMANIO && casilla >= 0 && casilla < TAMANIO && board.board[casilla][currentColumn + 1] != '.' && islower(board.board[casilla][currentColumn + 1])) {
            moves->board[casilla][currentColumn + 1] = '2';  // Captura posible
        }

    }
}

// TORRE
void rookMoves(Board *moves){
    // Mover hacia arriba
    for(int i = currentRow - 1; i >= 0; i--){
        if(board.board[i][currentColumn] == '.'){ // Si la casilla está vacía
            moves->board[i][currentColumn] = '1'; // Se marca como movimiento válido con el 1
        } else if(turn && isupper(board.board[i][currentColumn])){ // Si es el turno de las blancas y hay una pieza negra
            moves->board[i][currentColumn] = '2'; // Se marca como captura posible con el 2
            break;
        } else if (!turn && islower(board.board[i][currentColumn])){ // Si es el turno de las negras y hay una pieza blanca
            moves->board[i][currentColumn] = '2'; // Se marca como captura posible con el 2
            break;
        } else { // Si hay una pieza del mismo color, sale del bucle, porque no puede saltar otra piezas
            break;
        }
    }

    // Mover hacia abajo
    for(int i = currentRow + 1; i < TAMANIO; i++){
        if(board.board[i][currentColumn] == '.'){ // Si la casilla está vacía
            moves->board[i][currentColumn] = '1'; // Se marca como movimiento válido con el 1
        } else if(turn && isupper(board.board[i][currentColumn])){ // Si es el turno de las blancas y hay una pieza negra
            moves->board[i][currentColumn] = '2'; // Se marca como captura posible con el 2
            break;
        } else if (!turn && islower(board.board[i][currentColumn])){ // Si es el turno de las negras y hay una pieza blanca
            moves->board[i][currentColumn] = '2';
            break;
        } else { // Si hay una pieza del mismo color, sale del bucle, porque no puede saltar otra piezas
            break;
        }
    }

    // Mover hacia la izquierda
    for(int j = currentColumn - 1; j >= 0; j--){  // Lo mismo, pero hacia la izquierda
        if(board.board[currentRow][j] == '.'){
            moves->board[currentRow][j] = '1';
        } else if(turn && isupper(board.board[currentRow][j])){
            moves->board[currentRow][j] = '2';
            break;
        } else if(!turn && islower(board.board[currentRow][j])){
            moves->board[currentRow][j] = '2';
            break;
        } else {
            break;
        }
    }

    // Mover hacia la derecha
    for(int j = currentColumn + 1; j < TAMANIO; j++){
        if(board.board[currentRow][j] == '.'){
            moves->board[currentRow][j] = '1';
        } else if(turn && isupper(board.board[currentRow][j])){
            moves->board[currentRow][j] = '2';
            break;
        } else if (!turn && islower(board.board[currentRow][j])){
            moves->board[currentRow][j] = '2';
            break;
        } else {
            break;
        }
    }
}

// CABALLO
void knightMoves(Board *moves) {
    // Se hace un array con los movimientos posibles del caballo, que son en forma de L
    int movimientos[TAMANIO][2] = {{-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}}; // Es de 8 x 2, porque son 8 movimientos posibles
    
    for (int i = 0; i < TAMANIO; i++) {
        int x = currentRow + movimientos[i][0]; // Se calcula la fila a la que se moverá
        int y = currentColumn + movimientos[i][1]; // Se calcula la columna a la que se moverá
        
        if (x >= 0 && x < TAMANIO && y >= 0 && y < TAMANIO) { // Si esta dentro del rango el movimiento
            if (turn) { // Si es el turno de las blancas
                if (board.board[x][y] == '.' || isupper(board.board[x][y])) { // Si la casilla está vacía o hay una pieza negra
                    moves->board[x][y] = board.board[x][y] == '.' ? '1' : '2'; // Si está vacía, es un movimiento válido, si no, es una captura posible
                }
            } else { // Si es el turno de las negras
                if (board.board[x][y] == '.' || islower(board.board[x][y])) { // Lo mismo, pero con las negras
                    moves->board[x][y] = board.board[x][y] == '.' ? '1' : '2';
                }
            }
        }
    }
}

// ALFIL
void bishopMoves(Board *moves){
    // Mover hacia arriba e izquierda
    for(int i = currentRow - 1, j = currentColumn - 1; i >= 0 && j >= 0; i--, j--){ // Se mueve en diagonal hacia arriba y hacia la izquierda
        if(board.board[i][j] == '.'){ // Si la casilla está vacía
            moves->board[i][j] = '1'; // Se marca como movimiento válido con el 1
        } else if(turn && isupper(board.board[i][j])){ // Si es el turno de las blancas y hay una pieza negra
            moves->board[i][j] = '2'; // Es una posible captura
            break;
        } else if(!turn && islower(board.board[i][j])){ // Si es el turno de las negras y hay una pieza blanca
            moves->board[i][j] = '2'; // Es una posible captura
            break;
        } else { // Si hay una pieza del mismo color, sale del bucle, porque no puede saltar otra piezas
            break;
        }
    }

    // Después es lo mismo, pero en las otras tres direcciones
    // Mover hacia arriba y derecha
    for(int i = currentRow - 1, j = currentColumn + 1; i >= 0 && j < TAMANIO; i--, j++){
        if(board.board[i][j] == '.'){
            moves->board[i][j] = '1';
        } else if(turn && isupper(board.board[i][j])){ // Turno de blancas y pieza negra
            moves->board[i][j] = '2';
            break;
        } else if(!turn && islower(board.board[i][j])){ // Turno de negras y pieza blanca
            moves->board[i][j] = '2';
            break;
        } else {
            break;
        }
    }

    // Mover hacia abajo e izquierda
    for(int i = currentRow + 1, j = currentColumn - 1; i < TAMANIO && j >= 0; i++, j--){
        if(board.board[i][j] == '.'){
            moves->board[i][j] = '1';
        } else if(turn && isupper(board.board[i][j])){ // Turno de blancas y pieza negra
            moves->board[i][j] = '2';
            break;
        } else if(!turn && islower(board.board[i][j])){ // Turno de negras y pieza blanca
            moves->board[i][j] = '2';
            break;
        } else {
            break;
        }
    }

    // Mover hacia abajo y derecha
    for(int i = currentRow + 1, j = currentColumn + 1; i < TAMANIO && j < TAMANIO; i++, j++){
        if(board.board[i][j] == '.'){
            moves->board[i][j] = '1';
        } else if(turn && isupper(board.board[i][j])){ // Turno de blancas y pieza negra
            moves->board[i][j] = '2';
            break;
        } else if(!turn && islower(board.board[i][j])){ // Turno de negras y pieza blanca
            moves->board[i][j] = '2';
            break;
        } else {
            break;
        }
    }
}

// REINA
void queenMoves(Board *moves){
    // La reina puede moverse en todas las direcciones, y combina los movimientos del alfil y la torre
    rookMoves(moves);
    bishopMoves(moves);
}

// REY
void kingMoves(Board *moves) {
    // Se hace un array con los movimientos posibles del rey, que es solo una casilla a su alrededor
    int movimientos[TAMANIO][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}; // Es de 8 x 2, porque son 8 movimientos posibles

    for (int i = 0; i < TAMANIO; i++) {
        int x = currentRow + movimientos[i][0]; // Se calcula la fila a la que se moverá
        int y = currentColumn + movimientos[i][1]; // Se calcula la columna a la que se moverá

        if (x >= 0 && x < TAMANIO && y >= 0 && y < TAMANIO) {
            if (turn) { // Si es el turno de las blancas
                if (board.board[x][y] == '.' || isupper(board.board[x][y])) { // Si la casilla está vacía o hay una pieza negra
                    moves->board[x][y] = board.board[x][y] == '.' ? '1' : '2'; // Si está vacía, es un movimiento válido, si no, es una captura posible
                }
            } else { // Si es el turno de las negras
                if (board.board[x][y] == '.' || islower(board.board[x][y])) { // Lo mismo, pero con las negras
                    moves->board[x][y] = board.board[x][y] == '.' ? '1' : '2';
                }
            }
        }
    }

    // Se llama a la función enroque, para que se puedan hacer los movimientos de enroque, si es el caso
    castling(moves);
}

// ----- JUGADAS EXTRA --------

// Coronar un peón
bool promotion() {
    if (piece == 'P' && row == 7) { // Si un peón blanco llega a la última fila
        printf("Peón negro coronado, seleccione que piece quiere reclamar (Q, R, B, N): "); // Ingresa por teclado que pieza quiere reclamar
        scanf(" %c", &newPiece);
        newPiece = toupper(newPiece); // Se pasa a minúscula, porque son piezas blancas
        board.board[row][column] = newPiece; // Y se sustituye
        return true;
    } else if (piece == 'p' && row == 0) { // Lo mismo, pero con el peón negro
        printf("Peón blanco coronado, seleccione que piece quiere reclamar (q, r, b, n): ");
        scanf(" %c", &newPiece);
        newPiece = tolower(newPiece);
        board.board[row][column] = newPiece;
        return true;
    }
    return false;
}

// Hacer enroque, largo o corto
bool castling(Board *moves) {
    if (turn) { // Enroque para las blancas
        if (currentRow == 7 && currentColumn == 4) { // Si es la casilla del rey blanco
            // Enroque corto
            if (board.board[7][5] == '.' && board.board[7][6] == '.' && board.board[7][7] == 'r' && !whiteCastling) { // Y las demás piezas están en su lugar, es decir, la torre, y no hay anda entre las dos piezas, y las blancas no hay realizado un enroque
                moves->board[7][6] = '1'; // Se marca como una posible jugada
            }
            // Enroque largo
            if (board.board[7][1] == '.' && board.board[7][2] == '.' && board.board[7][3] == '.' && board.board[7][0] == 'r' && !whiteCastling) { // Lo mismo, pero con el enroque largo
                moves->board[7][2] = '1';
            }
        }
    } else { // Enroque para las negras
        if (currentRow == 0 && currentColumn == 4) {
            // Enroque corto
            if (board.board[0][5] == '.' && board.board[0][6] == '.' && board.board[0][7] == 'R' && !blackCastling) {
                moves->board[0][6] = '1';
            }
            // Enroque largo
            if (board.board[0][1] == '.' && board.board[0][2] == '.' && board.board[0][3] == '.' && board.board[0][0] == 'R' && !blackCastling) {
                moves->board[0][2] = '1';
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