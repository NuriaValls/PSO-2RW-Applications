CFLAGS = -Wall -Wextra -lm -O5 -Wno-unused-variable -Wno-unused-parameter -I/usr/local/include -L/usr/local/lib -lfftw3
MAIN = data_main_aux.c
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
