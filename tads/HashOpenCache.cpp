#include <iostream>
#include <string>
#include <cstring>
using namespace std;

struct DomainNode;
struct MedioNode;

struct MedioNode {
    string domain;       
    string path;
    string title;
    int time;

    MedioNode* nextHash;    // siguiente en la tabla hash 
    MedioNode* nextDomain;  // siguiente en la lista de recursos del dominio

    MedioNode( string& d,  string& p,  string& t, int tm): domain(d), path(p), title(t), time(tm), nextHash(NULL), nextDomain(NULL) {}
};

struct DomainNode {
    string domain;
    MedioNode* recursos;   //el mas reciente
    int count;
    bool deleted;
    DomainNode* nextHash;  

    DomainNode( string& d): domain(d), recursos(NULL), count(0), deleted(false), nextHash(NULL) {}
};

class Cache {
private:
    MedioNode** table;      //recursos
    int tableSize;
    DomainNode** domTable;  //dominios
    int domSize;
    int totalCount;

    int hashStr( const string& s)  {
        int h;
        for (size_t i = 0; i < s.size(); ++i)
            h = ((h << 5) + h) + (char)s[i]; // h * 33 + c es para evitar overflow 
        return h;
    }

    int hashKey( const string& d,  const string& path)  {
        return hashStr(d + "/" + path) % tableSize;
    }

    int hashDomain(const string& d)  {
        return hashStr(d) % domSize;
    }

    // retorna el nodo de dominio o NULL si no existe
    DomainNode* findDomain( string& d)  {
        int idx = hashDomain(d);
        DomainNode* cur = domTable[idx];
        while (cur) {
            if (!cur->deleted && cur->domain == d) return cur;
            cur = cur->nextHash;
        }
        return NULL;
    }

    // retorna el dominio existente o crea uno nuevo al frente
    DomainNode* getOrCreateDomain( string& d) {
        int idx = hashDomain(d);
        DomainNode* cur = domTable[idx];
        while (cur) {
            if (!cur->deleted && cur->domain == d) return cur;
            cur = cur->nextHash;
        }
        // si no existe, crear nuevo dominio 
        DomainNode* nuevo = new DomainNode(d);
        nuevo->nextHash = domTable[idx];
        domTable[idx] = nuevo;
        return nuevo;
    }

    // desenlazar nodo de la lista de recursos del dominio 
    void unlinkFromDomain(DomainNode* domNode, MedioNode* node) {
        MedioNode* cur = domNode->recursos;
        MedioNode* prev = NULL;
        while (cur) {
            if (cur == node) {
                if (prev) prev->nextDomain = cur->nextDomain;
                else domNode->recursos = cur->nextDomain;
                return;
            }
            prev = cur;
            cur = cur->nextDomain;
        }
    }

    // desenlazar nodo de la tabla hash
    void unlinkFromBucket(int idx, MedioNode* node) {
        MedioNode* cur = table[idx];
        MedioNode* prev = NULL;
        while (cur) {
            if (cur == node) {
                if (prev) prev->nextHash = cur->nextHash;
                else table[idx] = cur->nextHash;
                return;
            }
            prev = cur;
            cur = cur->nextHash;
        }
    }

public:
    Cache(int maxOps) {
        tableSize = maxOps * 2 + 1;
        domSize = maxOps * 2 + 1;
        table = new MedioNode*[tableSize];
        domTable = new DomainNode*[domSize];
        for (int i = 0; i < tableSize; ++i) table[i] = NULL;
        for (int i = 0; i < domSize; ++i) domTable[i] = NULL;
        totalCount = 0;
    }

