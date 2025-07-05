#include "utils.h"
#include <stdbool.h>
#include <ctype.h>

bool confirmacionColor(){
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
                if(temp == 'n'){
                    fc[contcf++] = i;
                    cc[contcc++] = j; 
                } else if (temp == 'b'){
                    ac[contaf++] = i;
                    af[contac++] = j;
                } else if (temp == 'r'){
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