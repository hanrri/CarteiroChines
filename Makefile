# Compilador e flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude

# Nome do executável final
TARGET = programa

# Regra principal
all: $(TARGET)

# Como construir o executável
$(TARGET): src/main.cpp include/grafo.hpp
	$(CXX) $(CXXFLAGS) src/main.cpp -o $(TARGET)

# Regra para limpar os arquivos gerados (útil antes de subir pro GitHub)
clean:
	rm -f $(TARGET)