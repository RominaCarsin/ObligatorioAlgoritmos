#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

int main()
{
    return 0;
}


struct Jugador{
    NodoAVL* raiz;
    int cantElem;
    NodoAVL*top1;
    Jugador()  : raiz(NULL), top1(NULL), cantElem(0){}
};


struct NodoAVL {
    string nombre;
    int puntaje;
    int id;
    int fVal;
    NodoAVL *izq; 
    NodoAVL *der;
    
    NodoAVL(string nom, int p, int id):nombre(nom), puntaje (p), id(id), fVal(0), izq(NULL), der(NULL){}
};

void ADD (Jugador *& j, string nombre, int puntos, int id){
    bool created = ADDPointTree(j->raiz, nombre, puntos,id, false);
    NodoAVL* ret = ADDId(j->raiz,  nombre,  puntos, id, false);
    
    if (created){
        j->cantElem++;
    }
    if(j->top1 == NULL){
        j->top1 = ret;
    }else if(j->top1->puntaje < ret->puntaje){
        j->top1 = ret;
    }
    
}

bool ADDPointTree (NodoAVL *&a, string nombre, int puntos, int id, bool varioAltura){
    bool insertado;
    if (a == NULL)
    {
        a = new NodoAVL(nombre, puntos, id);
        varioAltura = true;
        return true;
    }
    if (puntos < a->puntaje)
    {
        insertado = ADDPointTree (a->izq, nombre, puntos, id, varioAltura);
        if (varioAltura)
        {
            switch (a->fVal)
            {
                case 1:
                    a->fVal = 0;
                    varioAltura = false;
                    break;
                case 0:
                    a->fVal = -1;
                    break;
                case -1:
                    NodoAVL *p1 = a->izq;
                    if(p1->fVal == -1){
                        a->izq = p1->der;
                        p1->der = a;
                        a->fVal = 0;
                        a = p1;
                    } else {
                        NodoAVL *p2 = p1->der;
                        p1->der = p2->izq;
                        p2->izq = p1;
                        a->izq = p2->der;
                        p2->der = a;
                        a->fVal = p2->fVal == -1 ? 1 : 0;
                        p1->fVal = p2->fVal == 1 ? -1 : 0;
                        a = p2;
                    }
                    a->fVal = 0;
                    varioAltura = false;
                    break;
            }
            varioAltura = false;
        }
        return insertado;
    }
    else if (puntos > a->puntaje)
    {
        insertado = ADDPointTree(a->der, nombre, puntos, id, varioAltura);
        if (varioAltura)
        {
            switch (a->fVal)
            {
                case -1:
                    a->fVal = 0;
                    varioAltura = false;
                    break;
                case 0:
                    a->fVal = 1;
                    break;
                case 1:
                    NodoAVL *p1 = a->der;
                    if(p1->fVal == 1){
                        a->der = p1->izq;
                        p1->izq = a;
                        a->fVal = 0;
                        a = p1;
                    } else {
                        NodoAVL *p2 = p1->izq;
                        p1->izq = p2->der;
                        p2->der = p1;
                        a->der = p2->izq;
                        p2->izq = a;
                        a->fVal = p2->fVal == 1 ? -1 : 0;
                        p1->fVal = p2->fVal == -1 ? 1 : 0;
                        a = p2;
                        return insertado;
                    }
                    a->fVal = 0;
                    varioAltura = false;
                    break;
            }
            varioAltura = false;
        }
        return insertado;
    } else return false;
}

NodoAVL* ADDId (NodoAVL *&a, string nombre, int puntos, int id, bool varioAltura){
    if (a == NULL)
    {
        a = new NodoAVL(nombre, puntos, id);
        varioAltura = true;
        return;
    }
    if (id < a->id)
    {
        ADDId (a->izq, nombre, puntos, id, varioAltura);
        if (varioAltura)
        {
            switch (a->fVal)
            {
                case 1:
                    a->fVal = 0;
                    varioAltura = false;
                    break;
                case 0:
                    a->fVal = -1;
                    break;
                case -1:
                    NodoAVL *p1 = a->izq;
                    if(p1->fVal == -1){
                        a->izq = p1->der;
                        p1->der = a;
                        a->fVal = 0;
                        a = p1;
                    } else {
                        NodoAVL *p2 = p1->der;
                        p1->der = p2->izq;
                        p2->izq = p1;
                        a->izq = p2->der;
                        p2->der = a;
                        a->fVal = p2->fVal == -1 ? 1 : 0;
                        p1->fVal = p2->fVal == 1 ? -1 : 0;
                        a = p2;
                    }
                    a->fVal = 0;
                    varioAltura = false;
                    break;
            }
            varioAltura = false;
        }
        return;
    }
    else if (id > a->id)
    {
        ADDId(a->der, nombre, id, id, varioAltura);
        if (varioAltura)
        {
            switch (a->fVal)
            {
                case -1:
                    a->fVal = 0;
                    varioAltura = false;
                    break;
                case 0:
                    a->fVal = 1;
                    break;
                case 1:
                    NodoAVL *p1 = a->der;
                    if(p1->fVal == 1){
                        a->der = p1->izq;
                        p1->izq = a;
                        a->fVal = 0;
                        a = p1;
                    } else {
                        NodoAVL *p2 = p1->izq;
                        p1->izq = p2->der;
                        p2->der = p1;
                        a->der = p2->izq;
                        p2->izq = a;
                        a->fVal = p2->fVal == 1 ? -1 : 0;
                        p1->fVal = p2->fVal == -1 ? 1 : 0;
                        a = p2;
                        return;
                    }
                    a->fVal = 0;
                    varioAltura = false;
                    break;
            }
            varioAltura = false;
        }
        return;
    } else return;
}

void FIND (NodoAVL *&a, int id){
    if(!a){
        cout<< "jugador_no_encontrado" << endl;
        return;
    }
    if(a->id == id){
        cout<< a->nombre << " " << a->puntaje << endl;
    }else if (a->id < id){
        FIND(a->der, id);
    }else{
        FIND(a->izq, id);
    }
}

int RANK (NodoAVL *&a, int puntaje, int cantidad){
    if(!a){
        return cantidad;
    }
    if(a->puntaje >= puntaje){
       cantidad++;
        RANK(a->der, puntaje, cantidad);
    }else{
        RANK(a->izq, puntaje, cantidad);
    }
}

NodoAVL* TOP1(Jugador* j){
    return j->top1;
}

int CANT (Jugador* j){
    return j->cantElem;
}