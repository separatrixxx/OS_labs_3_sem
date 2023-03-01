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
    sem_unlink("_sem");
    sem_t *semaphore = sem_open("_sem", O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH, 3);
    int value = 3;

    int id1 = fork();

    if (id1 == 0) {
        if (sem_close(semaphore) != 0) {
            perror("sem_close error");
        }
        execlp("./child1", "./child1", NULL);

    } else {
        int id2 = fork();

        if (id2 == 0) {
            if (sem_close(semaphore) != 0) {
                perror("sem_close error");
            }
            execlp("./child2", "./child2", NULL);
        }

        while (1) {
            if (sem_getvalue(semaphore, &value) != 0) {
                perror("sem_getvalue error");
            }
            

            if (value == 3) {
                printf("It's Parent process!\n\n");

                char *string = (char *)calloc(1, sizeof(char));
                int size = 0;
                char c;

                while ((c = getchar()) != '\n') {
                    if (c == EOF) {   
                        int fd = shm_open("_back", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
                        ftruncate(fd, 1);
                        char *mapped = (char *) mmap(NULL, 1, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
                        
                        memset(mapped, '\0', 1);
                        sprintf(mapped, "%c", '\0');

                        if (sem_wait(semaphore) != 0) {
                            perror("sem_wait error");
                        }
                        if (sem_close(semaphore) != 0) {
                            perror("sem_close error");
                        }

                        return 0;
                    }

                    string[size] = c;
                    string = (char *)realloc(string, (++size + 1) * sizeof(char));
                }

                string[size++] = '\0';

                int fd = shm_open("_back", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
                ftruncate(fd, size);
                char *mapped = (char *) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

                memset(mapped, '\0', size);
                sprintf(mapped, "%s", string);
                close(fd);

                if (sem_wait(semaphore) != 0) {
                    perror("sem_wait error");
                }
            }

            if (value == 0) {
                int fd = shm_open("_back", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
                struct stat statBuf;

                fstat(fd, &statBuf);

                int size = statBuf.st_size;
                char *mapped = (char *) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

                printf("%s\n\n", mapped);

                close(fd);
                munmap(mapped, size);

                if (sem_post(semaphore) != 0) {
                    perror("sem_post error");
                }
                if (sem_post(semaphore) != 0) {
                    perror("sem_post error");
                }
                if (sem_post(semaphore) != 0) {
                    perror("sem_post error");
                }
            }
        }
    }

    sem_close(semaphore);

    return 0;
}