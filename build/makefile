# Variables
CC = gcc
CFLAGS = $(shell sdl2-config --cflags)
LIBS = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf
SRC = ajedrez.c
OBJ = ajedrez.o
OUT = a

# Regla para compilar el programa
$(OUT): $(SRC)
	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LIBS)

# Regla para compilar el archivo fuente .c a .o (si se agrega más código fuente en el futuro)
$(OBJ): $(SRC)
	$(CC) -c $(SRC) $(CFLAGS)

# Limpiar archivos generados
clean:
	rm -f $(OBJ) $(OUT)

