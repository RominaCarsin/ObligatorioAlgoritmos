#include <iostream>
#include <string>
#include <cstring>

using namespace std;
typedef unsigned long long U64;


const int MAXLEN = 51; // maximo largo de domain, path, title (50 caracteres 1 terminator)

// declaro antes porque tienen conexion circular y se tienen qeu declarar una antes de la otra
struct DomainNode;
struct HashNode;

// pasar a clase?
// el nodo de cada hash conn todo s los datos 
struct HashNode{
    DomainNode* domainNode;// puntero al dominio al que pertenece este recurso cache
    HashNode* nextDomain;// cache (ordenada por orden de insert)
    HashNode* prevDomain;
    char path[MAXLEN];
    char title[MAXLEN];
    int time;
    bool deleted;

    // constructor nulo pero si no lo termino usando borrar
    HashNode(): domainNode(NULL), nextDomain(NULL), prevDomain(NULL), time(0), deleted(false) {
    path[0] = '\0';
    title[0] = '\0';
    }

    HashNode(DomainNode* dom, const char* p, const char* t, int tim): domainNode(dom), nextDomain(NULL), prevDomain(NULL), time(tim), deleted(false) {
    strncpy(path, p, MAXLEN); // se usa esto porque me saltaba error en el path(p)
    path[MAXLEN-1] = '\0'; // para que se sepa su ultima posicion como un string
    strncpy(title, t, MAXLEN);
    title[MAXLEN-1] = '\0';
    }
};

// el nodo solo para dominios para la busqueda eficiente por dominio como el list domain
struct DomainNode{
    string domain;
    HashNode* head; // mas reciente
    HashNode* tail; // mas antiguo
    int count;
    bool deleted;// tombstone, empty null, ocupied false, deleted true

    // al igual que arriba si no termina siendo necesario el empty constructor borrar
    DomainNode(): domain(""), head(NULL), tail(NULL), count(0), deleted(false) {}
    DomainNode(const string& dom): domain(dom), head(NULL), tail(NULL), count(0), deleted(false) {}

};


class Cache{
    private:
        HashNode** hashTable; // tabla hash con todos los   s datos
        int tableSize;
        DomainNode** domainTable;
        int domainTableSize;
        int totalCount; // cantidad de caches en la tabla
        
        // para la eleccion del tamano de la tabla
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


        int primoSupMinimo(int valor){
            if (valor <= 2) return 2;
            if(valor % 2 ==0) valor++; // lo hacce impar
            while(!esPrimo(valor)) valor+=2;
            return valor;
        }

        // revisar mejor despues
        uint64_t hashKey(const string& domain, const char* path){ // fixed longer length int always 64 bits
            const uint64_t FNV_OFFSET = 1469598103934665603ULL;
            const uint64_t FNV_PRIME = 1099511628211ULL;
            uint64_t hash = FNV_OFFSET;
            // Incorporar caracteres del dominio
            for (char c: domain) {
                hash ^= (unsigned char)c;
                hash *= FNV_PRIME;
            }
            // Separador para diferenciar dominio y path
            hash ^= (unsigned char)'/';
            hash *= FNV_PRIME;
            // Incorporar caracteres del path
            for (const char* p = path; *p; ++p) {
                hash ^= (unsigned char)(*p);
                hash *= FNV_PRIME;
            }
            return hash;
        }
        

