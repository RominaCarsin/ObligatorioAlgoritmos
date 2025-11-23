#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

class Archivo {
    public:
        int tamanio, lineas, puntos;
        Archivo(int t, int l, int p): tamanio(t), lineas(l), puntos(p){}
};

int main(){
    int N, S, L;
    cin >> N >> S >> L;
    
    Archivo** archivos = new Archivo*[N];
    for (int i = 0; i < N; i++)
    {
        int t, l, p;
        cin >> t >> l >> p;
        archivos[i] = new Archivo(t, l, p);
    }
    
    // revisar si no genera errores de memoria como el ejercicio 8 que una estructura 3D era muy grande
    int*** mat = new int**[N + 1];
    for (int i = 0; i <= N; i++)
    {
        mat[i] = new int*[S + 1];
        for (int s = 0; s <= S; s++)
        {
            mat[i][s] = new int[L + 1];
            for (int l = 0; l <= L; l++)
            {
                mat[i][s][l] = 0;
            }
        }
    }

    for (int i = 1; i <= N; i++)
    {
        Archivo* arch = archivos[i-1];
        
        for (int s = 0; s <= S; s++)
        {
            for (int l = 0; l <= L; l++)
            {
                // Opción 1: NO tomar el archivo i
                mat[i][s][l] = mat[i-1][s][l];
                
                // Opción 2: TOMAR el archivo i (si cabe en ambas restricciones)
                if (arch->tamanio <= s && arch->lineas <= l)
                {
                    mat[i][s][l] = max(mat[i][s][l], 
                                     arch->puntos + mat[i-1][s - arch->tamanio][l - arch->lineas]);
                }
            }
        }
    }

    cout << mat[N][S][L] << endl;
    
    // Liberar memoria, precaucion innecesaria pero quede traumado del 8
    for (int i = 0; i <= N; i++)
    {
        for (int s = 0; s <= S; s++)
        {
            delete[] mat[i][s];
        }
        delete[] mat[i];
    }
    delete[] mat;
    
    for (int i = 0; i < N; i++)
    {
        delete archivos[i];
    }
    delete[] archivos;
    
    return 0;
}