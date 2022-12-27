#include <stdio.h>
#include "lib.h"

int main() {
    int arg;
    while(scanf("%d ", &arg) != EOF) {
        if (arg == 1) {
            int a, b;
            scanf("%d", &a);
            scanf("%d", &b);
            printf("%d\n", gcf(a, b));
        } else if (arg == 2) {
            int a, b;
            scanf("%d", &a);
            scanf("%d", &b);
            printf("%d\n", PrimeCount(a, b));
        } else {
            return -1;
        }
    }
    return 0;
}