        // Función de hash 64-bit para clave de dominio (solo dominio) usando FNV-1a
        uint64_t hashDomain(const string& domain) {
            const uint64_t FNV_OFFSET = 1469598103934665603ULL;// pasar a formato constante global en archivo
            const uint64_t FNV_PRIME = 1099511628211ULL;
            uint64_t hash = FNV_OFFSET;
            for (char c: domain) {
                hash ^= (unsigned char)c;
                hash *= FNV_PRIME;
            }
            return hash;
        }
    public:
        Cache(int maxOps){// maxOps se utiliza como la cantidad de opperaciones de usuario al principio que tiene sus debilidades ya que pueden ser 1 put y 100 gets, pero queda bonito
            tableSize = primoSupMinimo(maxOps);
            domainTableSize = primoSupMinimo(maxOps);
            hashTable = new HashNode*[tableSize];
            domainTable = new DomainNode*[domainTableSize];
            for (int i = 0; i < tableSize; ++i)  hashTable[i]  = NULL;//++i o i++?
            for (int i = 0; i < domainTableSize; ++i)  domainTable[i] = NULL;
            totalCount=0;
        }

        //pre: se instancia dentro de un non null cache
        //post: inserta el nuevo nodoHash y domainHash an cache
        void put(const string& dom, const string& path, const string& tit, int tim){
            uint64_t h = hashDomain(dom);
            int idx = h% domainTableSize;// indice inicial con wrap around por si se pasa
            int step = 1 + ((h>>32) % (domainTableSize - 1));// double hashing con el indice y esto es double hash
            DomainNode* domNode = NULL;
            int firstTombstoneIndex = -1;// tombstone es una marca del deleted como mi sistema viejo de generadciones pero con bools
            
            // buscamos la posicion a insertar en la tabla
            while(domainTable[idx]!=NULL){
                if(!domainTable[idx]->deleted && domainTable[idx]->domain == dom){// existe la posicion pero no esta eliminado
                    domNode = domainTable[idx];
                    break;
                }
                if(domainTable[idx]->deleted && firstTombstoneIndex == -1){// esta eliminada la posicion y no tenemos indice
                    firstTombstoneIndex = idx;
                }
                idx = (idx+step)% domainTableSize;// ahora si el double hashing combina los dos
            }
            if(domNode == NULL){
                // si no encontramos dominio se inserta el nuevo normal
                if(firstTombstoneIndex !=-1){
                    idx = firstTombstoneIndex;
                }
                if (domainTable[idx] != NULL) {
                    // Reutilizar nodo DomainNode marcado borrado
                    domNode = domainTable[idx];
                    domNode->domain = dom;
                    domNode->deleted = false;
                    domNode->count = 0;
                    domNode->head = domNode->tail = NULL;
                } else {
                    // Crear nuevo nodo de dominio
                    domNode = new DomainNode(dom);
                    domainTable[idx] = domNode;
                }
            }
            // 2. Buscar el recurso en la tabla principal (para actualizar si ya existe)
            uint64_t h2 = hashKey(dom, path.c_str());
            int index = h2 % tableSize;                           // índice inicial en tabla de recursos
            int step2 = 1 + ((h2 >> 32) % (tableSize - 1));       // paso (double hashing)
            HashNode* node = NULL;
            int firstTombIndex = -1;
            // Buscar recurso existente (y primer tombstone para posible inserción)
            while (hashTable[index] != NULL) {
                if (!hashTable[index]->deleted && hashTable[index]->domainNode == domNode 
                    && strcmp(hashTable[index]->path, path.c_str()) == 0) {
                    node = hashTable[index]; // recurso encontrado
                    break;
                }
                if (hashTable[index]->deleted && firstTombIndex == -1) {
                    firstTombIndex = index;
                }
                index = (index + step2) % tableSize;
            }

            if (node != NULL) {
                // Si el recurso ya existe, actualizar título y tiempo
                strncpy(node->title, tit.c_str(), MAXLEN);
                node->title[MAXLEN-1] = '\0';
                node->time = tim;
                // Mover el recurso actualizado a la cabeza de la lista del dominio
                if (domNode->head != node) {
                    // Desenlazar nodo de su posición actual
                    if (node->prevDomain) node->prevDomain->nextDomain = node->nextDomain;
                    if (node->nextDomain) node->nextDomain->prevDomain = node->prevDomain;
                    if (domNode->tail == node) {
                        domNode->tail = node->prevDomain;
                    }
                    // Insertar al frente
                    node->prevDomain = NULL;
                    node->nextDomain = domNode->head;
                    if (domNode->head) domNode->head->prevDomain = node;
                    domNode->head = node;
                    if (domNode->tail == NULL) {
                        domNode->tail = node;
                    }
                }
            } else {
                // Si el recurso no existe, insertar nuevo nodo
                if (firstTombIndex != -1) {
                    index = firstTombIndex;
                }
                // Crear o reutilizar HashNode en la posición encontrada
                if (hashTable[index] != NULL) {
                    // Reutilizar nodo tombstone existente
                    node = hashTable[index];
                    node->deleted = false;
                    node->domainNode = domNode;
                    strncpy(node->path, path.c_str(), MAXLEN);
                    node->path[MAXLEN-1] = '\0';
                    strncpy(node->title, tit.c_str(), MAXLEN);
                    node->title[MAXLEN-1] = '\0';
                    node->time = tim;
                    node->nextDomain = node->prevDomain = NULL;
                } else {
                    // Espacio vacío: crear nuevo nodo
                    node = new HashNode(domNode, path.c_str(), tit.c_str(), tim);
                    hashTable[index] = node;
                }
                // Enlazar el nuevo recurso en la lista del dominio (al frente por ser más reciente)
                node->nextDomain = domNode->head;
                node->prevDomain = NULL;
                if (domNode->head != NULL) {
                    domNode->head->prevDomain = node;
                }
                domNode->head = node;
                if (domNode->tail == NULL) {
                    domNode->tail = node;
                }
                // Actualizar contadores
                domNode->count += 1;
                totalCount += 1;
            }
        }
        //pre:
        //post: imprimir titulo tiempo o recurso no encontrado si no existe
        void get(const string& dom, const string& path) {
            // Buscar el dominio en la tabla de dominios
            uint64_t h = hashDomain(dom);
            int idx = h % domainTableSize;
            int step = 1 + ((h >> 32) % (domainTableSize - 1));
            DomainNode* domNode = NULL;
            while (domainTable[idx] != NULL) {
                if (!domainTable[idx]->deleted && domainTable[idx]->domain == dom) {
                    domNode = domainTable[idx];
                    break;
                }
                idx = (idx + step) % domainTableSize;
                if (domainTable[idx] == NULL) break;// llegó a un espacio vacío => no existe
            }
            if (domNode == NULL) {
                cout << "recurso_no_encontrado\n";
                return;
            }
            // Buscar el recurso en la tabla principal 
            // sigo pensando que tal vez tengan el mismo largo pero tengo que probar
            uint64_t h2 = hashKey(dom, path.c_str());
            int index = h2 % tableSize;
            int step2 = 1 + ((h2 >> 32) % (tableSize - 1));
            HashNode* node = NULL;
            while (hashTable[index] != NULL) {
                if (!hashTable[index]->deleted && hashTable[index]->domainNode == domNode 
                    && strcmp(hashTable[index]->path, path.c_str()) == 0) {
                    node = hashTable[index];
                    break;
                }
                index = (index + step2) % tableSize;
                if (hashTable[index] == NULL) break;
            }
            if (node == NULL) {
                cout << "recurso_no_encontrado\n";
            } else {
                cout << string(node->title) + " " + to_string(node->time) + "\n";
            }
        }

