#include <bits/stdc++.h>
#include <fstream>
//inclui a biblioteca de grafos capada
#include "../headers/Grafos.hpp"
#include "../headers/Heuristicas.hpp"

using namespace std;

#define endl '\n'

pair<double, int> Greedy(Grafo g){

    //cria os vetores fora dos laços para economizar memória
    vector<int> dist; //vetor de distância para o dijkstra
    vector<int> pai; //vetor de pais
    vector<pair<int, pair<int, int>>> arestas_virtuais; //vetor de arestas virtuais para os pares de vértices ímpares
    vector<bool> pareados; //vetor de verificação dos vértices ímpares(se eles já foram "pareados")
    
    // VARIÁVEL PARA ACUMULAR O CUSTO TOTAL (Começa com o peso do grafo original)
    // Nota: Se sua classe não tiver get_peso_total(), comece com 0 e mude a lógica do grafo.
    int custo_total = 0; 

    auto inicio = chrono::high_resolution_clock::now(); //lógica de tempo

    //pegamos o vetor de vértices de grau ímpar
    vector<int> vertices_impares = g.get_vertices_impares();

    //se o vertices_impares não estiver vazio, o grafo não é euleriano, segue a lógica
    if(!vertices_impares.empty()){
        //guardamos o número de vértices em uma variável para facilitar as coisas
        int num_vertices = g.get_n_vertices()+1;
        
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
        for(auto it:vertices_impares){
            //usamos o dijkstra para preenchermos os vetores dist e pai
            g.dijkstra(it, dist, pai);
            //percorremos o vetor de vértices ímpares para preenchermos o vetor de arestas virtuais
            for(auto itt:vertices_impares){
                if(it < itt){
                    arestas_virtuais.push_back({dist[itt], {it, itt}});
                }
            }
        }
  
        //ordenamos as arestas virtuais da "mais leve" para a "mais pesada"
        sort(arestas_virtuais.begin(), arestas_virtuais.end());
        
        //vamos percorrer o vetor de arestas virtuais para coletarmos todos esses melhores caminhos
        for(auto it:arestas_virtuais){
            if(pareados[it.second.first] == false && pareados[it.second.second] == false){
                pareados[it.second.first] = true;
                pareados[it.second.second] = true;
        
                g.dijkstra(it.second.first, dist, pai);
        
                //aqui finalmente iremos duplicar as arestas que compõe esse caminho do par
                int atual = it.second.second;
                while(atual != it.second.first){
                    int anterior = pai[atual];
                    int peso_real = g.get_peso(anterior, atual);
                    
                    // ACUMULA O CUSTO DA ARESTA DUPLICADA
                    custo_total += peso_real; 

                    g.adicionar_aresta(anterior, atual, peso_real);
                    atual = anterior;
                }
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
