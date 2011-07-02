FILES = main.c graph.c game.c editor.c save_load.c report.c mapgen.c
OBJ = main.o graph.o game.o editor.o save_load.o report.o mapgen.o
CFLAGS = -W -Wall

main : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) `allegro-config --libs` -o main

debug : $(FILES)
	$(CC) -g -W -Wall $(FILES) `allegro-config --libs` -o debug

release : $(FILES)
	$(CC) -s -W -Wall $(FILES) `allegro-config --libs` -o neopend

main.o : main.c
graph.o : graph.c
game.o : game.c
editor.o : editor.c
save_load.o : save_load.c
report.o : report.c
mapgen.o : mapgen.c

clean :
	rm *.o
