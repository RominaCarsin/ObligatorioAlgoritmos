#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;




class Medio{
    public:
    string dominio;
    string path;
    string titulo;
    int tiempo;
    Medio(string d, string p, string t, int time ): dominio(d), path(p), titulo(t), tiempo(time){}  

    bool operator==(const Medio& otro) const { //chat gpt- funcion para comparar en medio
    return dominio == otro.dominio && path == otro.path;
}
};

// class nodoDominio{
//     string dominio;
//     int cantidad;
//     nodoDominio(string d): dominio(d), cantidad(0){}
// };

template<class K, class V>
class nodoHash {//representa una celda hash
public:
    pair<K,V> dato;
    bool estaBorrado;
    nodoHash(T dato): dato(dato), estaBorrado(false) {}
};

template<class K, class V>
class Hash{
    private: 
        nodoHash<K,V>** tabla;
        int maxElementos;
        int cantElementos;
        int largoVec;
        int(*fHash1)(T);
        int(*fHash2)(T);
        double factorDeCaga;

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

        void resize(){
            int largoAnterior = largoVec;
            nodoHash<K,V>** tablaVieja = tabla;
            //calculo el nuevo tamaño
            int largoNuevo = primoSupMinimo(largoVec*2);
            largoVec = largoNuevo;
            cantElementos = 0;
            
            //creo tabla vacia
            tabla = newnodoHash<K,V>*[largoNuevo];
            for(int i=0; i<largoNuevo; i++) tabla[i]=NULL;
            //vuelvo a poner los elementos en la tabla nueva
            for (int i = 0; i < largoAnterior; i++)
            {
                if(tablaVieja[i] != NULL && !tablaVieja[i]->estaBorrado){
                    put(tablaVieja[i]->dato);
                }
            }

            delete[] tablaVieja;
            
        }

        void maxCantElementos(){
            maxElementos = int (largoVec * factorDeCaga);
        }

        int calcularPos (K dato){
            int h1 = fHash1(dato) % largoVec;
            int h2 = (fHash2(dato) % (largoVec - 1)) + 1; // evitar h2 = 0
            int pos = h1;
            int i = 0;

            while (tabla[pos] && (!tabla[pos]->estaBorrado && tabla[pos]->dato != dato) && i < largoVec) {
                pos = (h1 + i * h2) % largoVec;
                i++;
            }
            return (i < largoVec) ? pos : -1; 
        }
    public:
        Hash(int (*h1)(K), int (*h2)(K), int tam = 101, double fCarga = 0.7) {
            largoVec = tam;
            factorDeCarga = fCarga;
            fHash1 = h1;
            fHash2 = h2;
            tabla = new nodoHash<K,V>*[largoVec];
            for (int i = 0; i < largoVec; i++) tabla[i] = NULL;
            cantElementos = 0;
            maxCantElementos();
        }
        
        void put (K clave, V valor) {//es el PUT
            if ((cantElementos + 1) > maxElementos) {
            resize();
            maxCantElementos();
        }

            int h1 = fHash1(clave) % largoVec;
            int h2 = (fHash2(clave) % (largoVec - 1)) + 1;
            int pos = h1;
            int i = 0;

            while (tabla[pos] && !tabla[pos]->estaBorrado && !(tabla[pos]->dato.first == clave) && i < largoVec) {
                i++;
                pos = (h1 + i * h2) % largoVec;
            }

            if (tabla[pos] == NULL || tabla[pos]->estaBorrado) {
                tabla[pos] = new nodoHash<K,V>(make_pair(clave, valor));
                cantElementos++;
            } else {
                tabla[pos]->dato.second = valor; // actualizar valor
            }
        }
        
};

int hash1(pair<string,string> clave) {
    int hash = 0;
    for (char c : clave.first) hash = hash * 31 + c;
    return hash;
}

int hash2(pair<string,string> clave) {
    int hash = 0;
    for (char c : clave.second) hash = hash * 73 + c;
    return hash;
}






int main()
{
    int N;
    cin >> N;

    Hash tablaDominios = new Hash();

    for(i = 0; i < N; i++){
        string actual;
        cin >> actual;

        switch(/*la primera palabra (PUT, GET, CLEAR...)*/) {
            case "PUT":
                tablaDominios.insertar();
                break;
            case "GET":
                // code block
                break;
            default:
                // code block
        }

        // Partir string en 5 (si es un put)
        /*
        
        Si es un PUT: Agrego a la tabla la clave
        (dominio,path) con el valor (titulo,tiempo)

        Si es un size: devuelvo la cantidad de elementos
        de la tabla

        Si es un count domain: cuento los /xxx
        (los path de un dominio)
        
        Si es un list domain: los muestra (los /xxx)

        Si es un get: devuelvo (titulo,tiempo)

        contains: true si esta en la tabla, false si no

        clear domain: elimina un elemento de la tabla

        clear: elimina todos los elementos de la tabla

        remove: elimina un recuso (/xxx) especifico
        */
    }
}