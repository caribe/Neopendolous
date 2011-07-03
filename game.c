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

/*
 * Description: Gamelogic routines and game GUI
 * Version; 1.0
 */

#include "game.h"

#define INFO_TURN 7
#define INFO_PLAYER 8
#define INFO_RESERVES 9
#define INFO_RESOURCES 10
#define INFO_ATTACK 11
#define INFO_DEFENSE 12
#define INFO_SUCCESS 13

// DIALOG structure for the game GUI
DIALOG game_dialog[] = {
	/* (dialog proc)	(x)  (y)	(w)  (h) (fg) (bg)(key)  (flags) (d1) (d2) (dp) (dp2) (dp3) */
   { d_box_proc,    480,   0, 160, 480,   0,   0,   0,      0,   0,   0, NULL, NULL, NULL  },
	{ game_proc,       0,   0, 480, 480,   0,   0,   0,      0,   0,   0, NULL, NULL, NULL  },
   { d_button_proc, 520,  10,  80,  18,   0,   0, 'n', D_EXIT,   0,   0, "&Next Turn", NULL, NULL  },
   { d_button_proc, 520,  34,  80,  18,   0,   0, 'w', D_EXIT,   0,   0, "&World Map", NULL, NULL  },
   { d_button_proc, 520,  58,  80,  18,   0,   0, 'r', D_EXIT,   0,   0, "&Report", NULL, NULL  },
   { d_button_proc, 520,  82,  80,  18,   0,   0, 's', D_EXIT,   0,   0, "&Save Game", NULL, NULL  },
   { d_button_proc, 520, 104,  80,  18,   0,   0, 'q', D_EXIT,   0,   0, "&Quit", NULL, NULL  },
	{ info_proc,     500, 150, 120,  20,   0,   0,   0,      0,   1,   0, "Turn", &game.turn, NULL  },
	{ info_proc,     500, 170, 120,  20,   0,   0,   0,      0,   0,   0, "Player", NULL, NULL  },
	{ info_proc,     500, 190, 120,  20,   0,   0,   0,      0,   2,   0, "Reserves", NULL, NULL  },
	{ info_proc,     500, 210, 120,  20,   0,   0,   0,      0,   2,   0, "Resources", NULL, NULL  },
	{ info_proc,     500, 230, 120,  20,   0,   0,   0,      0,   0,   0, "Attack", info_attack, NULL  },
	{ info_proc,     500, 250, 120,  20,   0,   0,   0,      0,   0,   0, "Defense", info_defense, NULL  },
	{ info_proc,     500, 270, 120,  20,   0,   0,   0,      0,   0,   0, "Success", info_success, NULL  },
	
   { NULL,            0,   0,   0,   0,   0,   0,   0,      0,   0,   0, NULL, NULL, NULL  }	
};

/*
 * int start_game()
 * Inits same enviroment variables, ask user to select a game and implements the main game loop
 * Return
 *		-1 on user exit
 * 	0-7 the winning player index
 *		8 on the end of turns
 */
int start_game() {
	int i;

	if (load_scenario(SL_GAME)) return GAME_ABORT;
		
	if (game.random!=0xffff) start_mapgen(1);
	game.random=0xffff;
	
	offset[0] = -1	-game.row_x;
	offset[1] =		-game.row_x;
	offset[2] = +1	-game.row_x;
	offset[3] = +1;
	offset[4] = +1	+game.row_x;
	offset[5] = 	+game.row_x;
	offset[6] = -1	+game.row_x;
	offset[7] = -1;

	for (i=1; game_dialog[i].proc; i++) {
		game_dialog[i].fg = colors[black];
		game_dialog[i].bg = colors[gray];
	}
	game_dialog[0].bg = colors[darkgray];
	
	for (i=INFO_TURN; game_dialog[i].proc; i++) game_dialog[i].bg = colors[darkgray];

	game_dialog[INFO_PLAYER].dp2 = players[game.player].name;
	game_dialog[INFO_RESERVES].dp2 = &players[game.player].reserves;
	game_dialog[INFO_RESOURCES].dp2 = &players[game.player].resources;
	
	if (game.turn==-1) next_turn();

	while(1) {
		switch(do_dialog(game_dialog, -1)) {
		case 2:
			i = next_turn();
			if (i==-1) {
				scare_mouse();
				clear_to_color(screen, colors[darkgray]);
				alert(players[game.player].name, "It's your turn!", NULL, "OK", NULL, 0, 0);
				unscare_mouse();
				break;
			}
			return i;
		case 3:
			start_minimap();
			break;
		case 4:
			report();
			break;
		case 5:
			save_scenario(SL_GAME);
			break;
		case 6:
			if (alert("Save the current game?", NULL, NULL, "&Yes", "&No", 'y', 'n')==1) save_scenario(SL_GAME);
			return GAME_ABORT;
		}
	}
	return 0;
}

