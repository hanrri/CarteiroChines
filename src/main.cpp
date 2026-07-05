#include <bits/stdc++.h>
#include <fstream>
//inclui a biblioteca de grafos capada
#include "../include/grafo.hpp"

using namespace std;

#define endl '\n'

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    //prepara para a contagem do tempo
    srand(time(NULL));

    vector<int> tamanhos_gulosos={50, 100, 500, 1000, 5000, 10000};
    //já começam a demorar muito
    //50000, 100000

    //início do json
    ofstream arquivo_json("estatisticas_gulosas.json");
    arquivo_json << "[\n";
    bool primeiro_registro = true;

    //cria os vertores fora dos laços para economizar memória
    vector<int> dist; //vetor de distância para o dijkstra
    vector<int> pai; //vetor de pais
    vector<pair<int, pair<int, int>>> arestas_virtuais; //vetor de arestas virtuais para os pares de vértices ímpares
    vector<bool> pareados; //vetor de verificação dos vértices ímpares(se eles já foram "pareados")
    
    //testes gulosos la eleeee
    for(auto n:tamanhos_gulosos){ //percorre o vetor com os números de vértices que definimos antes
        double soma_tempo=0; //lógica de tempo

        //laço para as 10 iterações(para calcular a média no final)
        for(int i=0; i<10; i++){
            Grafo g(n); //cria o grafo
            g.gerar_grafo_aleatorio(n, n*2); //geramos as arestas(aleatoriamente)

            auto inicio=chrono::high_resolution_clock::now(); //lógica de tempo

            //pegamos o vetor de vértices de grau ímpar
            vector<int> vertices_impares = g.get_vertices_impares();

            //se o vertices_impares não estiver vazio, o grafo não é euleriano, segue a lógica(se ele for, fds)
            if(!vertices_impares.empty()){
                //guardamos o número de vértices em uma variável para facilitar as coisas
                int num_vertices = g.get_n_vertices()+1;
                
                //zera os vetores criados como preparação para cada iteração
                dist.assign(num_vertices, 0);
                pai.assign(num_vertices, 0);
                pareados.assign(num_vertices, false);
                arestas_virtuais.clear();

                //lógica para estimar a quantidade de arestas virtuais e reservar espaço no vetor arestas_virtuais
                //quantidade de vértices ímpares no nosso grafo
                long long k=vertices_impares.size();
                //combinação dois a dois entre eles
                arestas_virtuais.reserve((k*(k-1))/2);

                //calculamos os possíveis caminhos a cada par de vértices ímpares
                for(auto it:vertices_impares){
                    //usamos o dijkstra para preenchermos os vetores dist e pai, para cada vértice ímpar,
                    //pegamos o menor caminho até cada outro vértice ímpar
                    g.dijkstra(it, dist, pai);
                    //percorremos o vetor de vértices ímpares para preenchermos o vetor de arestas virtuais
                    for(auto itt:vertices_impares){
                        //só vamos formar o par com it, se itt for maior, isso evita calcular um mesmo par duaa vezes e formar
                        //um par de um vértice com ele mesmo
                        if(it<itt){
                            //finalmente formamos o par e atualizamos nosso vetor de arestas virtuais
                            arestas_virtuais.push_back({dist[itt], {it, itt}});
                        }
                    }
                }
        
                //ordenamos as arestas virtuais da "mais leve" para a "mais pesada", para conseguirmos a melhor solução
                sort(arestas_virtuais.begin(), arestas_virtuais.end());
                
                //vamos percorrer o vetor de arestas virtuais para coletarmos todos esses melhores caminhos pareando os
                //os vértices, ou seja, formando nossos pares
                for(auto it:arestas_virtuais){
                    //vamos olhar para os dois vértices do par da vez, se ambos não tiverem sido pareados ainda, pareamos ele,
                    //caso contrário, vamos para o próximo par
                    if(pareados[it.second.first] == false && pareados[it.second.second] == false){
                        pareados[it.second.first] = true;
                        pareados[it.second.second] = true;
                
                        //esse dijkstra vai recuperar o caminho entre o par.
                        //note que rodamos várias vezes esse algoritmo quando fomos calular todos os pares, logo nosso vetor
                        //pai foi reescrito muitas vezes para cada vértice, então precisamos reculcular esse caminho
                        g.dijkstra(it.second.first, dist, pai);
        
                        //aqui finalmente iremos duplicar as arestas que compõe esse caminho do par
                        //começamos do destino(já que nosso caminho vem ao contrário)
                        int atual=it.second.second;
                        //percorremos o caminho até encontrarmos o início
                        while(atual!=it.second.first){
                            //consultamos no vetor pai para saber de onde vimos para chegar no vértice atual
                            int anterior=pai[atual];
                            //obtemos o valor do peso dessa aresta
                            int peso_real = g.get_peso(anterior, atual);
                            //com os dois vértices e o peso, estamos prontos para duplicar a aresta e retroceder o caminho
                            //mais um vértice até chegar no início
                            g.adicionar_aresta(anterior, atual, peso_real);
                            //voltamos um vértice
                            atual=anterior;
                        }
                    }
                }
            }

            //não incluir o hierholzer no cálculo do tempo? não sei aí como que fica
            vector<int> ciclo = g.extrair_ciclo_euleriano(1);
            reverse(ciclo.begin(), ciclo.end());
            
            //lógica de tempo
            auto fim = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> tempo = fim-inicio;
            soma_tempo+=tempo.count();
            
        }

        //output no terminal para verificação rápida dos resultados, desnecessária na versão final
        double tempo_medio = soma_tempo/10.0;
        cout<<"N = "<<n<<" | Tempo Medio: "<<tempo_medio<<" ms"<<endl;

        //organização do json
        //OBS: criei com o gemini, não sei como é preferido a organização dos dados, tem que ver
        //com os cara lá
        if(!primeiro_registro) arquivo_json << ",\n";
        arquivo_json << "  {\n";
        arquivo_json << "    \"n_vertices\": " << n << ",\n";
        arquivo_json << "    \"tempo_medio_ms\": " << tempo_medio << "\n";
        arquivo_json << "  }";
        primeiro_registro = false;
        
        //limite de tempo q o gemesson disse, ver oq acontece com isso
        /*
        if(tempo_medio>600000){
            cout<<"Passou de 10 minutos. Abortado!!!"<<endl;
            break;
        }
        */
    }

    //finaliza o json ao final de todas as iterações de todos os grafos
    arquivo_json << "\n]\n";
    arquivo_json.close();

    //output de confirmação da criação de json,  talvez desnecessário na versão final
    cout << "estatística_gulosas.json gerado." << endl;

    return 0;
}