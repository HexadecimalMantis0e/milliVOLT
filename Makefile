CC=gcc
CFLAGS=-std=c17 -pedantic -Wall
EXE=milliVOLT

all: $(EXE)

$(EXE): milliVOLT.c
	$(CC) $(CFLAGS) -o $@ $^ -static

clean:
	rm -f $(EXE)
