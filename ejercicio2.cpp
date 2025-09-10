#include <cassert>
#include <string>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

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
    int* slotGen;
    int gen;
    int capacidad;
    int tamano;
    double fatorDeCargaMax;
    Cache(int capacidad) : capacidad(capacidad), tamano(0), fatorDeCargaMax(0.7), gen(1) {tabla = new NodoHash*[capacidad](); slotGen = new int[capacidad]();}
 };

 struct Dominio
 {
    NodoHash** tabla;

 };

typedef Cache* Hash;


 double FactorDeCarga(Cache* c){
    return double(c->tamano) / c->capacidad;
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

bool slotIsEmpty(Cache* h, int idx) {
    return h->slotGen[idx] != h->gen || h->tabla[idx] == nullptr || h->tabla[idx]->estaBorrado;
}

void setSlot(Cache* h, int idx, NodoHash* nodo) {
    h->slotGen[idx] = h->gen; // marca la generacion en la cual se inserto
    h->tabla[idx] = nodo;
}

void CLEAR(Hash& h) {
    h->gen++;        // mueve una generacion
    h->tamano = 0;   // elimina tamano a 0
}



void refactor(Hash& h) {
    int nuevaCapacidad = primoSupMinimo(h->capacidad * 2);
    NodoHash** nuevaTabla = new NodoHash*[nuevaCapacidad]();
    int* nuevoSlotGen = new int[nuevaCapacidad]();
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
            nuevoSlotGen[idx] = h->gen;
        }
    }
    delete[] h->tabla;
    h->tabla = nuevaTabla;
    h->capacidad = nuevaCapacidad;
    h->slotGen = nuevoSlotGen;
}

void PUT(Hash& h, string& dom, string& path, string& titulo, int tiempo) {
    if (FactorDeCarga(h) > h->fatorDeCargaMax) 
    {
        refactor(h);
    }
    string clave = dom + "#" + path;
    int h1 = hash1(clave, h->capacidad);
    int h2 = hash2(clave, h->capacidad);

    for (int j = 0; j < h->capacidad; ++j) {
        int idx = (h1 + j * h2) % h->capacidad;

        if (slotIsEmpty(h,idx)) {
            // eliminas el viejo
            delete h->tabla[idx];
            // insertas el nuevo
            h->tabla[idx] = new NodoHash(dom, path, titulo, tiempo);

            // indica la generacion para poder borrar en o1 osea eliminar sin eliminar
            h->slotGen[idx] = h->gen;

            h->tamano++;
            return;
        }

        // If it's the same key, update
        if (h->tabla[idx]->dominio == dom && h->tabla[idx]->path == path) {
            h->tabla[idx]->titulo = titulo;
            h->tabla[idx]->tiempo = tiempo;
            return;
        }
    }
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
                h->tamano--;
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
    cout << h->tamano;
}

void CLEAN(Hash& h){
    for (int i = 0; i < h->capacidad; i++)
    {
       if(h->tabla[i]){
        delete h->tabla[i];
        h->tabla[i]= NULL;
       }
    }
    h->tamano=0;
    delete[] h->tabla;
    h->tabla = new NodoHash*[h->capacidad]();
    
}

int main()
{
    Hash h = new Cache(10);
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        string op;
        cin >> op;
        int opAccion = -1;
        if (op == "PUT") opAccion = 0;
        else if (op == "GET") opAccion = 1;
        else if (op == "REMOVE") opAccion = 2;
        else if (op == "CONTAINS") opAccion = 3;
        else if (op == "COUNT_DOMAIN") opAccion = 4;
        else if (op == "LIST_DOMAIN") opAccion = 5;
        else if (op == "CLEAR_DOMAIN") opAccion = 6;
        else if (op == "SIZE") opAccion = 7;
        else if (op == "CLEAR") opAccion = 8;


        switch (opAccion) {
            case 0: { // PUT <dominio> <path> <titulo> <tiempo>
                string dominio, path, titulo;
                int tiempo;
                cin >> dominio >> path >> titulo >> tiempo;
                PUT(h, dominio, path, titulo, tiempo);
                break;
            }
            case 1: { // GET <dominio> <path>
                string dominio, path;
                cin >> dominio >> path;
                cout << GET(h, dominio, path) << '\n';
                break;
            }
            case 2: { // REMOVE <dominio> <path>
                string dominio, path;
                cin >> dominio >> path;
                REMOVE(h, dominio, path);
                break;
            }
            case 3: { // CONTAINS <dominio> <path>
                string dominio, path;
                cin >> dominio >> path;
                CONTAINS(h, dominio, path);
                break;
            }
            case 4: { // COUNT_DOMAIN <dominio>
                string dominio;
                cin >> dominio;
                COUNT_DOMAIN(h, dominio);
                break;
            }
            case 5: { // LIST_DOMAIN <dominio>
                string dominio;
                cin >> dominio;
                LIST_DOMAIN(h, dominio);
                break;
            }
            case 6: { // CLEAR_DOMAIN <dominio>
                string dominio;
                cin >> dominio;
                CLEAR_DOMAIN(h, dominio);
                break;
            }
            case 7: { // SIZE
                SIZE(h);
                break;
            }
            case 8: { // CLEAR
                CLEAR(h);
                break;
            }
            default:
                break;
        }
        return 0;
    }
    
}
//hay que revisar codigo repetido en CONTAINS, REMOVE, GET ; en vez de struct hacer con class el ejercicicio


