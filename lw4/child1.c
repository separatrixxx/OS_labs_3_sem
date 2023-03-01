#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>

int main() {
    sem_t *semaphore = sem_open("_sem", O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH, 0);
    int value = 0;
    int flag = 1;

    while (flag) {
        if (sem_getvalue(semaphore, &value) != 0) {
            perror("sem_getvalue error");
        }

        if (value == 2) {
            int fd = shm_open("_back", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
            struct stat statBuf;
            fstat(fd, &statBuf);
            int size = statBuf.st_size;
            char *mapped = (char *) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

            if (mapped[0] == '\0') {
                if (sem_wait(semaphore) != 0) {
                    perror("sem_wait error");
                }
                
                usleep(00150000);
                munmap(mapped, size);
                close(fd);                    
                flag = 0;
            }

            char *buf = (char *) malloc(size);
            sprintf(buf, "%s", mapped);

            for (int i = 0; i < size; i++) {
			    buf[i] = tolower(buf[i]);
		    }

            memset(mapped, '\0', size);
            sprintf(mapped, "%s", buf);
            munmap(mapped, size);
            close(fd);

            if (sem_wait(semaphore) != 0) {
                perror("sem_wait error");
            }

            usleep(00150000);
        }
    }

    if (sem_close(semaphore) != 0) {
        perror("sem_close error");
    }
    
    return 0;
}