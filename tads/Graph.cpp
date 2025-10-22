#include <cassert>
#include <string>
#include <iostream>
#include <limits>
using namespace std;


struct Arista{
    int u;
    int v;
    int peso;
};

void intercambiarAristas(Arista& a1, Arista& a2) {
    Arista aux = a1;
    a1 = a2;
    a2 = aux;
}

int particion(Arista* arr, int inicio, int fin){
    int pivote = arr[fin].peso;
    int i = inicio - 1;
    for (int j = inicio; j < fin; j++) {
        if (arr[j].peso <= pivote) {
            i++;
            intercambiarAristas(arr[i], arr[j]);
        }
    }
    intercambiarAristas(arr[i + 1], arr[fin]);
    return i + 1;
}

void quicksort(Arista* arr, int inicio, int fin){
    if (inicio < fin) {
        int indicePivote = particion(arr, inicio, fin);
        quicksort(arr, inicio, indicePivote - 1);
        quicksort(arr, indicePivote + 1, fin);
    }
}

class DisjointSet {
    private:
        int* padre;
        int* rango;
        int n;
    public:
        DisjointSet(int cant) {
            n = cant;
            padre = new int[n];
            rango = new int[n];
            for (int i = 0; i < n; i++) {
                padre[i] = i;
                rango[i] = 0;
            }
        }
        int buscar(int x) {
            if (padre[x] != x) {
                padre[x] = buscar(padre[x]);
            }
            return padre[x];
        }


        void unir(int a, int b) {
            int x = buscar(a);
            int y = buscar(b);
            if (x != y) {
                if (rango[x] < rango[y]) {
                    padre[x] = y;
                } else if (rango[x] > rango[y]) {
                    padre[y] = x;
                } else {
                    padre[y] = x;
                    rango[x]++;
                }
            }
        }
};

class Grafo{
    private:
        int V;
        int E;
        Arista* aristas;
        int indice; //cuantas aristas hay cargadas
    public:
        Grafo(int vertice, int cantAristas){
            V = vertice;
            E = cantAristas;
            aristas = new Arista[E];
            indice = 0;
        }
        void agregarArista(int u, int v, int peso){
            aristas[indice].u = u;
            aristas[indice].v = v;
            aristas[indice].peso = peso;
            indice++;
        }

        long long kruskalMST() {
            quicksort(aristas, 0, E - 1); //Ordena las aritas a base de su peso
            DisjointSet ds(V);  //utiiza los conjuntos disjuntos
            long long total = 0;
            for (int i = 0; i < E; i++) { // recorre aristas
                int u = aristas[i].u;
                int v = aristas[i].v;
                int w = aristas[i].peso;

                if (ds.buscar(u) != ds.buscar(v)) {
                    ds.unir(u, v);
                    total += w;
                }
            }

            return total; 
        }

};