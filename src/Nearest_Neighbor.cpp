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
    vector<pair<int, pair<int, int>>> arestas_virtuais; //vetor de arestas virtuais para os pares de vértices ímpares
    vector<bool> pareados; //vetor de verificação dos vértices ímpares(se eles já foram "pareados")
    vector<int> ind;
    
    // VARIÁVEL PARA ACUMULAR O CUSTO EXTRA DAS ARESTAS DUPLICADAS
    int custo_total = 0; 

    auto inicio = chrono::high_resolution_clock::now(); //lógica de tempo

    //pegamos o vetor de vértices de grau ímpar
    vector<int> vertices_impares = g.get_vertices_impares();

    //se o vertices_impares não estiver vazio, o grafo não é euleriano, segue a lógica
    if(vertices_impares.size()){
        //guardamos o número de vértices em uma variável para facilitar as coisas
        int num_vertices = g.get_n_vertices()+1;

        //Criando vector de indices
        ind.resize(vertices_impares.size());
        iota(ind.begin(), ind.end(), 0);

        // Criar um gerador de números aleatórios baseado no tempo atual
        random_device rd;
        mt19937 g_rand(rd());

        // Aleatoriza usando o std::shuffle
        shuffle(ind.begin(), ind.end(), g_rand);
        
        //zera os vetores criados como preparação para cada iteração
        dist.assign(num_vertices, 0);
        pai.assign(num_vertices, 0);
        pareados.assign(num_vertices, false);
        arestas_virtuais.clear();

        //lógica para estimar a quantidade de arestas virtuais e reservar espaço no vetor arestas_virtuais
        long long k = vertices_impares.size();
        //combinação dois a dois entre eles
        arestas_virtuais.reserve((k*(k-1))/2);

        //calculamos os possíveis caminhos a cada par de vértices ímpares
        for(auto it:ind){

            if(pareados[vertices_impares[it]]) continue;
            pair <int, int> melhor = {INF_LOCAL, -1};

            //usamos o dijkstra para preenchermos os vetores dist e pai
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
                
                // ACUMULA O CUSTO DA ARESTA DUPLICADA
                custo_total += peso_real; 

                g.adicionar_aresta(anterior, atual, peso_real);
                atual = anterior;
            }
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
