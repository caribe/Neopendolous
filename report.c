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

#include "report.h"

char strbuf[21][8];
char strbuf2[8][24];

DIALOG report_dialog[] = {
	/* (dialog proc)    (x)  (y)  (w)  (h)  (fg)(bg)(key)  (flags) (d1) (d2) (dp) (dp2) (dp3) */
	{ d_box_proc,      480,   0, 160, 480, 255,   0,   0,      0,   0,   0, NULL, NULL, NULL  },
	{ d_bitmap_proc,     0,   0, 480, 480, 255,   0,   0,      0,   0,   0, NULL, NULL, NULL  },
	{ d_button_proc,   520,  10,  80,  18, 255,   0, 'd', D_EXIT,   0,   0, "&Done", NULL, NULL  },

	{ d_frame_proc,     80,  20, 320, 123, 255,   0,   0,      0,   0,   0, "Scenario Settings", NULL, NULL },
	{ d_text_proc,      85,  40, 100,  12, 255,   0,   0,      0,   0,   0, "Name :", NULL, NULL  },
	{ d_text_proc,      85,  54,  60,  12, 255,   0,   0,      0,   0,   0, "Size : X Sectors", NULL, NULL  },
	{ d_text_proc,      85,  68,  60,  12, 255,   0,   0,      0,   0,   0, "Size : Y Sectors", NULL, NULL  },
	{ d_text_proc,      85,  82,  60,  12, 255,   0,   0,      0,   0,   0, "Turns : Last", NULL, NULL  },
	{ d_text_proc,      85,  96,  60,  12, 255,   0,   0,      0,   0,   0, "Turns : Current", NULL, NULL  },
	{ d_text_proc,	     85, 110,  60,  12, 255,   0,   0,      0,   0,   0, "Points : Sectors", NULL, NULL  },
	{ d_text_proc,	     85, 124,  60,  12, 255,   0,   0,      0,   0,   0, "Points : Cities", NULL, NULL  },
	{ d_edit_proc,     130,  40, 100,  12, 255,   0,   0,      0,   0,   0, game.name, NULL, NULL  },
	{ d_edit_proc,     200,  54, 100,  12, 255,   0,   0,      0,   0,   0, strbuf[0], NULL, NULL  },
	{ d_edit_proc,     200,  68,  30,  12, 255,   0,   0,      0,   0,   0, strbuf[1], NULL, NULL  },
	{ d_edit_proc,     200,  82,  30,  12, 255,   0,   0,      0,   0,   0, strbuf[2], NULL, NULL  },
	{ d_edit_proc,     200,  96,  30,  12, 255,   0,   0,      0,   0,   0, strbuf[3], NULL, NULL  },
	{ d_edit_proc,     200, 110,  30,  12, 255,   0,   0,      0,   0,   0, strbuf[4], NULL, NULL  },
	{ d_edit_proc,     200, 124,  30,  12, 255,   0,   0,      0,   0,   0, strbuf[5], NULL, NULL  },
	{ d_text_proc,     245,  40, 100,  12, 255,   0,   0,      0,   0,   0, "Barrier Crossing", NULL, NULL },
	{ d_text_proc,     245,  54, 100,  12, 255,   0,   0,      0,   0,   0, "Hidden units", NULL, NULL },
	{ d_text_proc,     245,  68, 100,  12, 255,   0,   0,      0,   0,   0, "Hidden sectors", NULL, NULL },

	{ d_frame_proc,     80, 160, 320, 205, 0xff,  0,   0,      0,   0,   0, "Players' Status", NULL, NULL  },
	{ d_list2_proc,     85, 180, 100, 180, 0xff,  1,   0,      0,   0,   0, players_list, NULL, NULL  },
	{ d_text_proc,     200, 180, 100,  10, 0xff,  0,   0,      0,   0,   0, "Total Offenses", NULL, NULL  },
	{ d_text_proc,     200, 192, 100,  10, 0xff,  0,   0,      0,   0,   0, "Total Defenses", NULL, NULL  },
	{ d_text_proc,     200, 204, 100,  10, 0xff,  0,   0,      0,   0,   0, "Offense Success %", NULL, NULL  },
	{ d_text_proc,     200, 216, 100,  10, 0xff,  0,   0,      0,   0,   0, "Defense Success %", NULL, NULL  },
	{ d_text_proc,     200, 228, 100,  10, 0xff,  0,   0,      0,   0,   0, "Averange Offense %", NULL, NULL  },
	{ d_text_proc,     200, 240, 100,  10, 0xff,  0,   0,      0,   0,   0, "Averange Defense %", NULL, NULL  },
	{ d_text_proc,     200, 252, 100,  10, 0xff,  0,   0,      0,   0,   0, "Units per turn", NULL, NULL  },
	{ d_text_proc,     200, 264, 100,  10, 0xff,  0,   0,      0,   0,   0, "Units per base", NULL, NULL  },
	{ d_text_proc,     200, 276, 100,  10, 0xff,  0,   0,      0,   0,   0, "Resources per turn", NULL, NULL  },
	{ d_text_proc,     200, 288, 100,  10, 0xff,  0,   0,      0,   0,   0, "Resources per base", NULL, NULL  },
	{ d_text_proc,     200, 300, 100,  10, 0xff,  0,   0,      0,   0,   0, "Maximum units", NULL, NULL  },
	{ d_text_proc,     200, 312, 100,  10, 0xff,  0,   0,      0,   0,   0, "Units in reserve", NULL, NULL  },
	{ d_text_proc,     200, 324, 100,  10, 0xff,  0,   0,      0,   0,   0, "Resources", NULL, NULL  },
	{ d_text_proc,     200, 336, 100,  10, 0xff,  0,   0,      0,   0,   0, "Points", NULL, NULL  },
	{ d_text_proc,     200, 348, 100,  10, 0xff,  0,   0,      0,   0,   0, "Unit effectiveness", NULL, NULL  },
	{ d_text_proc,     300, 180,  50,  10, 0xff,  0,   0,      0,   0,   0, strbuf[6], NULL, NULL },
	{ d_text_proc,     300, 192,  50,  10, 0xff,  0,   0,      0,   0,   0, strbuf[7], NULL, NULL },
	{ d_text_proc,     300, 204,  50,  10, 0xff,  0,   0,      0,   0,   0, strbuf[8], NULL, NULL },
	{ d_text_proc,     300, 216,  50,  10, 0xff,  0,   0,      0,   0,   0, strbuf[9], NULL, NULL },
	{ d_text_proc,     300, 228,  50,  10, 0xff,  0,   0,      0,   0,   0, strbuf[10], NULL, NULL },
	{ d_text_proc,     300, 240,  50,  10, 0xff,  0,   0,      0,   0,   0, strbuf[11], NULL, NULL },
	{ d_text_proc,     300, 252,  50,  10, 0xff,  0,   0,      0,   0,   0, strbuf[12], NULL, NULL },
	{ d_text_proc,     300, 264,  50,  10, 0xff,  0,   0,      0,   0,   0, strbuf[13], NULL, NULL },
	{ d_text_proc,     300, 276,  50,  10, 0xff,  0,   0,      0,   0,   0, strbuf[14], NULL, NULL },
	{ d_text_proc,     300, 288,  50,  10, 0xff,  0,   0,      0,   0,   0, strbuf[15], NULL, NULL },
	{ d_text_proc,     300, 300,  50,  10, 0xff,  0,   0,      0,   0,   0, strbuf[16], NULL, NULL },
	{ d_text_proc,     300, 312,  50,  10, 0xff,  0,   0,      0,   0,   0, strbuf[17], NULL, NULL },
	{ d_text_proc,     300, 324,  50,  10, 0xff,  0,   0,      0,   0,   0, strbuf[18], NULL, NULL },
	{ d_text_proc,     300, 336,  50,  10, 0xff,  0,   0,      0,   0,   0, strbuf[19], NULL, NULL },
	{ d_text_proc,     300, 348,  50,  10, 0xff,  0,   0,      0,   0,   0, strbuf[20], NULL, NULL },

	{ d_frame_proc,     80, 380, 320,  80, 0xff,  0,   0,      0,   0,   0, "Players Rankings", NULL, NULL  },
	{ d_list3_proc,     85, 400, 100,  55, 0xff,  1,   0,      0,   0,   0, ranking_list, NULL, NULL  },
	{ d_text_proc,     200, 400, 100,  10, 0xff,  0,   0,      0,   0,   0, strbuf2[0], NULL, NULL },
	{ d_text_proc,     200, 412, 100,  10, 0xff,  0,   0,      0,   0,   0, strbuf2[1], NULL, NULL },
	{ d_text_proc,     200, 424, 100,  10, 0xff,  0,   0,      0,   0,   0, strbuf2[2], NULL, NULL },
	{ d_text_proc,     200, 436, 100,  10, 0xff,  0,   0,      0,   0,   0, strbuf2[3], NULL, NULL },
	{ d_text_proc,     300, 400, 100,  10, 0xff,  0,   0,      0,   0,   0, strbuf2[4], NULL, NULL },
	{ d_text_proc,     300, 412, 100,  10, 0xff,  0,   0,      0,   0,   0, strbuf2[5], NULL, NULL },
	{ d_text_proc,     300, 424, 100,  10, 0xff,  0,   0,      0,   0,   0, strbuf2[6], NULL, NULL },
	{ d_text_proc,     300, 436, 100,  10, 0xff,  0,   0,      0,   0,   0, strbuf2[7], NULL, NULL },

	{ NULL,              0,   0,   0,   0,    0,  0,   0,      0,   0,   0, NULL, NULL, NULL  }
};

