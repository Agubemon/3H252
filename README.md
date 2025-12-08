# SOLITARIO

Juego de cartas

---

## Instrucciones rápidas

### Requisitos

* Un compilador C++ (g++, clang) y `make`.
* SFML (bibliotecas de desarrollo). En Ubuntu/Debian:

```
sudo apt install libsfml-dev build-essential
```

---

### Compilar y ejecutar (línea de comando)

```
cd /ruta/al/repositorio/SOLITARIO
make
./solitario
```

---

### Ejecutar con script (compila si es necesario)

```
bash scripts/run.sh
```

---

### Abrir en Visual Studio Code

* Abre la carpeta `SOLITARIO` en VS Code.
* `Terminal` → `Run Task...` → `Build (make)` para compilar.
* `Terminal` → `Run Task...` → `Run Solitario` para ejecutar.

---

### Notas

* Si ejecutas dentro de un contenedor sin servidor X (Codespaces/WSL sin X), la ventana gráfica no aparecerá.
  Ejecuta el juego en tu máquina local o usa VNC/X forwarding.
* Si quieres, se puede hacer un commit con estos cambios y dejar todo listo en la rama actual.
