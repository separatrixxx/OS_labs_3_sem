#include "lib.h"
#include "math.h"

// Algoritm Evklida
int gcf(int A, int B) {
    if (B == 0)
        return A;
    return gcf(B, A % B);
}

// Easy algoritm
int PrimeCount(int A, int B) {
    int count = 0;
    for (int i = A; i <= B; i++) {
        int flag = 0;

        for (int j = 2; j < i; j++) {
            if (i % j == 0) {
                flag = 1;
                break;
            }
        }

        if (flag == 0) {
            count++;
        }
    }

    return count;
}