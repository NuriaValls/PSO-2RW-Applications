#include "utils.h"


/**
 * Prints a message on the terminal.
 *
 * @param msg   Message to print.
 */
void print(const char *msg) {
	write(STDOUT_FILENO, msg, strlen(msg));
}


/**
 * Prints a message on the terminal with a \n.
 *
 * @param msg   Message to print.
 */
void println(const char *msg) {
	write(STDOUT_FILENO, msg, strlen(msg));
	write(STDOUT_FILENO, "\n", 1);
}


/**
 * Prints a message on the terminal in yellow.
 *
 * @param msg   Message to print.
 */
void debug(const char *msg) {
	if (DEBUG) {
		write(STDERR_FILENO, COLOR_DEBUG, strlen(COLOR_DEBUG));
		write(STDERR_FILENO, msg, strlen(msg));
		write(STDERR_FILENO, COLOR_RESET, strlen(COLOR_RESET));
	}
}


/**
 * Reads chars from a file descriptor until it finds a ',', '\n' or '\0'.
 *
 * @param fd    File descriptor.
 * @return      Char sequence.
 */
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
