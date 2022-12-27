#include "lib.h"

// Drugoy algoritm
int gcf(int A, int B) {
    int x;
    if (A > B) {
        x = B;
    } else {
        x = A;
    }

    while (A % x != 0 || B % x != 0)
        x--;
    return x;
}

// Resheto Eratosfena
int PrimeCount(int A, int B) {
    int count = 0;
    int N = 20000;
    unsigned int a[N];

    for(int i = 0; i < N; i++){
        a[i] = i;
    }
      
    a[1]=0;
    for(int s=2; s<N; s++){
        if(a[s] != 0){
            for(int j = s*2; j < N; j += s){
                a[j] = 0;
            }
        }
    }
    for(int i = 0; i < N; i++){
        if (a[i] != 0) {
            count++;
       }
    }
    
    return count;
}
