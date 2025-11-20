#include <iostream>
#include "tads/Backtracking.cpp"
using namespace std;

int main() {
    char buscado;
    cin >> buscado;
    int P;
    cin >> P;
    int mejorFC = -1;
    int mejorPasos = 1000000000;

    for(int idx = 1; idx <= P; idx++) {
        int M, N;
        cin >> M >> N;
        // crear matriz
        char** mapa = new char*[M];
        for(int i = 0; i < M; i++) {
            mapa[i] = new char[N];
            for(int j = 0; j < N; j++)
                cin >> mapa[i][j];
        }
        int pasos = resolverFC(buscado, M, N, mapa);

        // elegir el mejor FC
        if(pasos < mejorPasos) {
            mejorPasos = pasos;
            mejorFC = idx;
        }

        // liberar mapa
        for(int i = 0; i < M; i++) delete[] mapa[i];
        delete[] mapa;
    }

    cout << mejorFC << " " << mejorPasos;
    return 0;
}
