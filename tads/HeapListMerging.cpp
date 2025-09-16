#include <cassert>
#include <string>
#include <iostream>
#include <limits>
//es casi identico a lo hecho por el profesor, revisarlo
struct NodoInfo{
    int value;
    int listIndex;
    int elemList;
};

template<class T>
class HeapNode {
    public:
        T data;
        int priority;

        HeapNode(T data, int priority): data(data), priority(priority) {}
};

template <class T>
class miniHeap{
    private:
        HeapNode<T>** heap;
        int size;
        int capacity;

        int getPadre(int pos) {
            return (pos - 1) / 2;
        }

        int getHijoIzq(int pos) {
            return 2 * pos + 1;
        }

        int getHijoDer(int pos) {
            return 2 * pos + 2;
        }

        void flotar (HeapNode<T>** heap, int pos){
            int posPadre = getPadre(pos);
            if (pos > 0 && heap[pos]->priority < heap[posPadre]->priority) {
                HeapNode<T>* aux = heap[pos];
                heap[pos] = heap[posPadre];
                heap[posPadre] = aux;
                flotar(heap, posPadre);
            }
        }

        void hundir(HeapNode<T>** heap, int pos, int size){
            int posCandidato = getHijoIzq(pos);
            if (posCandidato < size) {
                if (posCandidato + 1 < size && heap[posCandidato + 1]->priority <= heap[posCandidato]->priority) {
                    posCandidato++;
                }
                if (heap[pos]->priority >= heap[posCandidato]->priority) {
                    HeapNode<T>* aux = heap[pos];
                    heap[pos] = heap[posCandidato];
                    heap[posCandidato] = aux;
                    hundir(heap, posCandidato, size);
                }
            }
        }

    public:
        miniHeap(int capacity) {
            this->capacity = capacity;
            this->size = 0;
            this->heap = new HeapNode<T>*[capacity];
            for (int i = 0; i < capacity; i++) {
                heap[i] = NULL;
            }
        }

        ~miniHeap() {
            for (int i = 0; i < size; i++) {
                delete heap[i];
            }
            delete[] heap;
        }

        bool isEmpty(){
            return size == 0;
        }

        T getMin() {
            return heap[0]->data;
        }

        void merge(T data, int priority) {
            heap[size] = new HeapNode<T>(data, priority);
            flotar(heap, size++);
        }

        // Pre: !esVacia(h)
        void unmerge(){
            delete heap[0];
            heap[0] = heap[--size];
            heap[size] = nullptr;
            hundir(heap, 0, size);
        }
};