        void contains(const string& dom, const string& path) {
            // Buscar dominio
            // funcion double hash?? revisar el resto, coutndomain tiene el mismo promedio si ahi funciona igual segureamente tengo que hacerlo
            uint64_t h = hashDomain(dom);
            int idx = h % domainTableSize;
            int step = 1 + ((h >> 32) % (domainTableSize - 1));
            DomainNode* domNode = NULL;
            while (domainTable[idx] != NULL) {
                if (!domainTable[idx]->deleted && domainTable[idx]->domain == dom) {
                    domNode = domainTable[idx];
                    break;
                }
                idx = (idx + step) % domainTableSize;
                if (domainTable[idx] == NULL) break;
            }
            if (domNode == NULL) {
                cout << "false\n";
                return;
            }
            // Buscar recurso en tabla principal
            // se repite siempre el aso index y step se deberia transformar en su propia funcion capaz vere cuando termine de escribir el resto si es igual siempre si
            uint64_t h2 = hashKey(dom, path.c_str());
            int index = h2 % tableSize;
            int step2 = 1 + ((h2 >> 32) % (tableSize - 1));
            bool found = false;
            while (hashTable[index] != NULL) {
                if (!hashTable[index]->deleted && hashTable[index]->domainNode == domNode 
                    && strcmp(hashTable[index]->path, path.c_str()) == 0) {
                    found = true;
                    break;
                }
                index = (index + step2) % tableSize;
                if (hashTable[index] == NULL) break;
            }
            cout<< (found ? "true\n" : "false\n");
        }

