#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

int main () {
	while (1) {
		int size = 0;
		read(STDIN_FILENO, &size, sizeof(size));
		char *buf = (char *) malloc(size);
		read(STDIN_FILENO, buf, size);

		char *string = (char *)calloc(1, sizeof(char));
		int sizeNew = size;
		size = 0;

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

		write(STDOUT_FILENO, &size, sizeof(size));
		write(STDOUT_FILENO, string, size);
		free(buf);
		free(string);
	}
}