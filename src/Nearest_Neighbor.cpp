#include "../headers/Heuristicas.hpp"
#include "../headers/Grafos.hpp"
#include <vector>
#include <numeric>
#include <random>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <fstream>

using namespace std;

const int INF_LOCAL = 0x3f3f3f3f;
#define endl '\n'

pair<double, int> Nearest_Neighbor(Grafo g){

    //Cria os vetores fora dos laços para economizar memória
    vector<int> dist; //vetor de distância para o dijkstra
    vector<int> pai; //vetor de pais
    vector<bool> pareados; //vetor de verificação dos vértices ímpares(se eles já foram "pareados")
    vector<int> ind;
    
    int custo_total = 0; 

    auto inicio = chrono::high_resolution_clock::now(); //Início do Cronômetro

    vector<int> vertices_impares = g.get_vertices_impares();

        //guardamos o número de vértices em uma variável para facilitar as coisas
        int num_vertices = g.get_n_vertices()+1;

        //Criando vector de indices
        ind.resize(vertices_impares.size());
        iota(ind.begin(), ind.end(), 0);

        // Criar um gerador de números aleatórios baseado no tempo atual
        random_device rd;
        mt19937 g_rand(rd());

        // Aleatoriza os índices usando o std::shuffle
        shuffle(ind.begin(), ind.end(), g_rand);
        
        //Reinicia os vetores auxiliares para cada iteração
        dist.assign(num_vertices, 0);
        pai.assign(num_vertices, 0);
        pareados.assign(num_vertices, false);

        //calculamos os possíveis caminhos a cada par de vértices ímpares
        for(auto it:ind){

            if(pareados[vertices_impares[it]]) continue;
            pair <int, int> melhor = {INF_LOCAL, -1};

            g.dijkstra(vertices_impares[it], dist, pai);
            for(auto itt:vertices_impares){
                if(vertices_impares[it] != itt and !pareados[itt] and dist[itt] < melhor.first) {
                    melhor = make_pair(dist[itt], itt);
                }
            }

            pareados[vertices_impares[it]] = 1;
            pareados[melhor.second] = 1;

            int atual = melhor.second;
            while(atual != vertices_impares[it]) {
                int anterior = pai[atual];
                int peso_real = g.get_peso(anterior, atual);
                
                custo_total += peso_real; 

                g.adicionar_aresta(anterior, atual, peso_real);
                atual = anterior;
            }
        }

    vector<int> ciclo = g.extrair_ciclo_euleriano(1);
    reverse(ciclo.begin(), ciclo.end());
    
    //lógica de tempo
    auto fim = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> tempo = fim - inicio;
    
    // Retorna o par contendo (Tempo em ms, Custo total do circuito)
    return make_pair(tempo.count(), custo_total);
} 
