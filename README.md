# Ajedrez en C con SDL2

Este proyecto es un juego de ajedrez interactivo desarrollado en lenguaje C usando la biblioteca gráfica SDL2, junto con SDL2_ttf para renderizar texto y SDL2_image para cargar imágenes.

Al ejecutar el programa, se abren dos ventanas independientes:

 - Una muestra el tablero de ajedrez con las piezas, permitiendo interactuar y mover las piezas.

 - La otra muestra en tiempo real la notación algebraica de cada jugada realizada.

El juego implementa las reglas clásicas del ajedrez, incluyendo enroques, coronación de peones, y tablas por material insuficiente.

## Características

- Tablero de ajedrez visual con piezas gráficas.
- Soporte para:
  - Movimientos legales.
  - Coronación de peones (con selección de pieza).
  - Enroque corto (O-O) y largo (O-O-O).
  - Tablas por material insuficiente.
- Planilla de notación algebraica en una ventana separada.
- Interfaz gráfica usando SDL2.
- Uso de fuentes Open Sans para la planilla.

## Capturas de pantalla
<img width="2083" height="1070" alt="image" src="https://github.com/user-attachments/assets/60e3ea6b-7d0f-4f45-adaf-ece694ee0d32" />


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
