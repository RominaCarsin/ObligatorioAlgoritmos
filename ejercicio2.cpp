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

    NodoHash(string d, string p, string t): dominio(d), path(p), titulo(t),tiempo(0), estaBorrado(false){}
};

struct Reprecentacion{
    NodoHash** tabla;
    int maxElementos;
    int cantElementos;
    int largoVec;

     Reprecentacion(int maxElementos){
        this->maxElementos = maxElementos;
        this->cantElementos = 0;
        this->largoVec = primoSupMinimo(maxElementos);
        this->tabla = new NodoHash*[this->largoVec];
        for (int i = 0; i < this->largoVec; i++) {
            this->tabla[i] = NULL;
        }
    }
};

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

int fHash(string dominio){ //esta mal esto es para un hash abierto creo
    int hash = 0;
    int primo = 31; //no importa el valor
    for (char c : dominio) {// Calcula un hash entero a partir del dominio
        hash = (hash * primo + c) % numeric_limits<int>::max();
    }
    return abs(hash);
}

void PUT (Reprecentacion *& h, string dominio, string path, string titulo){
    int i = fHash(dominio)%h->largoVec;
    while(h->tabla[i] && !h->tabla[i]->estaBorrado){
        i = (i+1) % h->largoVec;
    }
    if(h->tabla[i]){
        h->tabla[i]->estaBorrado=false;
        h->tabla[i]->dominio = dominio;
    } else {
        h->tabla[i] = new NodoHash(dominio, path, titulo);
    }
    h->cantElementos++;
}