int d_list2_proc(int msg, DIALOG *d, int c) {
	int ret = d_list_proc(msg, d, c);
	if (msg==MSG_START || msg==MSG_CLICK || msg==MSG_CHAR) {
		sprintf(strbuf[6], "%d", players[d->d1].attacks);
		sprintf(strbuf[7], "%d", players[d->d1].defenses);
		
		if (players[d->d1].attacks>0) {
			sprintf(strbuf[8], "%d", (100*players[d->d1].ok_attacks)/players[d->d1].attacks);
			sprintf(strbuf[10], "%d", players[d->d1].mean_attacks/players[d->d1].attacks);
		} else {
			sprintf(strbuf[8], "0");
			sprintf(strbuf[10], "0");
		}

		if (players[d->d1].defenses>0) {
			sprintf(strbuf[9], "%d", (100*players[d->d1].ok_defenses)/players[d->d1].defenses);
			sprintf(strbuf[11], "%d", players[d->d1].mean_defenses/players[d->d1].defenses);
		} else {
			sprintf(strbuf[9], "0");
			sprintf(strbuf[11], "0");
		}
		sprintf(strbuf[12], "%d", players[d->d1].reser_t);
		sprintf(strbuf[13], "%d", players[d->d1].reser_c);
		sprintf(strbuf[14], "%d", players[d->d1].resou_t);
		sprintf(strbuf[15], "%d", players[d->d1].resou_c);
		sprintf(strbuf[16], "%d", players[d->d1].reser_max);
		sprintf(strbuf[17], "%d", players[d->d1].reserves);
		sprintf(strbuf[18], "%d", players[d->d1].resources);
		sprintf(strbuf[19], "%d", players[d->d1].points);
		sprintf(strbuf[20], "%d", players[d->d1].effect);

		for (c=22; c<54; c++) report_dialog[c].flags |= D_DIRTY;
	}
	return ret;
}

