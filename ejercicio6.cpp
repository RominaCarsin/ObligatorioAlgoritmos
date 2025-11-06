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

    Par pozos[n + 1];
    Par mejoras[m + 1];

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

    while (posActual < f)
    {
        while (posMejora <= m && mejoras[posMejora].primero <= posActual + potenciaActual)
        {
            heap.insertar(mejoras[posMejora].segundo);
            posMejora++;
        }

        // Hay un pozo, puedo saltarlo?
        if (posPozo <= n && pozos[posPozo].primero <= posActual + potenciaActual)
        {
            // Sí, sigo de largo
            if (pozos[posPozo].segundo < posActual + potenciaActual)
            {
                posActual = max(posActual + potenciaActual, pozos[posPozo].segundo + 1);
                posPozo++;
            }
            // No, necesito una mejora
            else
            {
                if (heap.estaVacio())
                {
                    cout << "Imposible" << endl;
                    return 0;
                }

                // saco del minHeap y uso la mejora
                potenciaActual += heap.extraerMax();
                heap.eliminar();
                cantMejoras++;
            }
        }
        // No hay pozo, avanzo
        else
        {
            posActual += potenciaActual;
        }
    }
    cout << cantMejoras << endl;
    return 0;
}
