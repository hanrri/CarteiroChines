#pragma once
#include <bits/stdc++.h>
#include "Grafos.hpp"

inline const std::vector<int> Testes_Heu = {50, 100, 150};
inline const std::vector<int> Testes_Enum = {50, 100, 150};

std::pair<double, int> Greedy(Grafo g);
std::pair<double, int> Nearest_Neighbor(Grafo g);
