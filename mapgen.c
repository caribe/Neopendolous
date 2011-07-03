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

#include "mapgen.h"

char strbuf[9][6];

DIALOG genmap_dialog[] = {
	/* (dialog proc)    (x)  (y)  (w)  (h) (fg) (bg)(key) (flags) (d1) (d2) (dp) (dp2) (dp3) */
	{ d_frame_proc,    170, 200, 300, 165,   0,   0,   0,      0,   0,   0, "Map Generation Settings", NULL, NULL  },
	
	{ d_button_proc,   230, 335,  80,  18,   0,   0, 'o', D_EXIT,   0,   0, "&Okay", NULL, NULL  },	
	{ d_button_proc,   330, 335,  80,  18,   0,   0, 'c', D_EXIT,   0,   0, "&Cancel", NULL, NULL  },	

	{ d_text_proc,     175, 220,  80,  12,   0,   0,   0,      0,   0,   0, "Please insert the cities for each player", NULL, NULL  },
	{ d_text_proc,     175, 234,  80,  12,   0,   0,   0,      0,   0,   0, players[0].name, NULL, NULL  },
	{ d_text_proc,     175, 248,  80,  12,   0,   0,   0,      0,   0,   0, players[1].name, NULL, NULL  },
	{ d_text_proc,     175, 262,  80,  12,   0,   0,   0,      0,   0,   0, players[2].name, NULL, NULL  },
	{ d_text_proc,     175, 276,  80,  12,   0,   0,   0,      0,   0,   0, players[3].name, NULL, NULL  },
	{ d_text_proc,     325, 234,  80,  12,   0,   0,   0,      0,   0,   0, players[4].name, NULL, NULL  },
	{ d_text_proc,     325, 248,  80,  12,   0,   0,   0,      0,   0,   0, players[5].name, NULL, NULL  },
	{ d_text_proc,     325, 262,  80,  12,   0,   0,   0,      0,   0,   0, players[6].name, NULL, NULL  },
	{ d_text_proc,     325, 276,  80,  12,   0,   0,   0,      0,   0,   0, players[7].name, NULL, NULL  },

	{ d_edit_proc,     280, 234,  30,  12,   0,   0,   0,      0,   4,   0, strbuf[0], NULL, NULL  },
	{ d_edit_proc,     280, 248,  30,  12,   0,   0,   0,      0,   4,   0, strbuf[1], NULL, NULL  },
	{ d_edit_proc,     280, 262,  30,  12,   0,   0,   0,      0,   4,   0, strbuf[2], NULL, NULL  },
	{ d_edit_proc,     280, 276,  30,  12,   0,   0,   0,      0,   4,   0, strbuf[3], NULL, NULL  },
	{ d_edit_proc,     430, 234,  30,  12,   0,   0,   0,      0,   4,   0, strbuf[4], NULL, NULL  },
	{ d_edit_proc,     430, 248,  30,  12,   0,   0,   0,      0,   4,   0, strbuf[5], NULL, NULL  },
	{ d_edit_proc,     430, 262,  30,  12,   0,   0,   0,      0,   4,   0, strbuf[6], NULL, NULL  },
	{ d_edit_proc,     430, 276,  30,  12,   0,   0,   0,      0,   4,   0, strbuf[7], NULL, NULL  },
	
	{ d_text_proc,     175, 300,  80,  12,   0,   0,   0,      0,   0,   0, "Barrier level (0-20)", NULL, NULL},
	{ d_edit_proc,     280, 300,  30,  12,   0,   0,   0,      0,   4,   0, strbuf[8], NULL, NULL},
	{ d_check_proc,    175, 314, 100,  12,   0,   0,   0,      0,   1,   0, "Autogenerate Map at Game Start", NULL, NULL},
	
	{ NULL,              0,   0,   0,   0,   0,   0,   0,      0,   0,   0, NULL, NULL, NULL  }
};

