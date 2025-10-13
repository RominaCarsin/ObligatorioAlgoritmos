#include <iostream>
#include <string>
#include <cmath>
using namespace std;

struct DomainNode;
struct HashNode;

struct HashNode {
    DomainNode* domainNode;
    string path;
    string title;
    int time;
    HashNode* nextDomain;
    HashNode* prevDomain;
    bool deleted;

    HashNode(DomainNode* d, const string& p, const string& t, int tim) {
        domainNode = d;
        path = p;
        title = t;
        time = tim;
        deleted = false;
        nextDomain = prevDomain = NULL;
    }
};

struct DomainNode {
    string domain;
    HashNode* head;
    HashNode* tail;
    int count;
    bool deleted;

    DomainNode(): domain(""), head(NULL), tail(NULL), count(0), deleted(false) {}
    DomainNode(const string& dom): domain(dom), head(NULL), tail(NULL), count(0), deleted(false) {}
};

class Cache {
private:
    HashNode** hashTable;
    DomainNode** domainTable;
    int tableSize;
    int domainTableSize;
    int totalCount;

    bool esPrimo(int n) {
        if (n <= 1) return false;
        if (n == 2) return true;
        if (n % 2 == 0) return false;
        for (int i = 3; i * i <= n; i += 2)
            if (n % i == 0) return false;
        return true;
    }

    int primoSupMinimo(int val) {
        if (val <= 2) return 2;
        if (val % 2 == 0) val++;
        while (!esPrimo(val)) val += 2;
        return val;
    }

    int h1(const string& s) {
        unsigned long h = 0;
        for (char c : s) h = (h * 131 + c) % 2147483647;
        return (int)h;
    }

    int h2(const string& s) {
        unsigned long h = 0;
        for (char c : s) h = (h * 137 + c) % 2147483647;
        return (int)(h * 2 + 1);
    }

public:
    Cache(int maxOps) {
        tableSize = primoSupMinimo(maxOps);
        domainTableSize = primoSupMinimo(maxOps);
        hashTable = new HashNode*[tableSize];
        domainTable = new DomainNode*[domainTableSize];
        for (int i = 0; i < tableSize; ++i) hashTable[i] = NULL;
        for (int i = 0; i < domainTableSize; ++i) domainTable[i] = NULL;
        totalCount = 0;
    }

    void put(const string& dom, const string& path, const string& tit, int tim) {
        int h = h1(dom);
        int idx = h % domainTableSize;
        int step = h2(dom) % domainTableSize;
        if (step == 0) step = 1;

        DomainNode* domainNode = nullptr;
        int firstDeleted = -1;

        for (int i = 0; i < domainTableSize; ++i) {
            int pos = (idx + i * step) % domainTableSize;
            if (domainTable[pos] == NULL) {
                if (firstDeleted != -1) pos = firstDeleted;
                domainTable[pos] = new DomainNode(dom);
                domainNode = domainTable[pos];
                break;
            } else if (domainTable[pos]->deleted) {
                if (firstDeleted == -1) firstDeleted = pos;
            } else if (domainTable[pos]->domain == dom) {
                domainNode = domainTable[pos];
                break;
            }
        }
        if (!domainNode) {
            domainTable[firstDeleted] = new DomainNode(dom);
            domainNode = domainTable[firstDeleted];
        }

        string key = dom + "|" + path;
        int hkey = h1(key);
        int stepkey = h2(key) % tableSize;
        if (stepkey == 0) stepkey = 1;

        int hashIdx = hkey % tableSize;
        int firstDel = -1;
        HashNode* node = nullptr;

        for (int i = 0; i < tableSize; ++i) {
            int pos = (hashIdx + i * stepkey) % tableSize;
            if (hashTable[pos] == NULL) {
                if (firstDel != -1) pos = firstDel;
                hashTable[pos] = new HashNode(domainNode, path, tit, tim);
                node = hashTable[pos];
                totalCount++;
                node->nextDomain = domainNode->head;
                if (domainNode->head) domainNode->head->prevDomain = node;
                domainNode->head = node;
                if (!domainNode->tail) domainNode->tail = node;
                domainNode->count++;
                break;
            } else if (hashTable[pos]->deleted) {
                if (firstDel == -1) firstDel = pos;
            } else if (!hashTable[pos]->deleted &&
                       hashTable[pos]->domainNode == domainNode &&
                       hashTable[pos]->path == path) {
                hashTable[pos]->title = tit;
                hashTable[pos]->time = tim;
                node = hashTable[pos];
                break;
            }
        }

        if (!node && firstDel != -1) {
            hashTable[firstDel] = new HashNode(domainNode, path, tit, tim);
            node = hashTable[firstDel];
            totalCount++;
            node->nextDomain = domainNode->head;
            if (domainNode->head) domainNode->head->prevDomain = node;
            domainNode->head = node;
            if (!domainNode->tail) domainNode->tail = node;
            domainNode->count++;
        }
    }

