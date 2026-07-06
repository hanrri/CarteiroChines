# Nome do compilador
CXX = g++

# Flags de otimização extrema e padrão do C++
CXXFLAGS = -O3 -Wall -std=c++17

# Lista de todos os arquivos fonte da pasta src automaticamente
SRCS = $(wildcard src/*.cpp)

# Nome do executável final
TARGET = carteiro_chines

# Regra principal (o que roda ao digitar apenas 'make')
all: $(TARGET)

# Como o executável deve ser construído
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

# Regra para compilar e imediatamente rodar os testes em C++
run: $(TARGET)
	./$(TARGET)

# Regra para chamar o Python e gerar os gráficos
plot:
	python generate_graphs/main.py

# Regra para rodar o ciclo completo: C++ gera os dados e Python plota
pipeline: run plot

# Limpa o executável para manter a pasta organizada
clean:
	rm -f $(TARGET)