void start_mapgen(int mode) {
	int order[game.tiles], c, n, m, p1, p2, len, i, x, y, limit;
	
	genmap_dialog[0].bg = colors[gray];
	for (c=1; genmap_dialog[c].proc; c++) {
		if (genmap_dialog[c].proc==d_edit_proc) genmap_dialog[c].bg = colors[white]; else genmap_dialog[c].bg = colors[gray];
	}

	for (c=0; c<8; c++) sprintf(strbuf[c], "%d", players[c].cities);
	if (game.random==0xffff) {
		genmap_dialog[22].flags &= (!D_SELECTED);
		sprintf(strbuf[8], "5");
	} else {
		genmap_dialog[22].flags |= D_SELECTED;
		sprintf(strbuf[8], "%d", game.random);
	}
	
	if (mode==0) {
		if (do_dialog(genmap_dialog, -1)==2) return;
		for (c=0; c<8; c++) players[c].cities = (int)strtol(strbuf[c], NULL, 10);
		game.random = (int)strtol(strbuf[8], NULL, 10);
	}
	if (game.random>20) game.random=20;

	for (c=0; c<game.tiles; c++) {
		matrix[c].info=0;
		order[c]=-1;
	}
	
	p1 = p2 = 0;
	for (c=0; c<game.random && p1<game.tiles; c++) {
		do n = (rand()%game.tiles); while(matrix[n].info>0);
		matrix[n].info=1;
		order[p1++] = n;
	}
	
	limit = game.tiles*game.random*37/1000;
	
	while (order[p2]>=0 && p2<limit) {
		n = order[p2++];
		matrix[n].info=2;
		len = rand()%3+2;
		
		if (rand()%2) {
			x = n%game.row_x;
			for (c=1; c<len; c++) {
				if (x+c>=game.row_x) break;
				i = n+c;
				if (!matrix[i].info) {
					matrix[i].info = 1;
					order[p1++] = i;
				}
			}
			for (c=1; c<len; c++) {
				if (x-c<0) break;
				i = n-c;
				if (!matrix[i].info) {
					matrix[i].info = 1;
					order[p1++] = i;
				}
			}
		} else {
			y = n/game.row_x;
			for (c=1; c<len; c++) {
				if (y+c>=game.row_y) break;
				i = n+c*game.row_x;
				if (!matrix[i].info) {
					matrix[i].info = 1;
					order[p1++] = i;
				}
			}
			for (c=1; c<len; c++) {
				if (y-c<0) break;
				i = n-c*game.row_x;
				if (!matrix[i].info) {
					matrix[i].info = 1;
					order[p1++] = i;
				}
			}
		}
	}
	
	offset[0] = -1	-game.row_x;
	offset[1] =		-game.row_x;
	offset[2] = +1	-game.row_x;
	offset[3] = +1;
	offset[4] = +1	+game.row_x;
	offset[5] = 	+game.row_x;
	offset[6] = -1	+game.row_x;
	offset[7] = -1;
	
	p2=p1;
	for (i=0; i<8; i++) for (c=0; c<players[i].cities && p1<game.tiles; c++) {
		do n = rand()%game.tiles; while(matrix[n].info>0);
		matrix[n].info=10+i;
		order[p1++]=n;
	}

	while(order[p2]>=0 && p2<game.tiles) {
		n = order[p2++];
		m = compute_m(n);
		for (c=0; c<8; c++) {
			i = n+offset[c];
			if ((nears[c]&m)!=nears[c] || matrix[i].info>0) continue;
			if (matrix[n].info>=20) matrix[i].info=matrix[n].info; else matrix[i].info=matrix[n].info+10;
			order[p1++] = i;
		}
	}

	for (c=0; c<game.tiles; c++) {
		if (matrix[c].info<10) matrix[c].info = MATRIX_WATER;
		else if (matrix[c].info<20) matrix[c].info = MATRIX_CITY + matrix[c].info-10;
		else matrix[c].info = MATRIX_VOID + matrix[c].info-20;
	}
	
	if (mode==0 && (genmap_dialog[22].flags&D_SELECTED)==0) game.random=0xffff;
}
