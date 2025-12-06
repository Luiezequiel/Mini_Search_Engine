# Compilador
CXX = g++

# Flags de compilación
CXXFLAGS = -Wall -Wextra -std=c++17

# Nombre del ejecutable
TARGET = search_engine

# Archivos fuente
SOURCES = main.cpp SearchEngine.cpp

# Archivos objeto
OBJECTS = $(SOURCES:.cpp=.o)

# Regla principal: compilar todo
all: $(TARGET)

# Crear el ejecutable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compilar archivos .cpp a .o
%.o: %.cpp SearchEngine.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -f $(OBJECTS) $(TARGET)

# Ejecutar el programa
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
