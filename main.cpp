#include <iostream>
//#include "GameManager.h"
//#include "Card.h"
#include "Deck.h"
#include "Pile.h"

// Dimensiones de la ventana
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

// Esta macro es necesaria para que SDL funcione correctamente con MinGW
//#undef main 

int main(int argc, char* argv[]) {
    // 1. Inicialización de la Ventana (SDL2)
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL no pudo inicializarse. Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Configuración de OpenGL (Versión 3.3, común para proyectos 3D)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    // Creación de la ventana
    SDL_Window* window = SDL_CreateWindow(
        "Solitario 3D - Proyecto",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Fallo al crear la ventana SDL. Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Creación del Contexto OpenGL (la "pizarra" 3D)
    SDL_GLContext context = SDL_GL_CreateContext(window);
    
    // 2. Inicialización del Cerebro del Juego
    GameManager gameManager;
    
    // Reparte las cartas iniciales en el tablero
    std::cout << "Iniciando juego..." << std::endl;
    gameManager.initializeBoard();
    std::cout << "Tablero listo. ¡A jugar!" << std::endl;

    // 3. Game Loop (Ciclo Principal del Juego)
    bool running = true;
    SDL_Event event;

    while (running) {
        // Manejo de Eventos (Mouse, Teclado)
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false; // El usuario cerró la ventana
            }
            // Aquí iría la lógica para detectar clics de mouse sobre las cartas 3D
        }

        // 4. Lógica de Renderizado 3D (Dibujo)
        glClearColor(0.08f, 0.39f, 0.19f, 1.0f); // Color verde oscuro de fondo
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // *** Aquí es donde comenzarías a llamar a tus funciones OpenGL ***
        // Por ejemplo: dibujar el tapete, dibujar los modelos 3D de las cartas.
        // La información de las cartas está en gameManager.getTableauPiles(), etc.

        // 5. Mostrar la Ventana
        SDL_GL_SwapWindow(window); // Muestra lo que dibujaste en pantalla
    }

    // Limpieza al cerrar
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}