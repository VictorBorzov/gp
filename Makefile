CFLAGS=-Wall -ggdb -std=c11 -pedantic
LIBS=`sdl2-config --cflags --libs`

gp: main.c
	$(CC) $(CFLAGS) -o gp main.c $(LIBS)
