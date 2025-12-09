# Nombre del ejecutable final
TARGET = solitario

# Directorios de Código Fuente
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = bin

# Compilador y Banderas
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I$(INCLUDE_DIR)
LDFLAGS = -L/usr/lib -L/mingw64/lib

# Librerías SFML y Box2D para MSYS2
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lBox2D

# Obtiene todos los archivos .cpp del directorio src
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)

# Define los archivos objeto (ej: src/main.cpp -> bin/main.o)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

# Regla por defecto: 'make' construye el target
.PHONY: all
all: $(BUILD_DIR) $(TARGET)

# Regla para crear el directorio bin si no existe
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Regla principal: Enlazar (linkear) los archivos objeto para crear el ejecutable
$(TARGET): $(OBJECTS)
	@echo "Enlazando el ejecutable: $@"
	@$(CXX) $(OBJECTS) -o $@ $(LDFLAGS) $(LIBS)

# Regla para compilar archivos .cpp en archivos .o (Archivos objeto)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compilando: $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para limpiar los archivos compilados (.o y el ejecutable)
.PHONY: clean
clean:
	@echo "Limpiando archivos compilados..."
	@rm -f $(BUILD_DIR)/*.o $(TARGET)
	@rmdir --ignore-fail-on-non-empty $(BUILD_DIR)