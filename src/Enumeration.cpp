#include <vector>
#include <algorithm>
#include <chrono>
#include "../headers/Grafos.hpp"
using namespace std;

int melhor_custo;
int bitmask;

void solve(size_t index, int custo, const vector<int>& vertices_impares, const vector<vector<int>>& matriz_dist) {
    size_t n = vertices_impares.size();
    
    if (custo >= melhor_custo) return;

    // .test(index) checa se o bit está ligado (true)
    while (index < n && (bitmask & (1 << index))) {
        index++;
    }

    if (index == n) {
        melhor_custo = custo;
        return;
    }

    // .set(index) liga o bit do elemento atual
    bitmask |= (1 << index);

    for (size_t i = index + 1; i < n; i++) {
        if (!(bitmask & (1 << i)) ) {
            bitmask |= (1 << i); // O(1) no nível do processador
            
            solve(index + 1, custo + matriz_dist[index][i], vertices_impares, matriz_dist);
            
	    bitmask &= (~(1 << i));// .reset(i) desliga o bit (backtrack)
        }
    }

    bitmask &= (~(1 << index));
}

pair<double, int> Enumeration(Grafo g) {
    auto inicio = chrono::high_resolution_clock::now();
    
    vector<int> vertices_impares = g.get_vertices_impares();
    melhor_custo = 0x3f3f3f3f;

        size_t k = vertices_impares.size();
        int num_vertices = g.get_n_vertices() + 1;

        vector<vector<int>> matriz_dist(k, vector<int>(k, 0));
        
        for (size_t i = 0; i < k; i++) {
            vector<int> dist(num_vertices);
            vector<int> pai(num_vertices);
            g.dijkstra(vertices_impares[i], dist, pai);
            
            for (size_t j = 0; j < k; j++) {
                matriz_dist[i][j] = dist[vertices_impares[j]];
            }
        }

	bitmask = 0; // Garante que todos os bits começam zerados
        solve(0, 0, vertices_impares, matriz_dist);

    auto fim = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> tempo = fim - inicio;

    return make_pair(tempo.count(), melhor_custo);
}
