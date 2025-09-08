#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

int main()
{
    // TODO
    return 0;
}

 struct NodoHash
 {
    string dominio;
    string path;
    string titulo;
    int tiempo;
    bool estaBorrado;
    NodoHash(string dominio, string& path, string titulo, int tiempo): dominio(dominio), path(path), titulo(titulo), tiempo(tiempo), estaBorrado(false) {}
 };
 
 struct Cache
 {
    NodoHash* tabla;
    int capacidad;
    int tamaño;
    Cache(int capacidad) : capacidad(capacidad), tamaño (0){}
 };
 

 