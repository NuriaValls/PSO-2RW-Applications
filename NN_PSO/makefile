CFLAGS = -Wall -Wextra -lm -O5 -Wno-unused-variable -Wno-unused-parameter
MAIN = main.c
MODULES = utils logica
EXE = psoAlgo

FILES = $(MAIN) $(patsubst %, %.c, $(MODULES))
HEADERS = $(patsubst %, %.h, $(MODULES))

all: $(FILES) $(HEADERS)
	gcc $(FILES) $(CFLAGS) -o $(EXE)

clean:
	rm -f $(EXE)

tar:
	tar -cf PSOAlgorithm.tar $(FILES) $(HEADERS) makefile
