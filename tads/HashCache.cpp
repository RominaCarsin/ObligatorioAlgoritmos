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
        int idx  = (int)((unsigned long long)h1(dom) % (unsigned long long)domainTableSize);
        int step = 1 + (int)((unsigned long long)h2(dom) % (unsigned long long)(domainTableSize - 1));

        DomainNode* domainNode = nullptr;
        int firstDeleted = -1;

        for (int i = 0; i < domainTableSize; ++i) {
            int pos = (int)(((long long)idx + (long long)i * step) % (long long)domainTableSize);
            if (domainTable[pos] == NULL) {
                int usePos = pos;
                if (firstDeleted != -1)
                {
                    delete domainTable[firstDeleted];
                    usePos = firstDeleted;
                }
                domainTable[usePos] = new DomainNode(dom);
                domainNode = domainTable[usePos];
                break;
            } else if (domainTable[pos]->deleted) {
                if (firstDeleted == -1) firstDeleted = pos;
            } else if (domainTable[pos]->domain == dom) {
                domainNode = domainTable[pos];
                break;
            }
        }
        if (!domainNode) {
            if (firstDeleted != -1) {
                delete domainTable[firstDeleted];
                domainTable[firstDeleted] = new DomainNode(dom);
                domainNode = domainTable[firstDeleted];
            } else {
                domainTable[idx] = new DomainNode(dom);
                domainNode = domainTable[idx];
            }
        }

        string key = dom + "|" + path;
        int hashIdx = (int)((unsigned long long)h1(key) % (unsigned long long)tableSize);
        int stepkey = 1 + (int)((unsigned long long)h2(key) % (unsigned long long)(tableSize - 1));

        int firstDel = -1;
        HashNode* node = nullptr;

        for (int i = 0; i < tableSize; ++i) {
            int pos = (int)(((long long)hashIdx + (long long)i * stepkey) % (long long)tableSize);
            if (hashTable[pos] == NULL) {
                int insertPos = (firstDel != -1 ? firstDel : pos);
                if (firstDel != -1) {
                    delete hashTable[firstDel];
                }
                hashTable[insertPos] = new HashNode(domainNode, path, tit, tim);
                node = hashTable[insertPos];
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
                HashNode* found = node;
                if (found->prevDomain) found->prevDomain->nextDomain = found->nextDomain;
                if (found->nextDomain) found->nextDomain->prevDomain = found->prevDomain;
                if (domainNode->head == found) domainNode->head = found->nextDomain;
                if (domainNode->tail == found) domainNode->tail = found->prevDomain;
                found->prevDomain = NULL;
                found->nextDomain = domainNode->head;
                if (domainNode->head) domainNode->head->prevDomain = found;
                domainNode->head = found;
                if (!domainNode->tail) domainNode->tail = found;
                break;
            }
        }

        if (!node && firstDel != -1) {
            delete hashTable[firstDel];
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
        int idx  = (int)((unsigned long long)h1(dom) % (unsigned long long)domainTableSize);
        int step = 1 + (int)((unsigned long long)h2(dom) % (unsigned long long)(domainTableSize - 1));


        DomainNode* domainNode = NULL;
        for (int i = 0; i < domainTableSize; ++i) {
            int pos = (int)(((long long)idx + (long long)i * step) % (long long)domainTableSize);
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
        int hashIdx = (int)((unsigned long long)h1(key) % (unsigned long long)tableSize);
        int stepkey = 1 + (int)((unsigned long long)h2(key) % (unsigned long long)(tableSize - 1));


        for (int i = 0; i < tableSize; ++i) {
            int pos = (int)(((long long)hashIdx + (long long)i * stepkey) % (long long)tableSize);
            if (hashTable[pos] == NULL) break;
            if (!hashTable[pos]->deleted &&
                hashTable[pos]->domainNode == domainNode &&
                hashTable[pos]->path == path) {
                cout << hashTable[pos]->title << " " << hashTable[pos]->time << endl;
                return;
            }
        }

        cout << "recurso_no_encontrado" << endl;
    }

    
    void remove(const string& dom, const string& path) {
        int idx  = (int)((unsigned long long)h1(dom) % (unsigned long long)domainTableSize);
        int step = 1 + (int)((unsigned long long)h2(dom) % (unsigned long long)(domainTableSize - 1));


        DomainNode* domainNode = nullptr;
        for (int i = 0; i < domainTableSize; ++i) {
            int pos = (int)(((long long)idx + (long long)i * step) % (long long)domainTableSize);
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
        int idx  = (int)((unsigned long long)h1(dom) % (unsigned long long)domainTableSize);
        int step = 1 + (int)((unsigned long long)h2(dom) % (unsigned long long)(domainTableSize - 1));

        DomainNode* domainNode = NULL;
        for (int i = 0; i < domainTableSize; ++i) {
            int pos = (int)(((long long)idx + (long long)i * step) % (long long)domainTableSize);
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
        int hashIdx = (int)((unsigned long long)h1(key) % (unsigned long long)tableSize);
        int stepkey = 1 + (int)((unsigned long long)h2(key) % (unsigned long long)(tableSize - 1));


        for (int i = 0; i < tableSize; ++i) {
            int pos = (int)(((long long)hashIdx + (long long)i * stepkey) % (long long)tableSize);
            if (hashTable[pos] == NULL) break; // No está
            if (!hashTable[pos]->deleted && hashTable[pos]->domainNode == domainNode && hashTable[pos]->path == path) { // Recurso encontrado
                cout << "true" << endl;
                return;
            }
        }
        cout << "false" << endl;
    }

    int countDomain(const string& dom) {
        int idx  = (int)((unsigned long long)h1(dom) % (unsigned long long)domainTableSize);
        int step = 1 + (int)((unsigned long long)h2(dom) % (unsigned long long)(domainTableSize - 1));


        DomainNode* domainNode = NULL;
        for (int i = 0; i < domainTableSize; ++i) {
            int pos = (int)(((long long)idx + (long long)i * step) % (long long)domainTableSize);
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
        int idx  = (int)((unsigned long long)h1(dom) % (unsigned long long)domainTableSize);
        int step = 1 + (int)((unsigned long long)h2(dom) % (unsigned long long)(domainTableSize - 1));

        DomainNode* domainNode = NULL;
        for (int i = 0; i < domainTableSize; ++i) {
            int pos = (int)(((long long)idx + (long long)i * step) % (long long)domainTableSize);
            if (domainTable[pos] == NULL) break; 
            if (!domainTable[pos]->deleted && domainTable[pos]->domain == dom) {
                domainNode = domainTable[pos];
                break;
            }
        }

        if (domainNode == NULL || domainNode->count == 0) {
            cout << endl;
            return;
        }

        HashNode* node = domainNode->head;
        bool first = true;
        while (node) {
            if (!node->deleted) {
                if (!first) cout << ' ';
                cout << node->path;
                first = false;
            }
            node = node->nextDomain;
        }
        cout << endl;
    }

    void clearDomain(const string& dom) {
        int idx  = (int)((unsigned long long)h1(dom) % (unsigned long long)domainTableSize);
        int step = 1 + (int)((unsigned long long)h2(dom) % (unsigned long long)(domainTableSize - 1));


        DomainNode* domainNode = nullptr;
        for (int i = 0; i < domainTableSize; ++i) {
            int pos = (int)(((long long)idx + (long long)i * step) % (long long)domainTableSize);
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
