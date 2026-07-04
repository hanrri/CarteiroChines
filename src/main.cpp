#include <bits/stdc++.h>
#include "../include/grafo.hpp"

using namespace std;

#define endl '\n'

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    Grafo teste(8, "GrafoTeste", false, true, true);

    teste.adicionar_aresta(1, 2, 2);
    teste.adicionar_aresta(1, 3, 5);
    teste.adicionar_aresta(2, 4, 1);
    teste.adicionar_aresta(3, 4, 3);
    teste.adicionar_aresta(4, 5, 2);
    teste.adicionar_aresta(5, 6, 4);
    teste.adicionar_aresta(6, 7, 1);
    teste.adicionar_aresta(7, 8, 3);
    teste.adicionar_aresta(8, 2, 2);
    teste.adicionar_aresta(1, 8, 6);

    vector<int> vertices_impares = teste.get_vertices_impares();

    bool primeiro = true;
    for(auto it : vertices_impares) {
        if(!primeiro) cout << " ";
        cout << it;
        primeiro = false;
    }
    cout << endl;

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

    vector<int> ciclo = teste.extrair_ciclo_euleriano(1);
    reverse(ciclo.begin(), ciclo.end());

    cout << "circuito euleriano" << endl;
    for(size_t i=0; i<ciclo.size(); ++i) {
        cout<<ciclo[i]<<(i==ciclo.size()-1 ? "" : " -> ");
    }
    cout << endl;

    return 0;
}