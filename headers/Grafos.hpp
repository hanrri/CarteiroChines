#pragma once
#include <vector>
#include <string>

const int INF = 0x3f3f3f3f;

class Grafo {
private:
    int n_vertices;
    std::string nome;
    std::vector<std::vector<std::pair<int, int>>> adj;

    // Método auxiliar privado 
    void excluir_aresta(int u, int v, int peso_alvo);

public:
    // Construtor
    Grafo(int vertices, std::string nome, bool direcionado = false, bool tem_peso = false, bool permite_multiplas = true);

    // Getters
    int get_n_vertices() const { return n_vertices; }
    int get_peso(int u, int v);
    std::vector<int> get_vertices_impares();

    // Algoritmos e Métodos Estruturais
    bool adicionar_aresta(int u, int v, int peso = 1);
    void dijkstra(int origem, std::vector<int>& dist, std::vector<int>& pai);
    std::vector<int> extrair_ciclo_euleriano(int inicio);
    int gerar_grafo_aleatorio(int n, int m);
};
