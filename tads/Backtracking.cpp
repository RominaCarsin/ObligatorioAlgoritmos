#include <iostream>
using namespace std;

int dI[4] = {1, -1, 0, 0};
int dJ[4] = {0, 0, 1, -1};

bool esAceptable(char** mapa, bool** visitado, int M, int N, int i, int j, char buscado) {
    if(i < 0 || j < 0 || i >= M || j >= N) return false;
    if(visitado[i][j]) return false;
    if(mapa[i][j] == 'C' || mapa[i][j] == buscado) return true;
    return false;
}

void backtracking(char** mapa, bool** visitado, int** dist, int M, int N, int i, int j, int pasos, char buscado, int &mejor) {
    // si encontramos el producto
    if(mapa[i][j] == buscado) {
        if(pasos < mejor) mejor = pasos;
        return;
    }
    // si ya estamos peor que el mejor => podar
    if(pasos >= mejor) return;

    // si ya llegamos aquí con menos pasos => podar
    if(pasos >= dist[i][j]) return;

    // actualizar mejor distancia a esta celda
    dist[i][j] = pasos;

    for(int k = 0; k < 4; k++) {
        int ni = i + dI[k];
        int nj = j + dJ[k];

        if(esAceptable(mapa, visitado, M, N, ni, nj, buscado)) {
            visitado[ni][nj] = true;
            backtracking(mapa, visitado, dist, M, N, ni, nj, pasos + 1, buscado, mejor);
            visitado[ni][nj] = false;
        }
    }
}

int resolverFC(char buscado, int M, int N, char** mapa) {

    bool** visitado = new bool*[M];
    int** dist = new int*[M];

    for(int i = 0; i < M; i++) {
        visitado[i] = new bool[N];
        dist[i] = new int[N];
        for(int j = 0; j < N; j++) {
            visitado[i][j] = false;
            dist[i][j] = 1000000000; // dist init alto
        }
    }

    int mejor = 1000000000;

    if(!(mapa[0][0] == 'C' || mapa[0][0] == buscado)) {
        return mejor;
    }

    visitado[0][0] = true;
    backtracking(mapa, visitado, dist, M, N, 0, 0, 0, buscado, mejor);

    // liberar
    for(int i = 0; i < M; i++) {
        delete[] visitado[i];
        delete[] dist[i];
    }
    delete[] visitado;
    delete[] dist;

    return mejor;
}
