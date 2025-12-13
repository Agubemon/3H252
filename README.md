# Solitario Navideño

Juego de cartas Solitario Klondike implementado en C++ con SFML.

---

## 🎮 Descripción del Proyecto

Solitario Navideño es una implementación del clásico juego de cartas Solitario (Klondike) desarrollado en C++ utilizando la biblioteca SFML para gráficos y sonido. El juego cuenta con una interfaz gráfica navideña, efectos de sonido y música de fondo.

### 🎯 Objetivo del Juego

El objetivo es mover todas las 52 cartas a las 4 pilas de fundación (foundation piles), ordenadas por palo desde el As hasta el Rey.

### 🎮 Controles

- **Mouse (Click Izquierdo)**: Seleccionar y arrastrar cartas
- **Mouse (Doble Click)**: Mover automáticamente una carta a las pilas de fundación si es posible
- **Click en Mazo**: Voltear carta del mazo al descarte
- **Botón BACK**: Regresar al menú principal
- **Botón CLOSE**: Cerrar el juego
- **Icono de Bocina**: Pausar/reanudar música

### ⚙️ Mecánicas

**Tableau (7 columnas del tablero):**
- Se pueden colocar cartas en orden descendente (K, Q, J, 10..., 2, A)
- Los colores deben alternarse (rojo sobre negro, negro sobre rojo)
- Solo se puede colocar un Rey en una columna vacía
- Las cartas se voltean automáticamente cuando quedan expuestas

**Foundation (4 pilas superiores):**
- Cada pila debe empezar con un As del mismo palo
- Las cartas deben apilarse en orden ascendente (A, 2, 3... K)
- Solo se aceptan cartas del mismo palo

**Mazo y Descarte:**
- Click en el mazo voltea cartas al descarte una por una
- Las cartas del descarte pueden moverse al tableau o foundation

### 🏆 Características

- Interfaz gráfica navideña personalizada
- Sistema de arrastre de cartas con validación de movimientos
- Detección automática de victoria
- Música de fondo y efectos de sonido
- Doble click para movimiento automático a fundaciones
- Pantalla de menú con botón de inicio

## 👥 Equipo

- **Líder**: [Nombre Completo] (@usuario-github)
- **Integrante 2**: [Nombre Completo] (@usuario-github)
- **Integrante 3**: [Nombre Completo] (@usuario-github)

## 🛠️ Tecnologías

- **Lenguaje**: C++17
- **Librería Gráfica**: SFML (Simple and Fast Multimedia Library)
- **Compilador**: GCC (MinGW64)
- **Sistema de compilación**: Make / Comandos manuales

### Instalación de SFML en MSYS2

```bash
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-sfml```

### Compilaci�n y Ejecuci�n

**PowerShell:**
```powershell
cd bin; g++ -std=c++17 -I../include -c ../src/Card.cpp -o Card.o; g++ -std=c++17 -I../include -c ../src/main.cpp -o main.o; g++ Card.o main.o -o solitaire.exe -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -mwindows; ./solitaire.exe
```

**MSYS2/Bash:**
```bash
cd bin && g++ -std=c++17 -I../include -c ../src/Card.cpp -o Card.o && g++ -std=c++17 -I../include -c ../src/main.cpp -o main.o && g++ Card.o main.o -o solitaire.exe -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -mwindows && ./solitaire.exe
```

##  Cr�ditos

### Assets de terceros utilizados

- **Sprites de cartas**: [Fuente]
- **Im�genes de fondo**: [Fuente]
- **Iconos**: [Fuente]

### Sonidos

- **M�sica de men�**: [Fuente]
- **M�sica de juego**: [Fuente]
- **Efectos de sonido**: [Fuente]

### Fuentes

- **Fuente de texto**: Maryson Rough (Solo uso personal) - https://www.mansgreback.com/

### Referencias o inspiraciones

- [Referencias]

### Agradecimientos

- [Agradecimientos]
