#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

const int MAXN = 1000;
const int HASH_SIZE = 1000007; // Prime number for hash table

// left and right index (for all combinatios), k amount of values of the same cristal, like when delete a block   
struct State {
    int l, r, k;
    int value;
    State* next;
    
    State(int _l, int _r, int _k, int _v) : l(_l), r(_r), k(_k), value(_v), next(nullptr) {}
};

// Hash table for memoization
State* hashTable[HASH_SIZE];
int cris[MAXN];

int getHash(int l, int r, int k) {
    long long h = ((long long)l * 1000000LL + (long long)r * 1000LL + k) % HASH_SIZE;
    if (h < 0) h += HASH_SIZE;
    return h;
}

int getValue(int l, int r, int k) {
    int h = getHash(l, r, k);
    State* curr = hashTable[h];
    while (curr != nullptr) {
        if (curr->l == l && curr->r == r && curr->k == k) {
            return curr->value;
        }
        curr = curr->next;
    }
    return -1;
}

void setValue(int l, int r, int k, int value) {
    int h = getHash(l, r, k);
    State* newState = new State(l, r, k, value);
    newState->next = hashTable[h];
    hashTable[h] = newState;
}

int maxPoints(int l, int r, int k, int n) {
    if (l > r) return 0;
    
    int cached = getValue(l, r, k);
    if (cached != -1) return cached;
    
    // Opción 1: eliminar el bloque final (cris[r] + k extras)
    int res = maxPoints(l, r - 1, 0, n) + (k + 1) * (k + 1);
    
    // Opción 2: juntar cris[r] con algún cris[i] del mismo color
    for (int i = l; i < r; i++) {
        if (cris[i] == cris[r]) {
            int cand = maxPoints(l, i, k + 1, n) + maxPoints(i + 1, r - 1, 0, n);
            if (cand > res) res = cand;
        }
    }
    
    setValue(l, r, k, res);
    return res;
}

void cleanupHash() {
    for (int i = 0; i < HASH_SIZE; i++) {
        State* curr = hashTable[i];
        while (curr != nullptr) {
            State* temp = curr;
            curr = curr->next;
            delete temp;
        }
        hashTable[i] = nullptr;
    }
}

int main() {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> cris[i];
    }
    
    // Initialize hash table
    for (int i = 0; i < HASH_SIZE; i++) {
        hashTable[i] = nullptr;
    }
    
    int ans = maxPoints(0, n - 1, 0, n);
    cout << ans << endl;
    
    cleanupHash();
    return 0;
}