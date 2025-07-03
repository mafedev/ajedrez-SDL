# Compilador
CC = gcc

# Rutas de los headers y libs de SDL en MSYS2
CFLAGS = -IC:/msys64/mingw64/include/SDL2 -Wall
LDFLAGS = -LC:/msys64/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image

# Archivos fuente y destino
SRC = ../src/main.c ../src/windows.c ../src/pieces.c ../src/board.c ../src/moves.c ../src/utils.c ../src/globals.c ../src/fonts.c
OUTDIR = ../bin
OUT = $(OUTDIR)/main.exe

# Regla principal
all: $(OUT)

$(OUT): $(SRC)
	mkdir -p $(OUTDIR)
	$(CC) $(SRC) $(CFLAGS) $(LDFLAGS) -o $(OUT)

# Limpiar binarios
clean:
	rm -f $(OUT)