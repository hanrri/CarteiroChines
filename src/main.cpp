#include <bits/stdc++.h>
#include <fstream>
#include "../include/grafo.hpp"

using namespace std;

#define endl '\n'

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    srand(time(NULL));

    vector<int> tamanhos_gulosos={50, 100, 500, 1000, 5000, 10000};
    //já começam a demorar muito
    //50000, 100000

    ofstream arquivo_json("estatisticas_gulosas.json");
    arquivo_json << "[\n";
    bool primeiro_registro = true;
    
    //testes gulosos
    for(auto n:tamanhos_gulosos){
        double soma_tempo=0;

        for(int i=0; i<10; i++){
            Grafo g(n, "teste");
            g.gerar_grafo_aleatorio(n, n*2);

            auto inicio=chrono::high_resolution_clock::now();

            vector<int> vertices_impares = g.get_vertices_impares();
                        
            if(!vertices_impares.empty()){
                vector<int> dist(g.get_n_vertices()+1);
                vector<int> pai(g.get_n_vertices()+1);
        
                vector<pair<int, pair<int, int>>> arestas_virtuais;
        
                for(auto it:vertices_impares){
                    g.dijkstra(it, dist, pai);
                    for(auto itt:vertices_impares){
                        if(it<itt){
                            arestas_virtuais.push_back({dist[itt], {it, itt}});
                        }
                    }
                }
        
                sort(arestas_virtuais.begin(), arestas_virtuais.end());
        
                vector<bool> pareados(g.get_n_vertices()+1, false);
        
                for(auto it:arestas_virtuais){
                    if(pareados[it.second.first] == false && pareados[it.second.second] == false){
                        pareados[it.second.first] = true;
                        pareados[it.second.second] = true;
                
                        g.dijkstra(it.second.first, dist, pai);
        
                        int atual=it.second.second;
                        while(atual!=it.second.first){
                            int anterior=pai[atual];
                            int peso_real = g.get_peso(anterior, atual);
                            g.adicionar_aresta(anterior, atual, peso_real);
                            atual=anterior;
                        }
                    }
                }
            }
        
            vector<int> ciclo = g.extrair_ciclo_euleriano(1);
            reverse(ciclo.begin(), ciclo.end());

            auto fim = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> tempo = fim-inicio;
            soma_tempo+=tempo.count();
        }

        double tempo_medio = soma_tempo/10.0;
        cout<<"N = "<<n<<" | Tempo Medio: "<<tempo_medio<<" ms"<<endl;

        if(!primeiro_registro) arquivo_json << ",\n";
        arquivo_json << "  {\n";
        arquivo_json << "    \"n_vertices\": " << n << ",\n";
        arquivo_json << "    \"tempo_medio_ms\": " << tempo_medio << "\n";
        arquivo_json << "  }";
        primeiro_registro = false;

        if(tempo_medio>600000){
            cout<<"Passou de 10 minutos. Abortado!!!"<<endl;
            break;
        }
    }

    arquivo_json << "\n]\n";
    arquivo_json.close();

    cout << "estatística_gulosas.json gerado." << endl;

    return 0;
}