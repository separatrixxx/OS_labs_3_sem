#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <ctype.h>


int main () {

	int fd1[2]; // P -> C1
	int fd2[2]; // C1 -> C2
	int fd3[2]; // C2 -> P

	if (pipe(fd1) == -1) {
		perror("pipe1 error");
		return -1;
	}
	if (pipe(fd2) == -1) {
		perror("pipe2 error");
		return -1;
	}
	if (pipe(fd3) == -1) {
		perror("pipe3 error");
		return -1;
	}
	
	int id1 = fork();
	if (id1 < 0) {
		perror("fork1 error");
		return -1;
	}

	if (id1 == 0) {
		int id2 = fork();
		if (id2 < 0) {
			perror("fork2 error");
			return -1;
		}

		if (id2 == 0) {
			close(fd2[1]);
			close(fd3[0]);

			close(fd1[0]);
			close(fd1[1]);

			dup2 (fd2[0], STDIN_FILENO);
			dup2 (fd3[1], STDOUT_FILENO);

			execl("./child2", "./child2", NULL);
			perror ("execl");

			close(fd2[0]);
			close(fd3[1]);
		} else {
			close(fd1[1]);
			close(fd2[0]);

			close(fd3[0]);
			close(fd3[1]);

			dup2 (fd1[0], STDIN_FILENO);
			dup2 (fd2[1], STDOUT_FILENO);

			execl("./child1", "./child1", NULL);
			perror ("execl");

			close(fd1[0]);
			close(fd2[1]);
		}
	
	} else {
		printf("It's parent process\n");

		close(fd2[0]);
		close(fd2[1]);
		close(fd1[0]);
		close(fd3[1]);

		while (1) {
				char *string = (char *)calloc(1, sizeof(char));
				int size = 0;
				char c;
				while ((c = getchar()) != '\n') {
					if (c == EOF) {
						return 0;
					}
					string[size] = c;
					string = (char *)realloc(string, (++size + 1) * sizeof(char));
				}
				string[size++] = '\0';
				write(fd1[1], &size, sizeof(size));
				write(fd1[1], string, size);
				free(string);
				read(fd3[0], &size, sizeof(size));
				char *buf = (char *) malloc(size);
				read(fd3[0], buf, size);
				printf("%s\n", buf);
				free(buf);
			}

			close(fd1[1]);
			close(fd3[0]);
	}
}
