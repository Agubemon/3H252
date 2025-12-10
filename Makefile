TARGET = bin/JuegoProyecto.exe

SRCS = main.cpp Deck.cpp Pile.cpp GameManager.cpp
OBJS = $(SRCS:.cpp=.o)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

SDL_LIBS = -lSDL2
GL_LIBS = -lglew32 -lopengl32
LIBS = $(SDL_LIBS) $(GL_LIBS)

INCLUDES = -I/usr/include/SDL2

# ==========================================================
# REGLAS DE COMPILACIÓN
# ==========================================================

$(TARGET): $(OBJS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ==========================================================
# LIMPIAR ARCHIVOS
# ==========================================================
clean:
	rm -f *.o *.exe *.out