#include <string>
#include <iostream>
#include "tads/AVL.cpp"


using namespace std;



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
                TOP1(j);
                break;
            }
            case 3: {
                CANT(j);
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
