#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

const int MAXN = 1000;
int cris[MAXN];

int maxPoints(int leftIndex, int rightIndex, int k /*amount of elements in subarray*/, int n /*total elements*/) {
    // ya recorri todo
    if(l>r) return 0;
    int val = getValue(l,r,k);
    
    //recorro el array 1 menos de la der y hago qeue el valor sea k^2 + el siguiente de puntos
    int res = maxPoints(l,r-1,0,n)+((k+1)*(k+1));
    //  cuando salgo del recursivo estoy en lo mas abajo
    //calculo valor por elementos del mismo color
    for(int i=0;i<r;i++){
        if(cris[i]==cris[r]){
            int cand = maxPoints(l,i,k+1,n)+ maxPoints(i+1,r-1,0,n);
            if(cand>res) res=cand;
        }
    }
    // el nuevo mejor valor para esa posicion
    setValue(l,r,k,res);
    return res;
}


int main() {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> cris[i];
    }
    
    // tad base case fill por ejemplo en array es array = -1
    setUp();

    int ans = maxPoints(0, n - 1, 0, n);// recorro del mas grande 
    cout << ans << endl;
    
    cleanup();
    return 0;
}