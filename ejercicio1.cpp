#include <cassert>
#include <string>
#include <iostream>
#include <limits>


using namespace std;


//Hacer nodoPuntaje nodoId
// se puede reducir la cantidad retirando countsub de este y haciendo otra solo para el de coutnsub 
struct NodoAVL {
    string nombre;
    int puntaje;
    int id;
    int fVal;
    NodoAVL *izq; 
    NodoAVL *der;
    int countSub;
    
    NodoAVL(string nom, int p, int id):nombre(nom), puntaje (p), id(id), fVal(0), izq(NULL), der(NULL), countSub(1){}
};

// representacion avl
struct Jugador{
    NodoAVL* pointsTree;
    NodoAVL* idTree;
    int cantElem;
    NodoAVL*top1;
    Jugador()  : pointsTree(NULL), idTree(NULL),top1(NULL), cantElem(0){}
};

void updateCount(NodoAVL* node) {
    if (node) {
        node->countSub = 1;
        if (node->izq) node->countSub += node->izq->countSub;
        if (node->der) node->countSub += node->der->countSub;
    }
}

void rotacionDerecha(NodoAVL *&a) {
    NodoAVL *b = a->izq;
    a->izq = b->der;
    b->der = a;
    a = b;
}

void rotacionIzquierda(NodoAVL *&a) {
    NodoAVL *b = a->der;
    a->der = b->izq;
    b->izq = a;
    a = b;
}

bool ADDId (NodoAVL *&a, string nombre, int puntos, int id, bool& varioAltura){
    bool insertado= false;
    if (a == NULL)
    {
        a = new NodoAVL(nombre, puntos, id);
        varioAltura = true;
        return true;
    }
    if (id < a->id)
    {
        insertado = ADDId (a->izq, nombre, puntos, id, varioAltura);
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
                        rotacionDerecha(a);
                        a->fVal = 0;
                        a->der->fVal = 0;
                    } else {
                        NodoAVL *p2 = p1->der;
                        rotacionIzquierda(a->izq);
                        rotacionDerecha(a);
                        if (p2->fVal == -1){
                            a->der->fVal = 1;
                            a->izq->fVal = 0;
                        }
                        else if (p2->fVal == 1){
                            a->izq->fVal = -1;
                            a->der->fVal = 0;
                        }
                        else{
                            a->izq->fVal = 0;
                            a->der->fVal = 0; }
                            a->fVal = 0;
                    }
                    a->fVal = 0;
                    varioAltura = false;
                    break;
            }
            if (a->izq) updateCount(a->izq);
            if (a->der) updateCount(a->der);
            updateCount(a);
            varioAltura = false;
        }
        updateCount(a);
        return insertado;
    }
    else if (id > a->id)
    {
        insertado = ADDId(a->der, nombre, puntos, id, varioAltura);
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
                        rotacionIzquierda(a);
                        a->fVal = 0;
                        a->izq->fVal = 0;
                    } else {
                        NodoAVL *p2 = p1->izq;
                        rotacionDerecha(a->der);
                        rotacionIzquierda(a);
                        if (p2->fVal == 1){
                            a->izq->fVal = -1;
                            a->der->fVal = 0;
                        }
                        else if (p2->fVal == -1){
                            a->der->fVal = 1;
                            a->izq->fVal = 0;
                        }
                        else{
                            a->izq->fVal = 0; 
                            a->der->fVal = 0;
                        }
                        a->fVal = 0;
                    }
                    a->fVal = 0;
                    varioAltura = false;
                    break;
            }
            if (a->izq) updateCount(a->izq);
            if (a->der) updateCount(a->der);
            updateCount(a);
            varioAltura = false;
        }
        updateCount(a);
        return insertado;
    } else { varioAltura = false; return false; }
}

