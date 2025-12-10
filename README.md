# Solitario 3D (Proyecto Final de Estructuras)

Juego de cartas Solitario implementado en C++ con capacidades 3D utilizando la librería SDL2 y OpenGL.

---

## 🛠️ Requisitos del Entorno (MSYS2 MINGW64)

Para compilar y ejecutar este proyecto, es necesario tener instalado el entorno **MSYS2 MINGW64** con el compilador GCC y las siguientes librerías de desarrollo:

* **SDL2 (Simple DirectMedia Layer)**
* **GLEW (OpenGL Extension Wrangler)**
* **OpenGL**

### Instalación de Librerías en MSYS2

Si las librerías SDL2 y GLEW no están instaladas, se pueden añadir usando el administrador de paquetes `pacman`:

```bash
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-SDL2
pacman -S mingw-w64-x86_64-glew