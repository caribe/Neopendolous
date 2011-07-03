/*
 * 
 * Neopedulous - A clone of Pendulous
 * Copyright (C) 2005 Vincenzo Buttazzo <vbuttazzo@yahoo.com>
 * 
 * This file is part of Neopendulous.
 * 
 * Neopendulous is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Neopendulous is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include <allegro.h>
#include "data.h"

#define MAGIC 0xFAD0

#define COLORS 10

#define MATRIX_WATER 0x00
#define MATRIX_VOID 0x08
#define MATRIX_DEATH_VOID 0x10
#define MATRIX_ARMY 0x18
#define MATRIX_DEATH_ARMY 0x20
#define MATRIX_CITY 0x28

#define MATRIX_PLAYER(i) (matrix[i].info&0x07)
#define MATRIX_TYPE(i) (matrix[i].info&0x38)
#define MATRIX_HIDE(i) (matrix[i].info&0x40)
#define MATRIX_CROSS(i) (matrix[i].info&0x80)
#define MATRIX_CHANGE(i) (matrix[i].info&0xC7)
#define MATRIX_FOG(i, p) ((matrix[i].fog & (1<<p)))

#define SL_GAME 0
#define SL_SCENARIO 1

#define GAME_ABORT -1
#define GAME_END 8

/* Types:
0: water
1: void
2: death void
3: army
4; death army
5: city
*/
typedef struct tile_t {
	unsigned char info; // 0-2 owner 3-5 type 6 Hide 7 Cross Attack
	unsigned char fog; // 1 bit per player
} tile_t;
tile_t matrix[2500]; // Tile infos

typedef struct {
	char description[256], name[16];
	unsigned short tiles, row_x, row_y;
	unsigned char points_tile, points_city, fog, hide, cross;
	short turn, turns;
	unsigned char player, pane_x, pane_y;
	unsigned short powerup_i, powerup_v, random;
} game_t;
game_t game;

typedef struct players_t {
	char name[16];
	unsigned short reserves, resources, reser_i, resou_i, reser_max, cities;
	unsigned char reser_t, resou_t, reser_c, resou_c, effect;
	unsigned int points, points_i;
	unsigned int attacks, defenses, ok_attacks, ok_defenses, mean_attacks, mean_defenses;
} players_t;
players_t players[8];

// Colors
enum {red, lightgray, green, cyan, yellow, blue, gray, white, black, darkgray};

DATAFILE *datafile;

/*
 * Boost array that contains the offset to surrounding tiles
 */
int offset[8];

/*
 * Binary values of surrounding tiles following this scheme
 *        3  2  6
 *        1     4
 *        9  8 12
 */
static const unsigned char nears[8] = {3, 2, 6, 4, 12, 8, 9, 1};

// game.c
int start_game();
unsigned char compute_m(int);

// save_load.c
int save_scenario(int);
int load_scenario(int);

// graph.c
void graph_init();
void rect3d(BITMAP*, int, int, int, int, int);
void text3d(BITMAP*, int, int, char *);
int start_minimap();
void draw_proc(DIALOG*);
int game_proc(int, DIALOG*, int);
int d_text3d_proc(int, DIALOG*, int);
int d_frame_proc(int, DIALOG*, int);
int d_masked_proc(int, DIALOG*, int);

// editor.c
void start_editor();

// report.c
void report();

// mapgen.c
void start_mapgen(int);
	
// extra
void foo();
