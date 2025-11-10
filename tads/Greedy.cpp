#include <iostream>
#include "../funciones/enteros.cpp"
#include "List.h"

using namespace std;

class maxHeap
{ // de más grande a más chico (hijos menores al padre)
private:
    int capacidad;
    int cantElementos;
    int *vector;

    bool lleno()
    {
        return cantElementos == capacidad;
    }

    int padre(int pos)
    {
        return pos / 2;
    }

    int hijoIzq(int pos)
    {
        return pos * 2;
    }

    int hijoDer(int pos)
    {
        return pos * 2 + 1;
    }

    void intercambiar(int posPadre, int posHijo)
    {
        int aux = vector[posPadre];
        vector[posPadre] = vector[posHijo];
        vector[posHijo] = aux;
    }

    bool comparar(int posPadre, int posHijo)
    {
        return vector[posPadre] < vector[posHijo];
    }

    void flotar(int pos)
    { // balancea comparando a los hijos con el padre
        if (pos > 1)
        {
            int posPadre = padre(pos);
            if (comparar(posPadre, pos))
            {
                intercambiar(posPadre, pos);
                flotar(posPadre);
            }
        }
    }

    void hundir(int pos)
    { // balancea comparando a el padre con los hijos
        int hijoI = hijoIzq(pos);
        int hijoD = hijoDer(pos);

        if (hijoI > cantElementos)
            return; // no tiene hijos

        int mayorHijo = hijoI;
        if (hijoD <= cantElementos && comparar(mayorHijo, hijoD))
        {
            mayorHijo = hijoD;
        }
        if (comparar(pos, mayorHijo))
        {
            intercambiar(pos, mayorHijo);
            hundir(mayorHijo);
        }
    }

public:
    bool estaVacio()
    {
        return cantElementos == 0;
    }

    maxHeap(int c)
    {
        capacidad = c;
        cantElementos = 0;
        vector = new int [c + 2];
    }

    void insertar(int valor){
        if (lleno()) return;
            vector[++cantElementos] = valor;
            flotar(cantElementos);
    }

    int extraerMax(){
        if (estaVacio()) return -1;
        return vector[1];
    }

    void eliminar()
    {
        if (estaVacio()) return;
        vector[1] = vector[cantElementos--];
        hundir(1);
    }
};
