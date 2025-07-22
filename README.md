# Chess in C with SDL2

This project is an interactive chess game developed in C using the SDL2 graphics library, along with SDL2_ttf for text rendering and SDL2_image for loading images.

When the program runs, two separate windows are opened:

- One displays the chessboard with the pieces, allowing for interaction and piece movement.
- The other shows the algebraic notation of each move in real time.

The game implements classical chess rules, including castling, pawn promotion, and draws due to insufficient material.

## Features

- Graphical chessboard with piece images.
- Support for:
  - Legal moves.
  - Pawn promotion (with piece selection).
  - Short castling (O-O) and long castling (O-O-O).
  - Draws due to insufficient material.
- Separate window displaying algebraic notation of the moves.
- Graphical interface using SDL2.
- Uses Open Sans fonts for the notation sheet.

## Screenshots
<img width="2083" height="1070" alt="image" src="https://github.com/user-attachments/assets/60e3ea6b-7d0f-4f45-adaf-ece694ee0d32" />

## Requirements

- [SDL2](https://www.libsdl.org/)
- [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)
- [SDL2_image](https://www.libsdl.org/projects/SDL_image/)
- GCC compiler (recommended: MSYS2 environment on Windows)

## Project Structure

```
.
├── src/ # Main source code
├── assets/ # Graphic and font resources
│ ├── images/pieces/ # Piece images
│ └── fonts/ # Open Sans fonts
├── build/ # Build files (Makefile)
└── bin/ # Generated binaries
```

## Compilation

To compile the project, use the included Makefile:

```sh
cd build
make
```
The executable will be generated in the bin/ folder as main.exe.

## Execution

Run the generated binary:

```sh
./bin/main.exe
```

## Font License

The included Open Sans fonts are licensed under the SIL Open Font License 1.1.
