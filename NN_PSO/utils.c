#include "utils.h"

void print(const char *msg) {
	write(STDOUT_FILENO, msg, strlen(msg));
}

void println(const char *msg) {
	write(STDOUT_FILENO, msg, strlen(msg));
	write(STDOUT_FILENO, "\n", 1);
}

void debug(const char *msg) {
	if (DEBUG) {
		write(STDERR_FILENO, COLOR_DEBUG, strlen(COLOR_DEBUG));
		write(STDERR_FILENO, msg, strlen(msg));
		write(STDERR_FILENO, COLOR_RESET, strlen(COLOR_RESET));
	}
}

char *readFileDescriptor(int fd) {
	char mychar = '\0';
	int index = 0;
	char *string;
	int length;

	string = NULL;
	while (1) {
		length = (int) read(fd, &mychar, sizeof(char));

		if (mychar == '\n' || mychar == '\0' || mychar == ',' || !length)
			return string;

		string = realloc(string, (size_t) index + 2);
		string[index] = mychar;
		string[index + 1] = '\0';
		index++;
	}
}
