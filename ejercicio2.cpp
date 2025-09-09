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


//template<class T>;

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
    Cache(int capacidad) : capacidad(capacidad), tamaño(0), fatorDeCargaMax(0.7) {tabla = new NodoHash*[capacidad]();}
 };

 struct Dominio
 {
    NodoHash** tabla;

 };

typedef Cache* Hash;


 double FactorDeCarga(Cache* c){
    return double(c->tamaño) / c->capacidad;
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
    return (hash % (capacidad-1))+1; //revisar nuevamente la cuenta 
    
}


void refactor(Hash& h) {
    int nuevaCapacidad = primoSupMinimo(h->capacidad * 2);
    NodoHash** nuevaTabla = new NodoHash*[nuevaCapacidad]();
    for (int i = 0; i < h->capacidad; i++) {
        if (h->tabla[i] && !h->tabla[i]->estaBorrado) {
            string clave = h->tabla[i]->dominio + "#" + h->tabla[i]->path;
            int h1 = hash1(clave, nuevaCapacidad);
            int h2 = hash2(clave, nuevaCapacidad);
            int j = 0;
            int idx = (h1 + j * h2) % nuevaCapacidad;
            while (nuevaTabla[idx] != nullptr) {
                j++;
                idx = (h1 + j * h2) % nuevaCapacidad;
            }
            nuevaTabla[idx] = h->tabla[i];
        }
    }
    delete[] h->tabla;
    h->tabla = nuevaTabla;
    h->capacidad = nuevaCapacidad;
}

void PUT(Hash& h, string& dom, string& path, string& titulo, int tiempo) {
    if (FactorDeCarga(h) > h->fatorDeCargaMax) 
    {
        refactor(h);
    }
    string clave = dom + "#" + path;
    int h1 = hash1(clave, h->capacidad);
    int h2 = hash2(clave, h->capacidad);
    int idx = h1;
    int firstBorrado = -1;
    bool encontre = false;
    //terminarlo
}

string GET (Hash h, string dom, string path){
    string clave = dom +"#"+ path;
    int h1 = hash1(clave, h->capacidad);
    int h2 = hash2(clave, h->capacidad);
    int idx = h1;
    
    for (int j = 0; j < h->capacidad; ++j) { //O(1) promedio
        idx = (h1 + j * h2) % h->capacidad;
        
        if (h->tabla[idx] == NULL) 
        {
            return "recurso_no_encontrado";
        }
        
        if (!h->tabla[idx]->estaBorrado) 
        {
            if (h->tabla[idx]->dominio == dom && h->tabla[idx]->path == path) {
                return h->tabla[idx]->titulo + " " + to_string(h->tabla[idx]->tiempo); //averiguar si se puede usar to string
            }
        }
    }
    return "recurso_no_encontrado";
}

void REMOVE (Hash h, string dom, string path){
    string clave = dom +"#"+ path;
    int h1 = hash1(clave, h->capacidad);
    int h2 = hash2(clave, h->capacidad);
    int idx = h1;
    for (int j = 0; j < h->capacidad; ++j) { //O(1) promedio
        idx = (h1 + j * h2) % h->capacidad;
        
        if (h->tabla[idx] == NULL) 
        {
            return ;
        }
        
        if (!h->tabla[idx]->estaBorrado) 
        {
            if (!h->tabla[idx]->estaBorrado && h->tabla[idx]->dominio == dom && h->tabla[idx]->path == path) {
                h->tamaño--;
                h->tabla[idx]->estaBorrado =true;
                int domHash = 0;






                // falta terminarlo
            }
        }
    }
}

void  CONTAINS (Hash h, string dom, string path){
    string clave = dom +"#"+ path;
    int h1 = hash1(clave, h->capacidad);
    int h2 = hash2(clave, h->capacidad);
    int idx = h1;
    for (int j = 0; j < h->capacidad; ++j) { //O(1) promedio
        idx = (h1 + j * h2) % h->capacidad;
        
        if (h->tabla[idx] == NULL) 
        {
            cout << "false" << endl;
            return ;
        }
        
        if (!h->tabla[idx]->estaBorrado) 
        {
            if (h->tabla[idx]->dominio == dom && h->tabla[idx]->path == path) {
                cout << "true" << endl;
                return;
            }
        }
    }
    cout << "false" << endl;
    return;

}

void COUNT_DOMAIN (Hash h, string dom){
    int cant = 0;
    for (int i = 0; i < h->capacidad; ++i) {
        if (h->tabla[i] && !h->tabla[i]->estaBorrado && h->tabla[i]->dominio == dom) {
            cant++;
        }
    }
    cout << cant << endl;
}

void LIST_DOMAIN(Hash h, string dom) {
   
}


void CLEAR_DOMAIN (Hash& h, string dom ){
    bool encontrado = false;
    
}
void SIZE(Hash h){
    cout << h->tamaño;
}

void CLEAN(Hash& h){
    for (int i = 0; i < h->capacidad; i++)
    {
       if(h->tabla[i]){
        delete h->tabla[i];
        h->tabla[i]= NULL;
       }
    }
    h->tamaño=0;
    delete[] h->tabla;
    h->tabla = new NodoHash*[h->capacidad]();
    
}
//hay que revisar codigo repetido en CONTAINS, REMOVE, GET ; en vez de struct hacer con class el ejercicicio


