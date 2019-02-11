#ifndef _UTILS_H_
#define _UTILS_H_

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define DEBUG				1

#define COLOR_DEBUG			"\x1b[33m"
#define COLOR_RESET			"\x1b[0m"

#define ANSI_COLOR_RED		"\x1b[31m"
#define ANSI_COLOR_GREEN	"\x1b[32m"
#define ANSI_COLOR_CYAN		"\x1b[36m"
#define ANSI_COLOR_RESET	"\x1b[0m"

#define MSG_CONFIG_OK		ANSI_COLOR_CYAN "[Config OK] " ANSI_COLOR_RESET
#define MSG_CONFIG_KO		ANSI_COLOR_RED "[Config KO] " ANSI_COLOR_RESET
#define MSG_WRONG_ARGS		MSG_CONFIG_KO "The calling format is not correct, it should be:\n\tpsoAlgo\t<population_size>\n"

void print(const char *msg);
void println(const char *msg);
void debug(const char *msg);

#endif
