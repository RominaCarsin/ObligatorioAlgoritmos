#include <cassert>
#include <string>
#include <iostream>
#include <limits>
using namespace std;

struct Nodo {
    string clave;
    int valor;
    int altura;
    Nodo* izquierda;
    Nodo* derecha;
    Nodo(string k, int v): clave(k), valor(v), altura(1), izquierda(NULL), derecha(NULL) {}
};

int altura(Nodo* n) {
    return n ? n->altura : 0;
}

int obtenerBalance(Nodo* n) {
    return n ? altura(n->izquierda) - altura(n->derecha) : 0;
}

Nodo* rotarDerecha(Nodo* y) {
    Nodo* x = y->izquierda;
    y->izquierda = x->derecha;
    x->derecha = y;
    y->altura = max(altura(y->izquierda), altura(y->derecha)) + 1;
    x->altura = max(altura(x->izquierda), altura(x->derecha)) + 1;
    return x;
}

Nodo* rotarIzquierda(Nodo* x) {
    Nodo* y = x->derecha;
    x->derecha = y->izquierda;
    y->izquierda = x;
    x->altura = max(altura(x->izquierda), altura(x->derecha)) + 1;
    y->altura = max(altura(y->izquierda), altura(y->derecha)) + 1;
    return y;
}

Nodo* insertarAVL(Nodo* raiz, string clave, int valor) {
    if (!raiz) return new Nodo(clave, valor);

    if (clave < raiz->clave) raiz->izquierda = insertarAVL(raiz->izquierda, clave, valor);
    else{
         raiz->derecha = insertarAVL(raiz->derecha, clave, valor);

    }
    raiz->altura = 1 + max(altura(raiz->izquierda), altura(raiz->derecha));
    int balance = obtenerBalance(raiz);  

    if (balance > 1 && clave < raiz->izquierda->clave) return rotarDerecha(raiz);
    if (balance < -1 && clave > raiz->derecha->clave) return rotarIzquierda(raiz);
    if (balance > 1 && clave > raiz->izquierda->clave) {
        raiz->izquierda = rotarIzquierda(raiz->izquierda);
        return rotarDerecha(raiz);
    }
    if (balance < -1 && clave < raiz->derecha->clave) {
        raiz->derecha = rotarDerecha(raiz->derecha);
        return rotarIzquierda(raiz);
    }

    return raiz;
}

int buscarAVL(Nodo* raiz, string clave) {
    if (!raiz) {
        cout << "Error: clave no encontrada en el AVL." << endl;
        return -1;
     } // no debería pasar
    if (raiz->clave == clave) return raiz->valor;
    if (clave < raiz->clave){
        return buscarAVL(raiz->izquierda, clave);
    }
    return buscarAVL(raiz->derecha, clave);
}

long long mergeAndCount(int arr[], int temporal[], int izquierda, int medio, int derecha) {
    int i = izquierda;
    int j = medio + 1; 
    int k = izquierda;
    long long contador = 0;

    while (i <= medio && j <= derecha) {
        if (arr[i] <= arr[j])
            temporal[k++] = arr[i++];
        else {
            temporal[k++] = arr[j++];
            contador += ((long long)medio - (long long)i + 1);
        }
    }

    while (i <= medio)
        temporal[k++] = arr[i++];
    while (j <= derecha)
        temporal[k++] = arr[j++];

    for (i = izquierda; i <= derecha; i++)
        arr[i] = temporal[i];

    return contador;
}

long long mergeSortAndCount(int arr[], int temporal[], int izquierda, int derecha) {
    if (izquierda >= derecha) return 0;
    int medio = (izquierda + derecha) / 2;

    long long contadorIzquierdo = mergeSortAndCount(arr, temporal, izquierda, medio);
    long long contadorDerecho = mergeSortAndCount(arr, temporal, medio + 1, derecha);
    long long contadorMezcla = mergeAndCount(arr, temporal, izquierda, medio, derecha);

    return contadorIzquierdo + contadorDerecho + contadorMezcla;
}

int main() {
    int n;
    cin >> n;

int s;
    string* oficial = new string[n];
    string* ayudante = new string[n];

    for (int i = 0; i < n; i++) //aca no esta entrando
        cin >> oficial[i];

    Nodo* raiz = nullptr;
    for (int i = 0; i < n; i++)
        raiz = insertarAVL(raiz, oficial[i], i);

    for (int i = 0; i < n; i++)
        cin >> ayudante[i];

    int* arr = new int[n];
    for (int i = 0; i < n; i++)
        arr[i] = buscarAVL(raiz, ayudante[i]);

    int* temporal = new int[n];
    long long resultado = mergeSortAndCount(arr, temporal, 0, n - 1);

    cout << (long long)resultado << endl;

    delete[] oficial;
    delete[] ayudante;
    delete[] arr;
    delete[] temporal;
    return 0;
}