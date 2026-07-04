#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <stack>

using namespace std;

const int INF = 0x3f3f3f3f;

class Grafo{
    private:
    // =======================================================
    // 1. ATRIBUTOS
    // =======================================================
    int n_vertices;
    string nome;
    bool direcionado;
    bool tem_peso;
    bool permite_multiplas;
    vector<vector<pair<int, int>>> adj;
    vector<vector<pair<int, int>>> inv_adj;
    vector<bool> visitados;

    // =======================================================
    // 2. MÉTODOS AUXILIARES (HELPERS PRIVADOS)
    // =======================================================
    int dfs(int vertice){
        if(visitados[vertice]) return 0;
        visitados[vertice] = true;

        int tamanho_ilha = 1;
        for(auto [vizinho, peso] : adj[vertice]){
            tamanho_ilha += dfs(vizinho);
        }

        return tamanho_ilha;
    }

    int dfs_componente(int vertice, vector<int>& componente_atual){
        if(visitados[vertice]) return 0;
        visitados[vertice] = true;

        componente_atual.push_back(vertice);

        int tamanho_ilha = 1;
        for(auto [vizinho, peso] : adj[vertice]){
            int funcao = dfs_componente(vizinho,componente_atual);
            tamanho_ilha += funcao;
        }

        return tamanho_ilha;
    }

    void dfs_ciclos_n_arestas(int vertice, int& qtd_vertices, int& qtd_arestas){
        visitados[vertice] = true;

        qtd_vertices++;
        qtd_arestas+=adj[vertice].size();

        for(auto [vizinho, peso] : adj[vertice]){
            if(!visitados[vizinho]){
                dfs_ciclos_n_arestas(vizinho, qtd_vertices, qtd_arestas);
            }
        }
    }

    bool dfs_ciclos_percorrendo(int vertice, int pai){
        visitados[vertice] = true;

        for(auto [vizinho, peso] : adj[vertice]){
            if(!visitados[vizinho]){
                if(dfs_ciclos_percorrendo(vizinho, vertice)){
                    return true;
                }
            }else if(vizinho!=pai){
                return true;
            }
        }

        return false;
    }

    bool dfs_bipartido(int vertice, int cor, vector<int>& coloracao){
        visitados[vertice] = true;
        coloracao[vertice] = cor;
        
        int prox_cor=cor^1;

        for(auto [vizinho, peso] : adj[vertice]){
            if(!visitados[vizinho]){
                if(!dfs_bipartido(vizinho, prox_cor, coloracao)){
                    return false;
                }
            }else{
                if(coloracao[vizinho]==coloracao[vertice]){
                    return false;
                }
            }
        }

        return true;
    }

    void dfs_kosaraju_1(int vertice, stack<int>& ordem){
        if(visitados[vertice]) return;
        visitados[vertice] = true;

        for(auto [vizinho, peso] : adj[vertice]){
            dfs_kosaraju_1(vizinho, ordem);
        }

        ordem.push(vertice);
        return;
    }

    void dfs_kosaraju_2(int vertice, vector<int>& componentes, int n_componentes){
        if(visitados[vertice]) return;
        visitados[vertice] = true;

        componentes[vertice] = n_componentes;

        for(auto [vizinho, peso] : inv_adj[vertice]){
            dfs_kosaraju_2(vizinho, componentes, n_componentes);
        }

        return;
    }

    void bfs(int vertice, vector<int>& dist, vector<int>& pai){
        queue<int> fila;
        dist[vertice]=0;
        fila.push(vertice);

        while(!fila.empty()){
            int atual = fila.front();
            fila.pop();

            for(auto [vizinho, peso] : adj[atual]){
                if(dist[vizinho]==-1){
                    dist[vizinho]=dist[atual]+1;
                    pai[vizinho]=atual;
                    fila.push(vizinho);
                }
            }
        }
    }

