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
