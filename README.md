# Solitario Navideno

Juego de cartas Solitario Klondike implementado en C++ con SFML.

---

## Descripcion del Proyecto

Solitario Navideno es una implementacion del clasico juego de cartas Solitario (Klondike) desarrollado en C++ utilizando la biblioteca SFML para graficos y sonido. El juego cuenta con una interfaz grafica navidena, efectos de sonido y musica de fondo.

### Objetivo del Juego

El objetivo es mover todas las 52 cartas a las 4 pilas de fundacion (foundation piles), ordenadas por palo desde el As hasta el Rey.

### Controles

- **Mouse (Click Izquierdo)**: Seleccionar y arrastrar cartas
- **Mouse (Doble Click)**: Mover automaticamente una carta a las pilas de fundacion si es posible
- **Click en Mazo**: Voltear carta del mazo al descarte
- **Boton BACK**: Regresar al menu principal
- **Boton CLOSE**: Cerrar el juego
- **Icono de Bocina**: Pausar/reanudar musica

### Mecanicas

**Tableau (7 columnas del tablero):**
- Se pueden colocar cartas en orden descendente (K, Q, J, 10..., 2, A)
- Los colores deben alternarse (rojo sobre negro, negro sobre rojo)
- Solo se puede colocar un Rey en una columna vacia
- Las cartas se voltean automaticamente cuando quedan expuestas

**Foundation (4 pilas superiores):**
- Cada pila debe empezar con un As del mismo palo
- Las cartas deben apilarse en orden ascendente (A, 2, 3... K)
- Solo se aceptan cartas del mismo palo

**Mazo y Descarte:**
- Click en el mazo voltea cartas al descarte una por una
- Las cartas del descarte pueden moverse al tableau o foundation

### Caracteristicas

- Interfaz grafica navidena personalizada
- Sistema de arrastre de cartas con validacion de movimientos
- Deteccion automatica de victoria
- Musica de fondo y efectos de sonido
- Doble click para movimiento automatico a fundaciones
- Pantalla de menu con boton de inicio

## Equipo

- **Lider**: Belen Abigail Aguilar Flores (@Agubemon)
- **Integrante 2**: Jose Maria Cervantes Salas (@usuario-github)

## Tecnologias

- **Lenguaje**: C++17
- **Libreria Grafica**: SFML (Simple and Fast Multimedia Library)
- **Compilador**: GCC (MinGW64)
- **Sistema de compilacion**: Make / Comandos manuales

### Instalacion de SFML en MSYS2

```bash
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-sfml
```

### Compilacion y Ejecucion

**PowerShell:**
```powershell
cd bin; g++ -std=c++17 -I../include -c ../src/Card.cpp -o Card.o; g++ -std=c++17 -I../include -c ../src/main.cpp -o main.o; g++ Card.o main.o -o solitaire.exe -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -mwindows; ./solitaire.exe
```

**MSYS2/Bash:**
```bash
cd bin && g++ -std=c++17 -I../include -c ../src/Card.cpp -o Card.o && g++ -std=c++17 -I../include -c ../src/main.cpp -o main.o && g++ Card.o main.o -o solitaire.exe -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -mwindows && ./solitaire.exe
```

## Creditos

### Assets de terceros utilizados

- **Sprites de cartas**: [Fuente]
- **Imagenes de fondo**: [Fuente]
- **Iconos**: [Fuente]

### Sonidos

- **Musica de menu**: [Fuente]
- **Musica de juego**: [Fuente]
- **Efectos de sonido**: [Fuente]

### Fuentes

- **Fuente de texto**: Maryson Rough (Solo uso personal) - https://www.mansgreback.com/

### Referencias o inspiraciones

- [Referencias]

### Agradecimientos

- [Agradecimientos]