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


int hash1(pair<string,string> clave) { //chatgpt : como me quedarian los hash 
    int hash = 0;
    for (char c : clave.first) hash = hash * 31 + c;
    return hash;
}

int hash2(pair<string,string> clave) {
    int hash = 0;
    for (char c : clave.second) hash = hash * 73 + c;
    return hash;
}

template<class K, class V>
class nodoHash {//representa una celda hash
public:
    pair<K,V> dato;
    bool estaBorrado;
    nodoHash(pair<K,V> dato): dato(dato), estaBorrado(false) {}
};

template<class K, class V>
class Hash{
    private:
        nodoHash<K,V>** tabla;
        int maxElementos;
        int cantElementos;
        int largoVec;
        int(*fHash1)(K);
        int(*fHash2)(K);
        double factorDeCarga;

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

        // rehashing
        void resize(){
            int largoAnterior = largoVec;
            nodoHash<K,V>** tablaVieja = tabla;
            //calculo el nuevo tamaño
            int largoNuevo = primoSupMinimo(largoVec*2);
            largoVec = largoNuevo;
            cantElementos = 0;
            
            //creo tabla vacia
            tabla = new nodoHash<K,V>*[largoNuevo];
            for(int i=0; i<largoNuevo; i++) tabla[i]=NULL;
            //vuelvo a poner los elementos en la tabla nueva
            for (int i = 0; i < largoAnterior; i++)
            {
                if(tablaVieja[i] != NULL && !tablaVieja[i]->estaBorrado){
                    put(tablaVieja[i]->dato.first, tablaVieja[i]->dato.second);
                }
            }

            delete[] tablaVieja;
            
        }

        void maxCantElementos(){
            maxElementos = int (largoVec * factorDeCarga);
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
        Hash(int (*h1)(K), int (*h2)(K), int tam, double fCarga) {
            largoVec = tam;
            factorDeCarga = fCarga;
            fHash1 = h1;
            fHash2 = h2;
            tabla = new nodoHash<K,V>*[largoVec];
            for (int i = 0; i < largoVec; i++) tabla[i] = NULL;
            cantElementos = 0;
            maxCantElementos();
        }

        //pre:       
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

        void get(K clave) {
            int h1 = fHash1(clave) % largoVec;
            int h2 = (fHash2(clave) % (largoVec - 1)) + 1;
            int pos = h1;
            int i = 0;

            while (tabla[pos] && (!tabla[pos]->estaBorrado && tabla[pos]->dato.first != clave) && i < largoVec) {
            i++;
            pos = (h1 + i * h2) % largoVec;
            }

            if (tabla[pos] && !tabla[pos]->estaBorrado && tabla[pos]->dato.first == clave) {
            cout << tabla[pos]->dato.second.first << " " << tabla[pos]->dato.second.second << endl;
            } else {
            cout << "recurso_no_encontrado" << endl;
            }
        }
        void remove(K clave) {
            int h1 = fHash1(clave) % largoVec;
            int h2 = (fHash2(clave) % (largoVec - 1)) + 1;
            int pos = h1;
            int i = 0;

            while (tabla[pos] && (!tabla[pos]->estaBorrado && tabla[pos]->dato.first != clave) && i < largoVec) {
            i++;
            pos = (h1 + i * h2) % largoVec;
            }

            if (tabla[pos] && !tabla[pos]->estaBorrado && tabla[pos]->dato.first == clave) {
            tabla[pos]->estaBorrado = true;
            cantElementos--;
            }
        }
};



int main()
{
    Hash<pair<string,string>, pair<string,int>>* h = new Hash<pair<string,string>, pair<string,int>>(hash1, hash2, 101, 0.7);
    int n;    cin >> n;
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
                h->put(make_pair(dominio, path), make_pair(titulo, tiempo));
                break;
            }
            case 1: { // GET <dominio> <path>
                string dominio, path;
                cin >> dominio >> path;
                h->get(make_pair(dominio,path));
                break;
            }
            case 2: { // REMOVE <dominio> <path>
                string dominio, path;
                cin >> dominio >> path;
                h->remove(make_pair(dominio,path));
                break;
            }
            case 3: { // CONTAINS <dominio> <path>
                string dominio, path;
                cin >> dominio >> path;
                //CONTAINS(h, dominio, path);
                break;
            }
            case 4: { // COUNT_DOMAIN <dominio>
                string dominio;
                cin >> dominio;
                //COUNT_DOMAIN(h, dominio);
                break;
            }
            case 5: { // LIST_DOMAIN <dominio>
                string dominio;
                cin >> dominio;
                //LIST_DOMAIN(h, dominio);
                break;
            }
            case 6: { // CLEAR_DOMAIN <dominio>
                string dominio;
                cin >> dominio;
                //CLEAR_DOMAIN(h, dominio);
                break;
            }
            case 7: { // SIZE
                //SIZE(h);
                break;
            }
            case 8: { // CLEAR
                //CLEAR(h);
                break;
            }
            default:
                break;
        }
    }
    return 0;
}