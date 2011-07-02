#include <string.h>
#include <stdio.h>
#include "share.h"

struct {
	char name[16];
	char reser_t[5], resou_t[5], reser_c[5], resou_c[5], resources[5], reserves[5], points[5], reser_max[5], effect[5];
} play;

extern int colors[COLORS];

char x_sectors[5], y_sectors[5];
char turns[5], sector_points[5], base_points[5];

short player;

char *player_list(int, int*);
char *tiles_list(int, int*);
void save_frame();
int map_edit_proc(int, DIALOG*, int);
void start_mapedit();