NodoAVL* ADDPointTree (NodoAVL *&a, string nombre, int puntos, int id, bool& varioAltura){
    if (a == NULL)
    {
        a = new NodoAVL(nombre, puntos, id);
        varioAltura = true;
        return a;
    }
    if (puntos < a->puntaje || (puntos == a->puntaje && id < a->id))
    {
        NodoAVL* ins = ADDPointTree(a->izq, nombre, puntos, id, varioAltura);
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
                        rotacionDerecha(a);
                        a->der->fVal = 0;
                    } else {
                        NodoAVL *p2 = p1->der;
                        rotacionIzquierda(a->izq);
                        rotacionDerecha(a);
                        if (p2->fVal == -1){
                            a->der->fVal = 1;
                            a->izq->fVal = 0;
                        }
                        else if (p2->fVal == 1){
                            a->izq->fVal = -1;
                            a->der->fVal = 0;
                        }
                        else{
                            a->izq->fVal = 0;
                            a->der->fVal = 0; }
                            a->fVal = 0;
                    }
                    a->fVal=0;
                    varioAltura = false;
                    break;
            }
            varioAltura = false;
        }
        if (a->izq) updateCount(a->izq);
        if (a->der) updateCount(a->der);
        updateCount(a);
        return ins;
    }
    else if (puntos > a->puntaje || (puntos == a->puntaje && id > a->id))
    {
        NodoAVL* ins = ADDPointTree(a->der, nombre, puntos, id, varioAltura);
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
                        rotacionIzquierda(a);
                        a->izq->fVal = 0;
                        a->fVal=0;
                    } else {
                        NodoAVL *p2 = p1->izq;
                        rotacionDerecha(a->der);
                        rotacionIzquierda(a);
                        if (p2->fVal == -1){
                            a->der->fVal = 1;
                            a->izq->fVal = 0;
                        }
                        else if (p2->fVal == 1){
                            a->izq->fVal = -1;
                            a->der->fVal = 0;
                        }
                        else{
                            a->izq->fVal = 0;
                            a->der->fVal = 0; }
                            a->fVal = 0;
                    }
                    a->fVal = 0;
                    varioAltura = false;
                    break;
            }
            if (a->izq) updateCount(a->izq);
            if (a->der) updateCount(a->der);
            updateCount(a);
            varioAltura = false;
        }
        updateCount(a);
        return ins;
    } else { updateCount(a); return a; }
}

void ADD (Jugador *& j, string nombre, int puntos, int id){
    bool h1 = false;
    bool created = ADDId(j->idTree, nombre, puntos,id, h1);
    NodoAVL* ret= NULL;
    
    if (created){
        j->cantElem++;
        h1=false;
        ret = ADDPointTree(j->pointsTree,  nombre,  puntos, id, h1);
    }
    if (ret && (!j->top1 || ret->puntaje > j->top1->puntaje || (ret->puntaje == j->top1->puntaje && ret->id < j->top1->id))) {
        j->top1 = ret;
    }
        
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

int RANK(NodoAVL* a, int puntaje) {
    int res = 0;
    while (a) {
        if (a->puntaje >= puntaje) {
            res += 1;
            if (a->der) res += a->der->countSub;
            a = a->izq;
        } else {
            a = a->der;
        }
    }
    return res;
}

NodoAVL* TOP1(Jugador* j){
    return j->top1;
}

int CANT (Jugador* j){
    return j->cantElem;
}

// void inorder(NodoAVL* j)
// {
//     if(j)
//     {

        
//         inorder(j->izq);
//         cout << "puntaje: " <<j->puntaje << "id: " << j->id << endl;
//         inorder(j->der);
//     }
// }

//capas y es mejor hacerlo con else if 
int main()
{
    Jugador* j = new Jugador();
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        string op;
        cin >> op;
        int opAccion = -1;
        if (op == "ADD") opAccion = 0;
        else if (op == "FIND") opAccion = 1;
        else if (op == "TOP1") opAccion = 2;
        else if (op == "COUNT") opAccion = 3;
        else if (op == "RANK") opAccion = 4;

        switch (opAccion) {
            case 0: {
                int id, puntos;
                string nombre;
                cin >> id >> nombre >> puntos;
                ADD(j, nombre, puntos, id);
                break;
            }
            case 1: {
                int id;
                cin >> id;
                FIND(j->idTree, id);
                break;
            }
            case 2: {
                NodoAVL* top = TOP1(j);
                if (top) cout << top->nombre << " " << top->puntaje << endl;
                break;
            }
            case 3: {
                cout << CANT(j) << endl;
                break;
            }
            case 4: {
                int puntaje;
                cin >> puntaje;
                cout << RANK(j->pointsTree, puntaje) << endl;
                break;
            }
            default:
                // No hace nada
                break;
        }
    }
    return 0;
    
}