    void get(const string& dom, const string& path) {
        int h = h1(dom);
        int idx = h % domainTableSize;
        int step = h2(dom) % domainTableSize;
        if (step == 0) step = 1;

        DomainNode* domainNode = NULL;
        for (int i = 0; i < domainTableSize; ++i) {
            int pos = (idx + i * step) % domainTableSize;
            if (domainTable[pos] == NULL) break; 
            if (!domainTable[pos]->deleted && domainTable[pos]->domain == dom) {
                domainNode = domainTable[pos];
                break;
            }
        }

        if (domainNode == NULL) {
            cout << "recurso_no_encontrado" << endl;
            return;
        }

        // Busco el path dentro de la tabla de recursos (h2)
        string key = dom + "|" + path;
        int hkey = h1(key);
        int stepkey = h2(key);
        if (stepkey == 0) stepkey = 1;
        int hashIdx = hkey % tableSize;

        for (int i = 0; i < tableSize; ++i) {
            int pos = (hashIdx + i * stepkey) % tableSize;
            if (hashTable[pos] == NULL) break; // No está
            if (!hashTable[pos]->deleted && hashTable[pos]->domainNode == domainNode && hashTable[pos]->path == path) { // Recurso encontrado
                cout << hashTable[pos]->title << " " << hashTable[pos]->time << endl;
                return;
            }
        }
        cout << "recurso_no_encontrado" << endl;
    }

    
    void remove(const string& dom, const string& path) {
        int h = h1(dom);
        int idx = h % domainTableSize;
        int step = h2(dom) % domainTableSize;
        if (step == 0) step = 1;

        DomainNode* domainNode = nullptr;
        for (int i = 0; i < domainTableSize; ++i) {
            int pos = (idx + i * step) % domainTableSize;
            if (domainTable[pos] == NULL) break;
            if (!domainTable[pos]->deleted && domainTable[pos]->domain == dom) {
                domainNode = domainTable[pos];
                break;
            }
        }
        if (!domainNode) return;

        // Buscar en la lista enlazada del dominio (O(K) promedio)
        HashNode* node = domainNode->head;
        while (node) {
            if (!node->deleted && node->path == path) {
                node->deleted = true;
                totalCount--;
                // Quitar de la lista enlazada del dominio
                if (node->prevDomain) node->prevDomain->nextDomain = node->nextDomain;
                if (node->nextDomain) node->nextDomain->prevDomain = node->prevDomain;
                if (domainNode->head == node) domainNode->head = node->nextDomain;
                if (domainNode->tail == node) domainNode->tail = node->prevDomain;
                domainNode->count--;
                if (domainNode->count == 0) {
                    domainNode->deleted = true;
                }
                break;
            }
            node = node->nextDomain;
        }
    }

