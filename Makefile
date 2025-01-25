CFLAGS=-Wall -Wold-style-definition -ggdb -std=c11 -pedantic
LIBS=`sdl2-config --cflags --libs` -lm -lSDL2_gfx

gp: main.c style.h
	$(CC) $(CFLAGS) -o gp main.c $(LIBS)