    public:
    // =======================================================
    // 3. CONSTRUTOR E INFORMAÇÕES BÁSICAS
    // =======================================================
    Grafo(int vertices, string palavra, bool direcionado = false, bool tem_peso = false, bool permite_multiplas = true){
        this->n_vertices = vertices;
        this->nome = palavra;
        this->direcionado = direcionado;
        this->tem_peso = tem_peso;
        this->permite_multiplas = permite_multiplas;
        
        adj.resize(vertices+1);
        inv_adj.resize(vertices+1);
        visitados.assign(vertices+1, false);
    }

    string get_nome(){ return nome; }

    int get_n_vertices() { return n_vertices; }

    bool is_direcionado() { return direcionado; }

    bool is_ponderado() { return tem_peso; }

    bool is_multigrafo() { return permite_multiplas; }

    int get_grau(int vertice) {
        if(vertice < 1 || vertice > n_vertices) return -1;
        return adj[vertice].size();
    }

    vector<int> get_vertices_impares() {
        vector<int> impares;
        if(this->direcionado){
            cout << "  [!] AVISO: A logica de impares atual eh para grafos nao direcionados!" << endl;
            return impares;
        }

        for(int i=1; i<=n_vertices; i++){
            if(adj[i].size() % 2 != 0){
                impares.push_back(i);
            }
        }
        return impares;
    }
    
    const vector<pair<int, int>>& get_vizinhos(int vertice) {
        return adj[vertice];
    }

    int get_peso(int u, int v) {
        for(auto [vizinho, peso] : adj[u]) {
            if(vizinho == v) return peso;
        }
        return 1;
    }

    // =======================================================
    // 4. MANIPULAÇÃO DA ESTRUTURA
    // =======================================================
    bool adicionar_aresta(int u, int v, int peso = 1){
        if(u < 1 || u > n_vertices || v < 1 || v > n_vertices){
            cout << "  [!] ERRO: Vertices invalidos! O grafo so vai de 1 a " << n_vertices << "." << endl;
            return false;
        }

        if(u == v){
            cout << "  [!] ERRO: Um vertice nao pode criar uma aresta com ele mesmo!" << endl;
            return false;
        }

        if(!this->permite_multiplas){
            auto it = find_if(adj[u].begin(), adj[u].end(), [v](const pair<int, int>& p){ return p.first == v; });
            if(it != adj[u].end()){
                cout<<"  [!] ERRO: Essa aresta já existe!"<<endl;
                return false;
            }
        }

        if(!this->tem_peso) peso = 1;

        adj[u].push_back({v, peso});
        inv_adj[v].push_back({u, peso});
        
        if(!this->direcionado){
            adj[v].push_back({u, peso});
            inv_adj[u].push_back({v, peso});
        }

        return true;
    }

    bool excluir_aresta(int u, int v, int peso_alvo = -1){
        if(u < 1 || u > n_vertices || v < 1 || v > n_vertices){
            cout << "  [!] ERRO: Vertices invalidos! O grafo so vai de 1 a " << n_vertices << "." << endl;
            return false;
        }

        if(u == v){
            cout << "  [!] ERRO: Um vertice nao possui uma aresta com ele mesmo!" << endl;
            return false;
        }

        auto it = find_if(adj[u].begin(), adj[u].end(), [v, peso_alvo](const pair<int, int>& p){ 
            return p.first == v && (peso_alvo == -1 || p.second == peso_alvo); 
        });
        
        if(it == adj[u].end()){
            cout<<"  [!] ERRO: Essa aresta não existe!"<<endl;
            return false;
        }

        int peso_excluido = it->second;
        adj[u].erase(it);
        
        auto inv_it = find_if(inv_adj[v].begin(), inv_adj[v].end(), [u, peso_excluido](const pair<int, int>& p){ 
            return p.first == u && p.second == peso_excluido; 
        });
        if(inv_it != inv_adj[v].end()){
            inv_adj[v].erase(inv_it);
        }

        if(!this->direcionado){
            auto itt = find_if(adj[v].begin(), adj[v].end(), [u, peso_excluido](const pair<int, int>& p){ 
                return p.first == u && p.second == peso_excluido; 
            });
            if(itt != adj[v].end()){
                adj[v].erase(itt);
            }

            auto itt_inv = find_if(inv_adj[u].begin(), inv_adj[u].end(), [v, peso_excluido](const pair<int, int>& p){ 
                return p.first == v && p.second == peso_excluido; 
            });
            if(itt_inv != inv_adj[u].end()){
                inv_adj[u].erase(itt_inv);
            }
        }

        return true;
    }