    void contains(const string& dom, const string& path) {
        int h = h1(dom);
        int idx = h % domainTableSize;
        int step = h2(dom) % domainTableSize;
        if (step == 0) step = 1;

        DomainNode* domainNode = NULL;
        for (int i = 0; i < domainTableSize; ++i) {
            int pos = (idx + i * step) % domainTableSize;
            if (domainTable[pos] == NULL) break; 
            if (!domainTable[pos]->deleted && domainTable[pos]->domain == dom) {
                domainNode = domainTable[pos];
                break;
            }
        }

        if (domainNode == NULL) {
            cout << "false" << endl;
            return;
        }

        // Busco el path dentro de la tabla de recursos (h2)
        string key = dom + "|" + path;
        int hkey = h1(key);
        int stepkey = h2(key);
        if (stepkey == 0) stepkey = 1;
        int hashIdx = hkey % tableSize;

        for (int i = 0; i < tableSize; ++i) {
            int pos = (hashIdx + i * stepkey) % tableSize;
            if (hashTable[pos] == NULL) break; // No está
            if (!hashTable[pos]->deleted && hashTable[pos]->domainNode == domainNode && hashTable[pos]->path == path) { // Recurso encontrado
                cout << "true" << endl;
                return;
            }
        }
        cout << "false" << endl;
    }

    int countDomain(const string& dom) {
        int h = h1(dom);
        int idx = h % domainTableSize;
        int step = h2(dom) % domainTableSize;
        if (step == 0) step = 1;

        DomainNode* domainNode = NULL;
        for (int i = 0; i < domainTableSize; ++i) {
            int pos = (idx + i * step) % domainTableSize;
            if (domainTable[pos] == NULL) break; 
            if (!domainTable[pos]->deleted && domainTable[pos]->domain == dom) {
                domainNode = domainTable[pos];
                break;
            }
        }

        if (domainNode == NULL) {
            cout << 0 << endl;
            return 0;
        }
        cout << domainNode->count << endl;
        return domainNode->count;
    }

    void listDomain(const string& dom) {
        int h = h1(dom);
        int idx = h % domainTableSize;
        int step = h2(dom) % domainTableSize;
        if (step == 0) step = 1;

        DomainNode* domainNode = NULL;
        for (int i = 0; i < domainTableSize; ++i) {
            int pos = (idx + i * step) % domainTableSize;
            if (domainTable[pos] == NULL) break; 
            if (!domainTable[pos]->deleted && domainTable[pos]->domain == dom) {
                domainNode = domainTable[pos];
                break;
            }
        }

        if (domainNode == NULL || domainNode->count == 0) {
            cout << endl;// lo de recurso no encontrado no va aca no es necesario para esto
            return;
        }

        HashNode* node = domainNode->head;
        while (node) {
            if (!node->deleted) {
                cout << node->path << " " << node->title << " " << node->time << endl;
            }
            node = node->nextDomain;
        }
    }

    void clearDomain(const string& dom) {
        int h = h1(dom);
        int idx = h % domainTableSize;
        int step = h2(dom) % domainTableSize;
        if (step == 0) step = 1;

        DomainNode* domainNode = nullptr;
        for (int i = 0; i < domainTableSize; ++i) {
            int pos = (idx + i * step) % domainTableSize;
            if (domainTable[pos] == NULL) break;
            if (!domainTable[pos]->deleted && domainTable[pos]->domain == dom) {
                domainNode = domainTable[pos];
                break;
            }
        }
        if (!domainNode) return;

        // Marcar todos los recursos del dominio como eliminados
        HashNode* node = domainNode->head;
        while (node) {
            if (!node->deleted) {
                node->deleted = true;
                totalCount--;
            }
            node = node->nextDomain;
        }
        domainNode->head = domainNode->tail = NULL;
        // totalCount -= domainNode->count;
        domainNode->count = 0;
        domainNode->deleted = true;
    }

    int size() {
        cout << totalCount << endl;
        return totalCount;
    }

    void clear() {
        for (int i = 0; i < tableSize; ++i) {
            if (hashTable[i]) {
                delete hashTable[i];
                hashTable[i] = NULL;
            }
        }
        for (int i = 0; i < domainTableSize; ++i) {
            if (domainTable[i]) {
                delete domainTable[i];
                domainTable[i] = NULL;
            }
        }
        totalCount = 0;
    }
};
