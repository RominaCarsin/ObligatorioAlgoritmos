#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "tads/HeapListMerging.cpp"

using namespace std;


struct ListNode {
    int value;
    ListNode* sig;
    ListNode(int v) : value(v), sig(NULL) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;


    ListNode** listas = new ListNode*[N];
    for (int i = 0; i < N; i++) 
    {
        listas[i] = NULL;
        int N;
        cin >> N;
        ListNode* head = NULL;
        ListNode* last = NULL;
        for (int j = 0; j < N; j++) {
            int x;
            cin >>x;
            ListNode* nuevo = new ListNode(x);
            if (!head) {
                head = last = nuevo;
            } else {
                last->sig = nuevo;
                last = nuevo;
            }
        }
        listas[i] = head;
    }

  //creo el heap
   miniHeap<NodoInfo>* heap = new miniHeap<NodoInfo>(N);

    
    for (int i = 0; i < N; i++) {// Inserto el primer elemento de cada lista
        if (listas[i] != NULL) {
            NodoInfo nodo = {listas[i]->value, i, 0};
            heap->merge(nodo, nodo.value);
            listas[i] = listas[i]->sig;

        }
    }

    // hago que el heap vaya sacando el minimo y metiendo el siguiente de la lista correspondiente
    while (!heap->isEmpty()) {
        NodoInfo actual = heap->getMin();
        cout << actual.value << "\n";
        heap->unmerge();

        // Insertar el siguiente nodo de la lista correspondiente (si existe)      // darle una mirada
        if (listas[actual.listIndex] != NULL) {
            NodoInfo siguiente;
            siguiente.value = listas[actual.listIndex]->value;
            siguiente.listIndex = actual.listIndex;
            siguiente.elemList = actual.elemList + 1;
            heap->merge(siguiente, siguiente.value);
            listas[actual.listIndex] = listas[actual.listIndex]->sig;
        }
    }

    //creo que esta parte no es necesaria

    //  Liberar memoria
    // for (int i = 0; i < N; i++) {
    //     while (listas[i] != NULL) {
    //         ListNode* tmp = listas[i];
    //         listas[i] = listas[i]->sig;
    //         delete tmp;
    //     }
    // }
    // delete[] listas;

    return 0;
}
