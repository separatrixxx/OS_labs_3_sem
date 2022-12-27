#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include "lib.h"

typedef int (*gcfFunction)(int A, int B);
typedef int (*primeFunction)(int A, int B);

const char* LIBS[] = {"./liblib1d.so", "./liblib2d.so"};

void handleOpen(void** lib_handle, const int cur_lib) {
    *lib_handle = dlopen(LIBS[cur_lib], RTLD_NOW);
    if (*lib_handle == NULL) {
        printf("%s\n", dlerror());
        exit(1);
    }
}

void implSet(gcfFunction *gcf, primeFunction *primeCount, void* lib_handle) {
    *gcf = dlsym(lib_handle, "gcf");
    if (*gcf == NULL) {
        printf("%s\n", dlerror());
        exit(1);
    }
    *primeCount = dlsym(lib_handle, "PrimeCount");
    if (*primeCount == NULL) {
        printf("%s\n", dlerror());
        exit(1);
    }
}

int main() {
    int arg, cur_lib = 0;
    void* lib_handle;
    gcfFunction gcf;
    primeFunction primeCount;
    handleOpen(&lib_handle, cur_lib);
    implSet(&gcf, &primeCount, lib_handle);
    while (scanf("%d", &arg) != EOF) {
        if (arg == 0) {
            dlclose(lib_handle);
            cur_lib = (cur_lib == 0)? 1 : 0;
            handleOpen(&lib_handle, cur_lib);
            implSet(&gcf, &primeCount, lib_handle);
        } else if (arg == 1) {
            int a, b;
            scanf("%d %d", &a, &b);
            printf("%d\n", gcf(a, b));
        } else if (arg == 2) {
            int a, b;
            scanf("%d %d", &a, &b);
            printf("%d\n", primeCount(a, b));
        } else {
            return -1;
        }
    }
    return 0;
}
