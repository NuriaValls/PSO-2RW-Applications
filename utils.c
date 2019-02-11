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