int d_list3_proc(int msg, DIALOG *d, int c) {
	int ret = d_list_proc(msg, d, c);
	if (msg==MSG_START || msg==MSG_CLICK || msg==MSG_CHAR) {
		int i, s, order[] = {0, 1, 2, 3, 4, 5, 6, 7}, values[8];

		switch(d->d1) {
		case 0:
			for(i=0; i<8; i++) values[i] = players[i].points;
			break;
		case 1:
			for(i=0; i<8; i++) values[i] = players[i].points-players[i].points_i;
			break;
		case 2:
			for(i=0; i<8; i++) values[i] = players[i].attacks;
			break;
		case 3:
			for(i=0; i<8; i++) values[i] = players[i].defenses;
			break;
		case 4:
			for(i=0; i<8; i++) if (players[i].attacks>0) values[i] = (100*players[i].ok_attacks)/players[i].attacks; else values[i]=0;
			break;
		case 5:
			for(i=0; i<8; i++) if (players[i].defenses>0) values[i] = (100*players[i].ok_defenses)/players[i].defenses; else values[i]=0;
			break;
		}

		i=0;
		do {
			if (values[order[i]] < values[order[i+1]]) {
				s = order[i];
				order[i] = order[i+1];
				order[i+1] = s;
				i=0;
				continue;
			} else {
				i++;
			}
		} while (i<7);
		
		for (i=0; i<8; i++) sprintf(strbuf2[i], "%d.%s (%d)", (i+1), players[order[i]].name, values[order[i]]);
		for (c=54; c<64; c++) report_dialog[c].flags |= D_DIRTY;
	}
	
	return ret;
}

char *players_list(int index, int *list_size) {
	if (index<0) {
		*list_size = 8;
		return NULL;
	}
	return players[index].name;
}

char *ranking_list(int index, int *list_size) {
	static char *labels[] = {"Points", "Points trend", "Attacks", "Defenses", "Attack %", "Defense %"};

	if (index<0) {
		*list_size = 5;
		return NULL;
	}
	return labels[index];
}

void report() {
	int ret;

	for(ret=0; report_dialog[ret].proc; ret++) {
		report_dialog[ret].fg = colors[black];
		if (report_dialog[ret].bg==0) report_dialog[ret].bg = colors[gray];
		else if (report_dialog[ret].bg==1) report_dialog[ret].bg = colors[white];
	}
	report_dialog[0].bg = colors[darkgray];
	report_dialog[1].dp = datafile[DATA_SCREEN].dat;

	for (ret=0; ret<29; ret++) strbuf[ret][0]='\0';
	for (ret=0; ret<8; ret++) strbuf2[ret][0]='\0';

	sprintf(strbuf[0], "%d", game.row_x);
	sprintf(strbuf[1], "%d", game.row_y);
	sprintf(strbuf[2], "%d", game.turns);
	sprintf(strbuf[3], "%d", game.turn);
	sprintf(strbuf[4], "%d", game.points_tile);
	sprintf(strbuf[5], "%d", game.points_city);

	report_dialog[18].dp = (game.fog ? "Unexplored land is not hidden" : "Unexplored land is hidden");
	report_dialog[19].dp = (game.hide ? "Opposing units are not hidden" : "Opposing units are hidden");
	report_dialog[20].dp = (game.cross ? "Barrier crossing allowed" : "Barrier crossing not allowed");

	do_dialog(report_dialog, -1);
}
