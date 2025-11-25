#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "tads/Greedy.cpp"

using namespace std;

struct Par
{
    int primero;
    int segundo;
};

int main()
{
    int n;
    cin >> n;
    int m;
    cin >> m;
    int f;
    cin >> f;

    /* Par pozos[n + 2]; //revisar esto 
    Par mejoras[m + 2]; */
    Par* pozos = new Par[n + 2];
    Par* mejoras = new Par[m + 2];

    for (int i = 1; i <= n; i++)
    {

        int pozoInicio;
        cin >> pozoInicio;

        int pozoFin;
        cin >> pozoFin;

        pozos[i].primero = pozoInicio;
        pozos[i].segundo = pozoFin;
    }

    for (int j = 1; j <= m; j++)
    {

        int casaMejora;
        cin >> casaMejora;

        int potencia;
        cin >> potencia;

        mejoras[j].primero = casaMejora;
        mejoras[j].segundo = potencia;
    }

    int cantMejoras = 0;
    int posActual = 1;
    int potenciaActual = 1;
    int posPozo = 1;
    int posMejora = 1;

    maxHeap heap(m);

    // avanza posPozo si ya quedó atrás
    auto syncPozos = [&]() {
        while (posPozo <= n && pozos[posPozo].segundo < posActual) posPozo++;
    };

    while (posActual < f)
    {
        syncPozos();
        int reach = posActual + potenciaActual;
        int lim = reach;

        if (posPozo <= n && pozos[posPozo].primero <= reach) {
            if (reach <= pozos[posPozo].segundo) {
                // bloqueado por el pozo actual
                lim = pozos[posPozo].primero - 1;
            } else {
                // puedo pasar este pozo; revisar el siguiente
                if (posPozo + 1 <= n && pozos[posPozo + 1].primero <= reach) {
                    lim = pozos[posPozo + 1].primero - 1;
                } else {
                    lim = reach;
                }
            }
        }

        // Agrego mejoras alcanzables al heap
        while (posMejora <= m && mejoras[posMejora].primero <= lim)
        {
            heap.insertar(mejoras[posMejora].segundo);
            posMejora++;
        }

        // si no puedo moverme, pido mejora y re-evalúo
        if (lim <= posActual) {
            if (heap.estaVacio()) {
                cout << "Imposible" << '\n';
                return 0;
            }
            potenciaActual += heap.extraerMax();
            heap.eliminar();
            cantMejoras++;
            continue;
        }
        posActual = lim;
        syncPozos();
        if(posActual >= f) break;
    }
    
    cout << cantMejoras << endl;
    delete[] pozos;
    delete[] mejoras;
    return 0;
}
