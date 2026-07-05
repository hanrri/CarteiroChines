#include "../headers/Grafos.hpp"
#include <queue>
#include <algorithm>
#include <stack>

// Usar o namespace aqui no .cpp é seguro porque o escopo é local a este arquivo
using namespace std;

Grafo::Grafo(int vertices, string nome, bool direcionado, bool tem_peso, bool permite_multiplas) {
    this->n_vertices = vertices;
    this->nome = nome;
    adj.resize(vertices + 1);
}

bool Grafo::adicionar_aresta(int u, int v, int peso) {
    if (u < 1 || u > n_vertices || v < 1 || v > n_vertices) return false;
    adj[u].push_back({v, peso});
    adj[v].push_back({u, peso});
    return true;
}

vector<int> Grafo::get_vertices_impares() {
    vector<int> impares;
    for (int i = 1; i <= n_vertices; i++) {
        if (adj[i].size() % 2 != 0) impares.push_back(i);
    }
    return impares;
}

void Grafo::dijkstra(int origem, vector<int>& dist, vector<int>& pai) {
    dist.assign(n_vertices + 1, INF);
    pai.assign(n_vertices + 1, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[origem] = 0;
    pq.push({0, origem});

    while (!pq.empty()) {
        auto [d, atual] = pq.top();
        pq.pop();
        if (d > dist[atual]) continue;
        for (auto [vizinho, peso] : adj[atual]) {
            if (dist[vizinho] > dist[atual] + peso) {
                dist[vizinho] = dist[atual] + peso;
                pai[vizinho] = atual;
                pq.push({dist[vizinho], vizinho});
            }
        }
    }
}

int Grafo::get_peso(int u, int v) {
    for (auto [vizinho, peso] : adj[u]) {
        if (vizinho == v) return peso;
    }
    return 1;
}

void Grafo::excluir_aresta(int u, int v, int peso_alvo) {
    for (auto it = adj[u].begin(); it != adj[u].end(); ++it) {
        if (it->first == v && it->second == peso_alvo) {
            adj[u].erase(it);
            break;
        }
    }
    for (auto it = adj[v].begin(); it != adj[v].end(); ++it) {
        if (it->first == u && it->second == peso_alvo) {
            adj[v].erase(it);
            break;
        }
    }
}

vector<int> Grafo::extrair_ciclo_euleriano(int inicio) {
    vector<int> circuito;
    stack<int> pilha;
    pilha.push(inicio);
    while (!pilha.empty()) {
        int u = pilha.top();
        if (!adj[u].empty()) {
            int v = adj[u].back().first;
            int peso = adj[u].back().second;
            excluir_aresta(u, v, peso);
            pilha.push(v);
        } else {
            circuito.push_back(u);
            pilha.pop();
        }
    }
    return circuito;
}

int Grafo::gerar_grafo_aleatorio(int n, int m) {
    int peso_total = 0;
    for (int i = 1; i < n; i++) {
	int peso = rand() % 10 + 1;
	peso_total += peso;
	adicionar_aresta(i, i + 1, peso); 
    }
    for (int i = 0; i < m - (n - 1); i++) {
        int u = rand() % n + 1;
        int v = rand() % n + 1;
        if (u != v) {
	   int peso = rand() % 10 + 1;
	   peso_total += peso;
	   adicionar_aresta(u, v, peso);
	}
    }
    return peso_total;
}
