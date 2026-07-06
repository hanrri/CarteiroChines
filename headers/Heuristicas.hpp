#pragma once
#include <bits/stdc++.h>
#include "Grafos.hpp"
using namespace std;

inline const int MaxImpar = 20;

std::pair<double, int> Greedy(Grafo g);
std::pair<double, int> Nearest_Neighbor(Grafo g);
std::pair<double, int> Enumeration(Grafo g);
