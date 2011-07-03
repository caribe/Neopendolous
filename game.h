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