    void limpar_visitados(){
        fill(visitados.begin(), visitados.end(), false);
    }

    // =======================================================
    // 5. CAMINHOS E DISTÂNCIAS
    // =======================================================
    void dijkstra(int origem, vector<int>& dist, vector<int>& pai){
        dist.assign(n_vertices+1, INF);
        pai.assign(n_vertices+1, -1);
        
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        dist[origem]=0;
        pq.push({0, origem});

        while(!pq.empty()){
            auto [d, atual] = pq.top();
            pq.pop();

            if(d>dist[atual]) continue;

            for(auto [vizinho, peso] : adj[atual]){
                if(dist[vizinho]>dist[atual]+peso){
                    dist[vizinho]=dist[atual]+peso;
                    pai[vizinho]=atual;
                    pq.push({dist[vizinho], vizinho});
                }
            }
        }
    }

    void mostrar_tamanhos(int origem){
        if(origem < 1 || origem > n_vertices){
            cout << "Vertice de origem invalido!" << endl;
            return;
        }

        limpar_visitados();
        vector<int> dist(n_vertices+1, -1);
        vector<int> pai(n_vertices+1, -1);
        
        bfs(origem, dist, pai);

        for(int i=1; i<=n_vertices; i++){ 
            if(i == origem) continue;

            if(dist[i] == -1){
                cout<<"  -> Vértice "<<i<<": SEM CONEXÃO"<<endl;
            }else{
                cout<<"  -> Vértice "<<i<<": "<<dist[i]<<" arestas"<<endl;
            }
        }
    }

    void mostrar_caminho(int origem, int destino){
        if(origem < 1 || origem > n_vertices){
            cout << "Vertice de origem invalido!" << endl;
            return;
        }

        if(destino < 1 || destino > n_vertices){
            cout << "Vertice de destino invalido!" << endl;
            return;
        }

        if(origem == destino){
            cout<< "Vértice de origem igual ao vértice de destino"<<endl;
            cout<< "Caminho: "<<origem<<endl;
            return;
        }

        limpar_visitados();
        vector<int> dist(n_vertices+1, -1);
        vector<int> pai(n_vertices+1, -1);

        bfs(origem, dist, pai);

        if(dist[destino]==-1){
            cout<<"Impossível chegar no vértice "<<destino<<" a partir do vertice "<<origem<<endl;
            return;
        }

        vector<int> caminho;
        int temp=destino;
        while(temp!=-1){ 
            caminho.push_back(temp);
            temp=pai[temp];
        }

        cout<<"---CAMINHO DO VERTICE: "<<origem<<" AO VERTICE: "<<destino<<"---"<<endl;
        int tam=caminho.size()-1;
        cout<<origem<<" -> ";
        for(int i=0; i<tam; i++){
            cout<<caminho[tam-i-1];
            if(i<tam-1){
                cout<<" -> ";
            }
        }
        cout<<endl;
    }