/*
 * info_proc
 * A Allegro DIALOG procedure for printing out informations in the game on two rows. The first left aligned
 * row is a 3D label. The second right aligned is the information.
 * The DIALOG parameters are used this way:
 * - The d1 param defines the type of data
 * - The d2 param is the pointer to the data
 * Returns always D_O_K
 */
int info_proc(int msg, DIALOG *d, int c) {
	if (msg==MSG_DRAW) {
		rectfill(screen, d->x, d->y, d->x+d->w, d->y+d->h, d->bg);
		text3d(screen, d->x, d->y, d->dp);
		switch (d->d1) {
			case 0:
				textout_right(screen, font, d->dp2, d->x+d->w, d->y, colors[white]);
				break;
			case 1:
				textprintf_right(screen, font, d->x+d->w, d->y, colors[white], "%d", *(unsigned char *)d->dp2);
				break;
			case 2:
				textprintf_right(screen, font, d->x+d->w, d->y, colors[white], "%d", *(unsigned short *)d->dp2);
				break;
			case 3:
				textprintf_right(screen, font, d->x+d->w, d->y, colors[white], "%d", *(int *)d->dp2);
				break;
		}
	}
	return D_O_K;
}	

/*
 * game_proc
 * A Allegro DIALOG procedure for the normal game playing. See details.
 */
int game_proc(int msg, DIALOG *d, int c) {
	int i;
	
	switch(msg) {
	case MSG_START:
		timer = 0;
		break;
	
	case MSG_DRAW:
		draw_proc(d);
		break;

	case MSG_WANTFOCUS:
		return D_WANTFOCUS;

	// Every timeslice it controls if the mouse has been moved to another position
	case MSG_IDLE:
		if (timer==0) break; else {
			int x, y;

			timer=0;

			x = (mouse_x - d->x)/24;
			y = (mouse_y - d->y)/24;
			i = (y + game.pane_y) * game.row_x + x + game.pane_x;
	
			if (last_i!=i) {
				last_i=i;
	
				// If mouse is on the outer border the pane is moved
				if (y==0 && game.pane_y>0) game.pane_y--;
				else if (y==19 && game.pane_y<game.row_y-20) game.pane_y++;
				else if (x==0 && game.pane_x>0) game.pane_x--;
				else if (x==19 && game.pane_x<game.row_x-20) game.pane_x++;
				else compute_values(i);
	
				d->flags |= D_DIRTY;
			}
		}
		break;

	case MSG_CLICK:
		i = (((mouse_y - d->y)/24)+game.pane_y)*game.row_x+((mouse_x - d->x)/24)+game.pane_x;
		
		if (MATRIX_PLAYER(i)==game.player) {
			switch (MATRIX_TYPE(i)) {
			case MATRIX_VOID:
				if (players[game.player].resources==0 || players[game.player].reserves==0) {
					ding(DATA_DING);
					break;
				}
				matrix[i].info = MATRIX_CHANGE(i) | MATRIX_ARMY;
				if (game.turn>0) players[game.player].resources--;
				players[game.player].reserves--;
				ding(DATA_MARCH);
				break;
			case MATRIX_ARMY:
				if (players[game.player].resources==0) {
					ding(DATA_DING);
					break;
				}
				matrix[i].info = MATRIX_CHANGE(i) | MATRIX_VOID;
				if (game.turn>0) players[game.player].resources--;
				players[game.player].reserves++;
				ding(DATA_MARCH);
				break;
			default:
				ding(DATA_DING);
			}
		} else if (MATRIX_TYPE(i)!=MATRIX_WATER) {
			int att, def, perc;

			// Cannot perform any action
			if (
				game.turn<1
				|| players[game.player].resources==0
				|| players[game.player].reserves==0
				|| (att = compute_att(i))==0
			) {
				ding(DATA_DING);
				break;
			}

			if (game.powerup_i==i) att+=game.powerup_v;


			players[game.player].resources--;
			players[game.player].reserves--;

			// Statistics update
			players[game.player].attacks++;
			players[MATRIX_PLAYER(i)].defenses++;
			def = compute_def(i);
			perc = ((100*att)/(att+def));
			players[game.player].mean_attacks += perc;
			players[MATRIX_PLAYER(i)].mean_defenses += (100-perc);

			if ((rand()%100) < perc) {
				int k, m, n, p;

				// Statistics update
				players[game.player].ok_attacks++;

				// Player gets the tile with "not hide", "not fog" and "not cross" setted
				switch (MATRIX_TYPE(i)) {
				case MATRIX_VOID:
				case MATRIX_ARMY:
				case MATRIX_DEATH_VOID:
				case MATRIX_DEATH_ARMY:
					matrix[i].info = 0x40 | MATRIX_ARMY | game.player;
					break;
				case MATRIX_CITY:
					matrix[i].info = 0x40 | MATRIX_CITY | game.player;
					break;
				}
				p = 1<<game.player;
				matrix[i].fog |= p;

				// Surrounding hide/fog flags are updated
				m = compute_m(i);
				for (k=0; k<8; k++) {
					n = i+offset[k];
					if ((nears[k]&m)!=nears[k]) continue;
					matrix[n].info = (matrix[n].info & 0x7f) | 0x40;
					matrix[n].fog |= p;
				}

				game.powerup_i=(unsigned short)-1;
				game.powerup_v=0;
			} else {
				// Statistics update
				players[MATRIX_PLAYER(i)].ok_defenses++;

				game.powerup_i=i;
				game.powerup_v+=4;
			}
			ding(DATA_BATTLE);
		} else {
			ding(DATA_DING);
		}

		compute_values(i);
		game_dialog[INFO_RESERVES].flags |= D_DIRTY;
		game_dialog[INFO_RESOURCES].flags |= D_DIRTY;
		d->flags |= D_DIRTY;
		return D_O_K;
	}
	return D_O_K;
}