        //pre:
        //post:
        void remove(const string& dom, const string& path) {
            // Buscar dominio
            uint64_t h = hashDomain(dom);
            int idx = h % domainTableSize;                         // índice inicial en tabla de dominios
            int step = 1 + ((h >> 32) % (domainTableSize - 1));    // paso (double hashing)
            DomainNode* domNode = NULL;
            while (domainTable[idx] != NULL) {
                if (!domainTable[idx]->deleted && domainTable[idx]->domain == dom) {
                    domNode = domainTable[idx];
                    break;
                }
                idx = (idx + step) % domainTableSize;
                if (domainTable[idx] == NULL) break;
            }
            if (domNode == NULL) {
                // Dominio no existe, nada que eliminar
                return;
            }
            // Buscar recurso en la tabla principal
            // otra vez esto funciona igual que is tiene sentido y yo ando bien distraido escribiendo asi que funcion
            uint64_t h2 = hashKey(dom, path.c_str());
            int index = h2 % tableSize;
            int step2 = 1 + ((h2 >> 32) % (tableSize - 1));
            while (hashTable[index] != NULL) {
                if (!hashTable[index]->deleted && hashTable[index]->domainNode == domNode 
                    && strcmp(hashTable[index]->path, path.c_str()) == 0) {
                    // Recurso encontrado: marcar como borrado
                    HashNode* node = hashTable[index];
                    node->deleted = true;
                    // Eliminar de la lista del dominio
                    if (node->prevDomain) node->prevDomain->nextDomain = node->nextDomain;
                    if (node->nextDomain) node->nextDomain->prevDomain = node->prevDomain;
                    if (domNode->head == node) domNode->head = node->nextDomain;
                    if (domNode->tail == node) domNode->tail = node->prevDomain;
                    // Actualizar contadores
                    domNode->count -= 1;
                    totalCount -= 1;
                    // Si el dominio queda sin recursos, marcar dominio como borrado (tombstone)
                    if (domNode->count == 0) {
                        domNode->deleted = true;
                    }
                    break;
                }
                index = (index + step2) % tableSize;
                if (hashTable[index] == NULL) break;
            }
        }

        //pre:
        //post: imprimier cantidad de apariciones de un domain
        void countDomain(const string& dom) {
            // Buscar dominio o funcion buscardominio?
            uint64_t h = hashDomain(dom);
            int idx = h % domainTableSize;                         // índice inicial en tabla de dominios
            int step = 1 + ((h >> 32) % (domainTableSize - 1));    // paso (double hashing)
            DomainNode* domNode = NULL;
            while (domainTable[idx] != NULL) {
                if (!domainTable[idx]->deleted && domainTable[idx]->domain == dom) {
                    domNode = domainTable[idx];
                    break;
                }
                idx = (idx + step) % domainTableSize;
                if (domainTable[idx] == NULL) break;
            }
            if (domNode == NULL) {
                cout << "0\n";
            } else {
                cout << to_string(domNode->count) + "\n";
            }
        }