    vector<int> extrair_ciclo_euleriano(int inicio) {
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

    // =======================================================
    // 6. ANÁLISE DE PROPRIEDADES DO GRAFO
    // =======================================================
    void imprimir_componentes(){
        limpar_visitados();
        int numero_componentes = 0;
        vector<int> tamanhos;
        vector<vector<int>> lista_de_componentes;
        vector<int> componente_atual;

        for(int i=1; i<=n_vertices; i++){
            if(!visitados[i]){
                numero_componentes++;
                tamanhos.push_back(dfs_componente(i,componente_atual));
                sort(componente_atual.begin(), componente_atual.end());
                lista_de_componentes.push_back(componente_atual);
                componente_atual.clear();
            }
        }

        cout<< "Numero de componentes: "<<numero_componentes<<endl<<endl;

        if(numero_componentes>1){
            cout<< "---TAMANHO DE CADA ILHA---"<<endl;
            for(int i = 0; i<numero_componentes; i++){
                cout<< "  -> Componente "<<i+1<<": "<<tamanhos[i]<<" vertices"<<endl;
                cout<<"    -> Vertices da componente: ";
                int tam = lista_de_componentes[i].size();
                for(int j=0; j<tam; j++){
                    cout<<lista_de_componentes[i][j];
                    if(j<tam-1){
                        cout<<", ";
                    }
                }
                cout<<endl;
            }
        }
    }

    bool encontra_ciclos_percorrendo(){
        limpar_visitados();
        for(int i=1; i<=n_vertices; i++){
            if(!visitados[i]){
                if(dfs_ciclos_percorrendo(i, -1)){
                    cout<<"O grafo possui pelo menos 1 ciclo"<<endl;
                    return true;
                }
            }
        }

        cout<<"O grafo não possui ciclos"<<endl;
        return false;
    }

    bool encontra_ciclos_n_arestas(){
        limpar_visitados();
        for(int i=1; i<=n_vertices; i++){
            if(!visitados[i]){
                int qtd_vertices=0;
                int qtd_arestas=0;
                dfs_ciclos_n_arestas(i, qtd_vertices, qtd_arestas);
                qtd_arestas/=2;

                if(qtd_arestas >= qtd_vertices){
                    cout<<"O grafo possui pelo menos 1 ciclo"<<endl;
                    return true;
                }
            }
        }

        cout<<"O grafo não possui ciclos"<<endl;
        return false;
    }

    vector<int> kosaraju(){
        limpar_visitados();
        stack<int> ordem;

        for(int i=1; i<=n_vertices; i++){
            if(!visitados[i]){
                dfs_kosaraju_1(i, ordem);
            }
        }

        limpar_visitados();
        int n_componentes=1;
        vector<int> componentes(n_vertices+1);

        while(!ordem.empty()){
            int atual = ordem.top();
            ordem.pop();

            if(!visitados[atual]){
                dfs_kosaraju_2(atual, componentes, n_componentes++);
            }
        }

        return componentes;
    }

    bool verifica_bipartido(){
        limpar_visitados();
        int cor=0;
        vector<int> coloracao(n_vertices+1, -1);

        for(int i=1; i<=n_vertices; i++){
            if(!visitados[i]){
                if(!dfs_bipartido(i, cor, coloracao)){
                    cout<<"O grafo não é bipartido"<<endl;
                    return false;
                }
            }
        }

        cout<<"O grafo é bipartido"<<endl;

        return true;
    }

    // =======================================================
    // 7. ENTRADA E SAÍDA DE DADOS (I/O)
    // =======================================================
    void imprime_nome(){
        cout<<nome;
    }

    void imprime_grafo(){
        cout<<"Nome: "<<nome<<" | Vértices: "<<n_vertices<<endl;
        imprimir_conexoes();
    }

    void imprimir_conexoes(){
        cout<<endl<<"---LISTA DE ADJACENCIA---"<<endl;
        for(int i=1; i<=n_vertices; i++){
            cout<<"Vertice "<<i<<" -> ";
            if(adj[i].size()==0){
                cout<< "VAZIO";
            }else{
                for(auto [vizinho, peso] : adj[i]){
                    cout<<"["<<vizinho<<"("<<peso<<")] ";
                }
            }
            cout<<endl;
        }
    }

    void salvar_grafo(ofstream& arquivo){
        arquivo << nome << "\n";
        arquivo << n_vertices << "\n";

        int total_arestas = 0;
        vector<pair<pair<int, int>, int>> arestas;
        for(int u = 1; u <= n_vertices; u++){
            for(auto [v, peso] : adj[u]){
                if(u <= v){
                    total_arestas++;
                    arestas.push_back({{u, v}, peso});
                }
            }
        }

        arquivo << total_arestas << "\n";
        for(auto aresta : arestas){
            arquivo << aresta.first.first << " " << aresta.first.second << " " << aresta.second << "\n";
        }
    }
};