/*
 * int next_turn()
 * At the and of a turn a lot of operation have to be done. See details.
 * Returns
 * 	-1 on normal turn change
 * 	0-7 the player index on game end by player win
 * 	8 on game end by end of turns
 */
int next_turn() {
	int i, j, n, k, p, list[game.tiles], info[game.tiles];
	int res = 0;
	int cities[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char m;

	// Destroys player's death armies. Set death all void and army tiles
	for (i=j=0; i<game.tiles; i++) {
		list[i] = -1;
		info[i] = 0;
		switch (MATRIX_TYPE(i)) {
		case MATRIX_VOID:
			matrix[i].info = MATRIX_PLAYER(i) | MATRIX_DEATH_VOID;
			break;
		case MATRIX_DEATH_ARMY:
			if (MATRIX_PLAYER(i)==game.player) matrix[i].info = game.player | MATRIX_DEATH_VOID;
			break;
		case MATRIX_ARMY:
			matrix[i].info = MATRIX_PLAYER(i) | MATRIX_DEATH_ARMY;
			break;
		case MATRIX_CITY:
			info[i] = 1;
			list[j++] = i;
			cities[MATRIX_PLAYER(i)]++;
		}
	}

	// Set alive all connected void and army tiles. Identifies border tiles (info = 2)
	for (i=0; list[i]!=-1 && i<game.tiles; i++) {
		m = compute_m(list[i]);
		p = MATRIX_PLAYER(list[i]);
		for (k=0; k<8; k++) {
			n = list[i]+offset[k];
			if ((nears[k]&m)!=nears[k]) continue;
			if (MATRIX_PLAYER(n)==p) {
				if (info[n]>0 || MATRIX_TYPE(n)==MATRIX_WATER) continue;
				info[n] = 1;
				list[j++] = n;

				switch (MATRIX_TYPE(n)) {
				case MATRIX_DEATH_VOID:
					matrix[n].info = p | MATRIX_VOID;
					break;
				case MATRIX_DEATH_ARMY:
					matrix[n].info = p | MATRIX_ARMY;
					break;
				}
			} else {
				// A border tile is eventually found
				if (MATRIX_TYPE(n)!=MATRIX_WATER) info[list[i]] = 2;
			}
		}
	}

	// Turn=-1 is a setup turn for the first player view
	if (game.turn==-1) {
		game.turn=0;
	} else {
		p = game.player;

		// Searches next player
		do {
			game.player = (game.player+1) & 0x07;
			if (game.player==0) game.turn++;
			if (cities[game.player]>0) break;
		} while (game.player!=p);

		if (game.player==p) return p;
		if (game.turn==game.turns) return GAME_END;
	
		game.powerup_i=(unsigned short)-1;
		game.powerup_v=0;
	}

	// Transforms death border	tiles into player's void tiles 
	for (i=0; i<game.tiles; i++) if (info[i]==2) {
		if (MATRIX_PLAYER(i)!=game.player) continue;
		m = compute_m(i);
		for (k=0; k<8; k++) {
			if ((nears[k]&m)!=nears[k]) continue;
			n = i+offset[k];
			if (MATRIX_TYPE(n)==MATRIX_DEATH_VOID || MATRIX_TYPE(n)==MATRIX_DEATH_ARMY) matrix[n].info = MATRIX_VOID | game.player;
		}
	}

	// Set death player's void and army tiles	and set "not fog/not hide" for player's cities
	for (i=j=0; i<game.tiles; i++) {
		list[i] = -1;
		info[i] = 0;
		if (MATRIX_PLAYER(i)!=game.player) continue;

		switch (MATRIX_TYPE(i)) {
		case MATRIX_VOID:
			matrix[i].info = MATRIX_DEATH_VOID | game.player;
			break;
		case MATRIX_ARMY:
			matrix[i].info = MATRIX_DEATH_ARMY | game.player;
			break;
		case MATRIX_CITY:
			list[j++] = i;
			info[i] = 1;
			matrix[i].info |= 0x40;
			matrix[i].fog |= (1<<game.player);
			break;
		}
	}

	// Set alive player's connected void and army tiles.and set them to "not fog/not hide".
	// Finds player's port cities. Counts player's armies on game area
	for (i=0; list[i]!=-1 && i<game.tiles; i++) {
		m = compute_m(list[i]);
		for (k=0; k<8; k++) {
			n = list[i]+offset[k];
			if ((nears[k]&m)!=nears[k] || info[n]>0) continue;

			matrix[n].info |= 0x40;
			matrix[n].fog |= (1<<game.player);
			info[n] = 1;

			if (MATRIX_TYPE(n)==MATRIX_WATER) {
				// if the tile is a water tile and there is a city then we found a port city
				if (MATRIX_TYPE(list[i])==MATRIX_CITY) info[list[i]] = 2;
				continue;
			}
			if (MATRIX_PLAYER(n)!=game.player) continue;

			list[j++] = n;

			switch (MATRIX_TYPE(n)) {
			case MATRIX_DEATH_VOID:
				matrix[n].info = MATRIX_CHANGE(n) | MATRIX_VOID;
				break;
			case MATRIX_DEATH_ARMY:
				matrix[n].info = MATRIX_CHANGE(n) | MATRIX_ARMY;
				res++;
				break;
			}
		}
	}

	// If necessary we find the tiles that could be attacked by barrier crossing
	if (game.cross) {
		for (i=j=0; i<game.tiles; i++) {
			list[i] = -1;

			// Rearrange the list by positioning port towns on head
			if (info[i]==2) {
				list[j++] = i;
				info[i]=1;
			} else {
				info[i]=0;
				continue;
			}
		}
		
		for (i=0; list[i]!=-1 && i<game.tiles; i++) {
			m = compute_m(list[i]);
			for (k=0; k<8; k++) {
				n = list[i]+offset[k];
				if ((nears[k]&m)!=nears[k] || info[n]>0) continue;
				info[n] = 1;

				if (MATRIX_TYPE(n)==MATRIX_WATER) {
					matrix[n].info |= 0x40;
					matrix[n].fog |= (1<<game.player);
					list[j++] = n;
				} else {
					// Found a cross barrier attackable tile
					if (MATRIX_PLAYER(n)!=game.player) matrix[n].info |= 0x80;
				}
			}
		}
	}

	// Update GUI data
	game_dialog[INFO_PLAYER].dp2 = players[game.player].name;
	game_dialog[INFO_RESERVES].dp2 = &players[game.player].reserves;
	game_dialog[INFO_RESOURCES].dp2 = &players[game.player].resources;
	
	// In the setup turn no other operations are required
	if (game.turn==0) return -1;
	
	players[game.player].reserves += players[game.player].reser_t + (cities[game.player] * players[game.player].reser_c);
	players[game.player].resources += players[game.player].resou_t + (cities[game.player] * players[game.player].resou_c);
	if (players[game.player].reserves+res > players[game.player].reser_max) players[game.player].reserves = players[game.player].reser_max-res;

	// Points computing
	players[game.player].points_i = players[game.player].points;
	for (i=0; i<game.tiles; i++) if (MATRIX_PLAYER(i)==game.player) switch(MATRIX_TYPE(i)) {
	case MATRIX_VOID:
	case MATRIX_ARMY:
		players[game.player].points += game.points_tile;
		break;
	case MATRIX_CITY:
		players[game.player].points += game.points_city;
		break;
	}

	return -1;
}

/*
 * unsigned char compute_m(int i)
 * Computes the binary value of surrounding tiles
 * Parameters
 * 	i the tile for which the computation is
 * Returns
 * 	the binary value
 */
unsigned char compute_m(int i) {
	int x, y;
	unsigned char m=0;
	
	x = i % game.row_x;
	y = i / game.row_x;
	
	if (x-1>=0) m|=0x01;
	if (y-1>=0) m|=0x02;
	if (x+1<game.row_x) m|=0x04;
	if (y+1<game.row_y) m|=0x08;

	return m;
}

/*
 * void compute_values(int i)
 * Print data into game labels' string buffers
 * Parameters
 * 	i the tile for which information is printed
 */
void compute_values(int i) {
	int att, def;

	game_dialog[INFO_ATTACK].flags |= D_DIRTY;
	game_dialog[INFO_DEFENSE].flags |= D_DIRTY;
	game_dialog[INFO_SUCCESS].flags |= D_DIRTY;

	if (MATRIX_TYPE(i)==MATRIX_WATER) {
		sprintf(info_attack, "N/A");
		sprintf(info_defense, "N/A");
		sprintf(info_success, "N/A");
		return;
	}

	def = compute_def(i);
	
	if (MATRIX_PLAYER(i)==game.player) {
		sprintf(info_attack, "N/A");
		sprintf(info_defense, "%d", def);
		sprintf(info_success, "N/A");
		return;
	}
	
	att = compute_att(i);
	
	if (game.powerup_i==i) att+=game.powerup_v;

	if (att==2) {
		sprintf(info_attack, "2");
		if (game.hide || MATRIX_HIDE(i)) {
			sprintf(info_defense, "%d", def);
			sprintf(info_success, "%d%%", (100*att)/(att+def));
		} else {
			sprintf(info_defense, "??");
			sprintf(info_success, "??");
		}
		return;
	}

	if (att) {
		sprintf(info_attack, "%d", att);
		sprintf(info_defense, "%d", def);
		sprintf(info_success, "%d%%", (100*att)/(att+def));
		return;
	}

	sprintf(info_attack, "N/A");
	sprintf(info_defense, "N/A");
	sprintf(info_success, "N/A");
}

/*
 * compute_def(int i)
 * Computes player's defense value for a tile
 * Parameters
 * 	i the tile
 * Returns
 * 	0 if the tile is not defensable
 *		the defense value
 */
int compute_def(int i) {
	int k, p, n, value;
	unsigned char m;

	switch (MATRIX_TYPE(i)) {
	case MATRIX_VOID:
		value = 2;
		break;
	case MATRIX_ARMY:
		value = 15;
		break;
	case MATRIX_CITY:
		value = 25;
		break;
	default:
		return 0;
	}

	m = compute_m(i);
	p = MATRIX_PLAYER(i);
	
	for (k=0; k<8; k++) {
		n = i+offset[k];
		if (((nears[k]&m)!=nears[k]) || (MATRIX_PLAYER(n)!=p)) continue;
		switch (MATRIX_TYPE(n)) {
		case MATRIX_ARMY:
		case MATRIX_CITY:
			value+=5;
		}
	}
	
	if (players[game.player].effect!=100) value = ((value+1)*75)/100;

	return value;
}

/*
 * compute_att(int i)
 * Computes player's attack value for a tile
 * Parameters
 * 	i the tile
 * Returns
 * 	0 if the tile is not attackable
 *		the attack value
 */
int compute_att(int i) {
	int k, n, value=0, flag=0;
	unsigned char m;

	if (MATRIX_PLAYER(i)==game.player) return 0;
	
	m = compute_m(i);
	
	for (k=0; k<8; k++) {
		n = i+offset[k];
		if (((nears[k]&m)!=nears[k]) || (MATRIX_PLAYER(n)!=game.player)) continue;
		switch (MATRIX_TYPE(n)) {
		case MATRIX_VOID:
			flag=1;
			break;
		case MATRIX_ARMY:
			value+=5;
			flag=1;
			break;
		case MATRIX_CITY:
			value+=3;
			flag=1;
			break;
		}
	}

	if (flag) value+=10; else if (MATRIX_CROSS(i)) value=2;
	
	if (players[game.player].effect!=100) value = ((value+1)*75)/100;

	return value;
}

/*
 * ding
 * Function that plays a "ding"
 */
void ding(int sample) {
	play_sample(datafile[sample].dat, 255, 127, 1000, 0);
}
