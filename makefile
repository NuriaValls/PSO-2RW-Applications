CFLAGS = -Wall -Wextra -lm
MAIN = main.c
MODULES = logica utils
EXE = psoAlgo

FILES = $(MAIN) $(patsubst %, %.c, $(MODULES))
HEADERS = $(patsubst %, %.h, $(MODULES))

all: $(FILES) $(HEADERS)
		gcc $(FILES) $(CFLAGS) -o $(EXE)

clean:
		rm -f $(EXE)

tar:
		tar -cf PSOAlgorithm.tar $(FILES) $(HEADERS) makefile