        //pre:
        //post: 
        void listDomain(const string& dom) {
            // Buscar dominio otra vez si no era funcion osea si de doublehashing conectada a buscar dominio y se solucina esta cantidad asqeurosa de codigo
            uint64_t h = hashDomain(dom);
            int idx = h % domainTableSize;
            int step = 1 + ((h >> 32) % (domainTableSize - 1));
            DomainNode* domNode = NULL;
            while (domainTable[idx] != NULL) {
                if (!domainTable[idx]->deleted && domainTable[idx]->domain == dom) {
                    domNode = domainTable[idx];
                    break;
                }
                idx = (idx + step) % domainTableSize;
                if (domainTable[idx] == NULL) break;
            }
            if (domNode == NULL || domNode->count == 0) {
                // Dominio no existe o sin recursos: línea vacía
                cout << "\n";
            } else {
                // Recorrer la lista de recursos del dominio desde el más reciente al más antiguo
                HashNode* cur = domNode->head;
                while (cur != NULL) {
                    cout<< string(cur->path);
                    cur = cur->nextDomain;
                    if (cur != NULL) {
                        cout << " ";
                    }
                }
                cout<<  "\n";
            }
        }

        //pre:
        //post:
        void size(){
            cout << to_string(totalCount) + "\n";
        }

        //pre:
        //post:
        void clearDomain(const string& dom) {
            // Buscar dominio otra vez buscamos dominio funcion y funcion doublehash
            uint64_t h = hashDomain(dom);
            int idx = h % domainTableSize;
            int step = 1 + ((h >> 32) % (domainTableSize - 1));
            DomainNode* domNode = NULL;
            while (domainTable[idx] != NULL) {
                if (!domainTable[idx]->deleted && domainTable[idx]->domain == dom) {
                    domNode = domainTable[idx];
                    break;
                }
                idx = (idx + step) % domainTableSize;
                if (domainTable[idx] == NULL) break;
            }
            if (domNode == NULL || domNode->count == 0) {
                // Dominio no existe o ya está vacío
                return;
            }
            // Recorrer la lista enlazada de recursos en ese dominio y marcarlos como eliminados
            HashNode* cur = domNode->head;
            while (cur != NULL) {
                // Marcar el nodo actual como eliminado en la tabla principal
                uint64_t h2 = hashKey(domNode->domain, cur->path);
                int index = h2 % tableSize;
                int step2 = 1 + ((h2 >> 32) % (tableSize - 1));   // paso (double hashing)
                while (hashTable[index] != NULL) {
                    if (hashTable[index] == cur) {
                        hashTable[index]->deleted = true;
                        break;
                    }
                    index = (index + step2) % tableSize;
                }
                totalCount -= 1;
                // Avanzar al siguiente antes de modificar cur
                HashNode* toRemove = cur;
                cur = cur->nextDomain;
                // Desenlazar (opcional, para limpiar referencias)
                toRemove->nextDomain = NULL;
                toRemove->prevDomain = NULL;
            }
            // Resetear la lista del dominio
            domNode->head = domNode->tail = NULL;
            domNode->count = 0;
            // Marcar dominio como eliminado (tombstone) en su tabla
            domNode->deleted = true;
        }

        void clear() {
            // Liberar todos los HashNode y limpiar tabla de recursos
            for (int i = 0; i < tableSize; ++i) {
                if (hashTable[i] != NULL) {
                    delete hashTable[i];
                    hashTable[i] = NULL;
                }
            }
            // Liberar todos los DomainNode y limpiar tabla de dominios
            for (int i = 0; i < domainTableSize; ++i) {
                if (domainTable[i] != NULL) {
                    delete domainTable[i];
                    domainTable[i] = NULL;
                }
            }
            // Reiniciar contadores
            totalCount = 0;
        }
};


