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
    NodoHash** tabla;
    int capacidad;
    int tamaño;
    double fatorDeCargaMax;
    Cache(int capacidad) : capacidad(capacidad), tamaño (0), fatorDeCargaMax(0.7){}
 };
typedef Cache* Hash;


 double FactorDeCarga(Cache& c){
    return double(c.tamaño / c.capacidad); //averiguar porque no funciona la ->
 }
 
bool esPrimo(int num){
    if(num<=1 || num%2==0 && num!=2) return false;
    if(num==2) return true;
    for (int i = 3; i < num/2; i+=2)
    {
        if(num%i==0)
        {
            return false;
        }
    }
    return true;    
}

int primoSupMinimo(int dato){
    while(!esPrimo(++dato)); 
    return dato;
}

int hash1 (string& clave, int capacidad){
    int moduloMax1 = 1000000007; //chatgpt- que valor debe tener el modulo para el hash1 y el hash2
    int hash = 0;
    for (char c : clave) 
    {
        hash = (hash * 31 + c) % moduloMax1;
    }
    return hash % capacidad;
    
}

int hash2 (string& clave, int capacidad){
    int moduloMax1 = 1000000009;
    int hash = 0;
    for (char c : clave) 
    {
        hash = (hash * 31 + c) % moduloMax1;
    }
    return hash % capacidad;
    
}


void refactor (Hash& h){

}