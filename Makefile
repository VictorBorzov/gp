CFLAGS=-Wall -Wold-style-definition -ggdb -std=c11 -pedantic
LIBS=`sdl2-config --cflags --libs`

gp: main.c style.h
	$(CC) $(CFLAGS) -o gp main.c $(LIBS)
