# Ajedrez en C con SDL2

Este proyecto es un juego de ajedrez desarrollado en C utilizando la biblioteca gráfica SDL2, junto con SDL2_ttf para fuentes y SDL2_image para imágenes. Permite jugar partidas de ajedrez con interfaz gráfica, mostrando el tablero, piezas y una planilla de notación de jugadas.

## Características

- Tablero de ajedrez visual con piezas gráficas.
- Soporte para:
  - Movimientos legales.
  - Coronación.
  - Enroque.
  - Tablas por material insuficiente.
- Planilla de notación algebraica en una ventana separada.
- Interfaz gráfica usando SDL2.
- Uso de fuentes Open Sans para la planilla.

## Requisitos

- [SDL2](https://www.libsdl.org/)
- [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)
- [SDL2_image](https://www.libsdl.org/projects/SDL_image/)
- Compilador GCC (recomendado: entorno MSYS2 en Windows)

## Estructura del Proyecto

```
.
├── src/           # Código fuente principal (main.c)
├── assets/        # Recursos gráficos y fuentes
│   ├── images/pieces/  # Imágenes de las piezas
│   └── fonts/          # Fuentes Open Sans
├── build/         # Archivos de construcción (Makefile)
└── bin/           # Binarios generados
```

## Compilación

Para compilar el proyecto, usa el Makefile incluido:

```sh
cd build
make
```

El ejecutable se generará en la carpeta `bin/` como `main.exe`.

## Ejecución

Ejecuta el binario generado:

```sh
./bin/main.exe
```

## Licencia de Fuentes

Las fuentes Open Sans incluidas están bajo la [SIL Open Font License 1.1](https://scripts.sil.org/OFL).

---

Desarrollado como proyecto de ajedrez en C con SDL2.