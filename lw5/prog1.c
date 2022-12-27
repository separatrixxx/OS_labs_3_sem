#include <stdio.h>
#include "lib.h"

int main() {
    int arg;
    while(scanf("%d ", &arg) != EOF) {
        if (arg == 1) {
            int a, b;
            scanf("%d %d", &a, &b);
            printf("%d\n", gcd(a, b));
        } else if (arg == 2) {
            int k;
            scanf("%d", &k);
            printf("%f\n", Pi(k));
        } else {
            return -1;
        }
    }
    return 0;
}
