#include <bits/stdc++.h>
#include "../include/grafo.hpp"

using namespace std;

#define endl '\n'

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n = 8;
    int m = 12;

    Grafo teste(n, "GrafoAleatorio");
    
    srand(time(NULL)); 
    teste.gerar_grafo_aleatorio(n, m);

    cout << "Grafo gerado com " << n << " vertices e " << m << " arestas." << endl;

    vector<int> vertices_impares = teste.get_vertices_impares();

    if(vertices_impares.empty()){
        cout << "O grafo ja eh Euleriano." << endl;
    }else{
        vector<int> dist(teste.get_n_vertices()+1);
        vector<int> pai(teste.get_n_vertices()+1);

        vector<pair<int, pair<int, int>>> arestas_virtuais;

        for(auto it:vertices_impares){
            teste.dijkstra(it, dist, pai);
            for(auto itt:vertices_impares){
                if(it<itt){
                    arestas_virtuais.push_back({dist[itt], {it, itt}});
                }
            }
        }

        sort(arestas_virtuais.begin(), arestas_virtuais.end());

        vector<bool> pareados(teste.get_n_vertices()+1, false);

        for(auto it:arestas_virtuais){
            if(pareados[it.second.first] == false && pareados[it.second.second] == false){
                pareados[it.second.first] = true;
                pareados[it.second.second] = true;

                cout<<"Formou par: "<<it.second.first<<" e "<<it.second.second<<" com custo "<<it.first<<endl;

                teste.dijkstra(it.second.first, dist, pai);

                int atual=it.second.second;
                while(atual!=it.second.first){
                    int anterior=pai[atual];
                    int peso_real = teste.get_peso(anterior, atual);
                    teste.adicionar_aresta(anterior, atual, peso_real);
                    atual=anterior;
                }
            }
        }
    }

    vector<int> ciclo = teste.extrair_ciclo_euleriano(1);
    reverse(ciclo.begin(), ciclo.end());

    cout << "circuito euleriano" << endl;
    for(size_t i=0; i<ciclo.size(); ++i) {
        cout<<ciclo[i]<<(i==ciclo.size()-1 ? "" : " -> ");
    }
    cout << endl;

    return 0;
}