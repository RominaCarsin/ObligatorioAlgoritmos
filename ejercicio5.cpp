#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "tads/Graph.cpp"


using namespace std;

int main()
{
    int V, E;
    cin >> V >> E;

    Grafo g(V, E);

    for (int i = 0; i < E; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        g.agregarArista(u, v, w);
    }

    cout << g.kruskalMST() << "\n";

    return 0;
}