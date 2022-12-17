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
		for (int i = 0; i < size; i++) {
			buf[i] = tolower(buf[i]);
		}

		write(STDOUT_FILENO, &size, sizeof(size));
		write(STDOUT_FILENO, buf, size);
		free(buf);
	}
}
