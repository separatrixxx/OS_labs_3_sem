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

int main()
{
    sem_t *semaphore = sem_open("_sem", O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH, 0);

    int value = 0;
    int flag = 1;

    while (flag) {
        if (sem_getvalue(semaphore, &value) != 0) {
            perror("sem_getvalue error");
        }

        if (value == 1) {
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
            char *string = (char *)calloc(1, sizeof(char));
            int sizeNew = size;
            size = 0;
            int cnt = 0;
            string[size] = buf[0];
            string = (char *)realloc(string, (++size + 1) * sizeof(char));

            for (int i = 1; i < sizeNew; i++) {
                if (buf[i] == ' ') {
                    buf[i] = '_';
                    string[size] = buf[i];
                    string = (char *)realloc(string, (++size + 1) * sizeof(char));  
                } else {
                    string[size] = buf[i];
                    string = (char *)realloc(string, (++size + 1) * sizeof(char));
                }  
            }

            string[size++] = '\0';
            ftruncate(fd, size);
            memset(mapped, '\0', size);
            sprintf(mapped, "%s", string);
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