    ~Cache() { //chatgpt
        clear();
        // liberar tablas
        delete [] table;
        delete [] domTable;
    }

   
    void put( string& d,  string& path,  string& title, int time) {
        DomainNode* domNode = getOrCreateDomain(d);
        int idx = hashKey(d, path);

        MedioNode* cur = table[idx];
        while (cur) {
            if (cur->domain == d && cur->path == path) {
                cur->title = title;
                cur->time = time;
                if (domNode->recursos != cur) {
                    unlinkFromDomain(domNode, cur);            
                    cur->nextDomain = domNode->recursos;
                    domNode->recursos = cur;
                }
                return;
            }
            cur = cur->nextHash;
        }

        // Si no existe se crea un nuevo nodo se coloca al frente 
        MedioNode* nuevo = new MedioNode(d, path, title, time);
        nuevo->nextHash = table[idx];
        table[idx] = nuevo;
        nuevo->nextDomain = domNode->recursos;
        domNode->recursos = nuevo;
        domNode->count += 1;
        totalCount += 1;
        domNode->deleted = false; // si antes estaba borrado, se vuelve a marcar como activo
    }

    void get( string& d,  string& path) {
        DomainNode* domNode = findDomain(d);
        if (!domNode) {
            cout << "recurso_no_encontrado\n";
            return;
        }
        int idx = hashKey(d, path);
        MedioNode* cur = table[idx];
        while (cur) {
            if (cur->domain == d && cur->path == path) {
                cout << cur->title << " " << cur->time << "\n";
                return;
            }
            cur = cur->nextHash;
        }
        cout << "recurso_no_encontrado\n";
    }

    void contains( string& d,  string& path) {
        DomainNode* domNode = findDomain(d);
        if (!domNode) { cout << "false\n"; return; }
        int idx = hashKey(d, path);
        MedioNode* cur = table[idx];
        while (cur) {
            if (cur->domain == d && cur->path == path) {
                cout << "true\n";
                return;
            }
            cur = cur->nextHash;
        }
        cout << "false\n";
    }

    void remove( string& d,  string& path) {
        DomainNode* domNode = findDomain(d);
        if (!domNode) return;
        int idx = hashKey(d, path);

        MedioNode* cur = table[idx];
        MedioNode* found = NULL;
        while (cur) {
            if (cur->domain == d && cur->path == path) {
                found = cur;
                break;
            }
            cur = cur->nextHash;
        }
        if (!found) return;

        unlinkFromDomain(domNode, found);
        domNode->count -= 1;

        unlinkFromBucket(idx, found);

        delete found;
        totalCount -= 1;

        if (domNode->count == 0) domNode->deleted = true;
    }

    void countDomain( string& d) {
        DomainNode* domNode = findDomain(d);
        if (!domNode || domNode->deleted) cout << "0\n";
        else cout << domNode->count << "\n";
    }

    void listDomain( string& d) {
        DomainNode* domNode = findDomain(d);
        if (!domNode || domNode->deleted || domNode->count == 0) {
            cout << "\n";
            return;
        }
        bool first = true;
        MedioNode* cur = domNode->recursos;
        while (cur) {
            if (!first) cout << " ";
            cout << cur->path;
            first = false;
            cur = cur->nextDomain;
        }
        cout << "\n";
    }


    void size() {
        cout << totalCount << "\n";
    }

    void clearDomain( string& d) {
        DomainNode* domNode = findDomain(d);
        if (!domNode || domNode->deleted) return;

        MedioNode* cur = domNode->recursos;
        while (cur) {
            MedioNode* next = cur->nextDomain;
    
            int idx = hashKey(cur->domain, cur->path);
            unlinkFromBucket(idx, cur);
            delete cur;
            totalCount -= 1;
            cur = next;
        }
        domNode->recursos = NULL;
        domNode->count = 0;
        domNode->deleted = true;
    }

    void clear() {
        // borra todos los nodos de recursos
        for (int i = 0; i < tableSize; ++i) {
            MedioNode* cur = table[i];
            while (cur) {
                MedioNode* next = cur->nextHash;
                delete cur;
                cur = next;
            }
            table[i] = NULL;
        }
        // borra todos los dominios
        for (int i = 0; i < domSize; ++i) {
            DomainNode* cur = domTable[i];
            while (cur) {
                DomainNode* next = cur->nextHash;
                delete cur;
                cur = next;
            }
            domTable[i] = NULL;
        }
        totalCount = 0;
    }
};


