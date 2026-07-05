#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include "../headers/Grafos.hpp"
#include "../headers/Heuristicas.hpp" 

using namespace std;

#define endl '\n'

int main() {
    srand(time(NULL));

    ofstream json_greedy("../results/Greedy Results.json");
    ofstream json_nn("../results/Nearest_Neighbor Results.json");

    json_greedy << "[\n"; json_nn << "[\n";
    bool primeiro_registro = true;

    for (auto n : Testes_Heu) {
        vector<double> tempos_greedy(10), tempos_nn(10);
        vector<int> custos_greedy(10), custos_nn(10);

        for (int i = 0; i < 10; i++) {
            // Gera o grafo único na main
            Grafo g_original(n, "grafo_aleatorio", false, true); 
            int peso_original = g_original.gerar_grafo_aleatorio(n, n * 2);

            auto [tempo_g, custo_g] = Greedy(g_original);
            tempos_greedy[i] = tempo_g;
            custos_greedy[i] = peso_original+custo_g;

            auto [tempo_nn, custo_nn] = Nearest_Neighbor(g_original);
            tempos_nn[i] = tempo_nn;
            custos_nn[i] = peso_original + custo_nn;
        }

	//Escrita no Json Greedy

       if (!primeiro_registro) json_greedy << ",\n";
        json_greedy << "  {\n";
        json_greedy << "    \"n_vertices\": " << n << ",\n";
        json_greedy << "    \"tempos_ms\": [";
        for (size_t j = 0; j < tempos_greedy.size(); j++) {
            json_greedy << tempos_greedy[j] << (j < tempos_greedy.size() - 1 ? ", " : "");
        }
        json_greedy << "],\n";
        json_greedy << "    \"custos_ciclo\": [";
        for (size_t j = 0; j < custos_greedy.size(); j++) {
            json_greedy << custos_greedy[j] << (j < custos_greedy.size() - 1 ? ", " : "");
        }
        json_greedy << "]\n  }";

        //Escrita no Json Nearest Neighbor
        if (!primeiro_registro) json_nn << ",\n";
        json_nn << "  {\n";
        json_nn << "    \"n_vertices\": " << n << ",\n";
        json_nn << "    \"tempos_ms\": [";
        for (size_t j = 0; j < tempos_nn.size(); j++) {
            json_nn << tempos_nn[j] << (j < tempos_nn.size() - 1 ? ", " : "");
        }
        json_nn << "],\n";
        json_nn << "    \"custos_ciclo\": [";
        for (size_t j = 0; j < custos_nn.size(); j++) {
	    json_nn << custos_nn[j] << (j < custos_nn.size() - 1 ? ", " : "");
        }
        json_nn << "]\n  }";

        primeiro_registro = false;
    }

    json_greedy << "\n]\n"; json_nn << "\n]\n";
    json_greedy.close(); json_nn.close();

    cout << "Arquivos com tempos e custos gerados!" << endl;
    return 0;
} 
