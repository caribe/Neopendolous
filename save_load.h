#include <string.h>
#include <stdio.h>
#include "share.h"

typedef struct files_t {
	char name[32];
} files_t;

extern int colors[COLORS];

files_t *files;
int file_index;

char *file_list(int, int*);
void add_file(const char*, int, int);
int load_game(char*);

char filename[16];
char ext[6];
