#include <stdio.h>
#include "share.h"

void ding();
int next_turn();
void save_game();
void compute_values(int);
int compute_def(int);
int compute_att(int);
int info_proc(int, DIALOG*, int);
int game_proc(int, DIALOG*, int);

extern int colors[COLORS];

/*
 * String buffers for printed informations
 */
char info_attack[6], info_defense[6], info_success[6];

/*
 * The last absolute position of the pointer in the game area
 */
int last_i = -1;

/*
 * Temporization flag. Declared in main.h
 */
extern volatile short timer;
