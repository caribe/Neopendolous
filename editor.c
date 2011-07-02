#include "editor.h"

#define B_LOAD 1
#define B_SAVE 2
#define B_MAP 3
#define B_EXIT 4
#define B_CROSS 19
#define B_HIDE 20
#define B_FOG 21
#define PLAYBOX 22
#define B_2NEXT 43
#define B_2REST 44
#define B_2ALL 45
#define B_PREV 46
#define B_NEXT 47

char strbuf[8];

DIALOG edit_dialog[] =
{
	/* (dialog proc)    (x)  (y)  (w)  (h) (fg) (bg)(key) (flags) (d1) (d2) (dp) (dp2) (dp3) */
	{ d_box_proc,      480,   0, 160, 480,   0,   0,   0,      0,   0,   0, NULL, NULL, NULL  },
	{ d_button_proc,   520,  10,  80,  18, 255,   0, 'l', D_EXIT,   1,   0, "&Load", NULL, NULL },
	{ d_button_proc,   520,  34,  80,  18, 255,   0, 's', D_EXIT,   1,   0, "&Save", NULL, NULL },
	{ d_button_proc,   520,  58,  80,  18, 255,   0, 'm', D_EXIT,   1,   0, "&Map edit", NULL, NULL },
	{ d_button_proc,   520,  82,  80,  18, 255,   0, 'q', D_EXIT,   1,   0, "&Quit", NULL, NULL },

	{ d_bitmap_proc,     0,   0, 480, 480,   0,   0,   0,      0,   0,   0, NULL, NULL, NULL  },
	
	{ d_frame_proc,     80,  80, 320, 107, 255,   0,   0,      0,   0,   0, "Scenario Settings", NULL, NULL },
	{ d_text_proc,      85, 100, 100,  12, 255,   0,   0,      0,   0,   0, "Name", NULL, NULL  },
	{ d_text_proc,      85, 114,  60,  12, 255,   0,   0,      0,   0,   0, "Size : X Sectors", NULL, NULL  },
	{ d_text_proc,      85, 128,  60,  12, 255,   0,   0,      0,   0,   0, "Size : Y Sectors", NULL, NULL  },
	{ d_text_proc,      85, 142,  60,  12, 255,   0,   0,      0,   0,   0, "Number of Turns", NULL, NULL  },
	{ d_text_proc,      85, 156,  60,  12, 255,   0,   0,      0,   0,   0, "Points : Sectors", NULL, NULL  },
	{ d_text_proc,	     85, 170,  60,  12, 255,   0,   0,      0,   0,   0, "Points : Cities", NULL, NULL  },
	{ d_edit_proc,     130, 100, 100,  12, 255,   0,   0,      0,  15,   0, game.name, NULL, NULL  },
	{ d_edit_proc,     200, 114,  30,  12, 255,   0,   0,      0,   4,   0, x_sectors, NULL, NULL  },
	{ d_edit_proc,     200, 128,  30,  12, 255,   0,   0,      0,   4,   0, y_sectors, NULL, NULL  },
	{ d_edit_proc,     200, 142,  30,  12, 255,   0,   0,      0,   4,   0, turns, NULL, NULL  },
	{ d_edit_proc,     200, 156,  30,  12, 255,   0,   0,      0,   4,   0, sector_points, NULL, NULL  },
	{ d_edit_proc,     200, 170,  30,  12, 255,   0,   0,      0,   4,   0, base_points, NULL, NULL  },
	{ d_check_proc,    245, 100, 100,  12, 255,   0,   0, D_EXIT,   1,   0, "Barrier Crossing", NULL, NULL },
	{ d_check_proc,    245, 114, 100,  12, 255,   0,   0, D_EXIT,   1,   0, "Hidden units", NULL, NULL },
	{ d_check_proc,    245, 128, 100,  12, 255,   0,   0, D_EXIT,   1,   0, "Hidden sectors", NULL, NULL },

	{ d_frame_proc,     80, 230, 320, 163, 255,   0,   0,      0,   0,   0, "Players Settings", NULL, NULL },
	{ d_text_proc,      85, 250,  60,  12, 255,   0,   0,      0,   0,   0, strbuf, NULL, NULL  },
	{ d_text_proc,      85, 264,  60,  12, 255,   0,   0,      0,   0,   0, "Units per turn", NULL, NULL  },
	{ d_text_proc,      85, 278,  60,  12, 255,   0,   0,      0,   0,   0, "Units per city", NULL, NULL  },
	{ d_text_proc,      85, 292,  60,  12, 255,   0,   0,      0,   0,   0, "Resources per turn", NULL, NULL  },
	{ d_text_proc,      85, 306,  60,  12, 255,   0,   0,      0,   0,   0, "Resources per city", NULL, NULL  },
	{ d_text_proc,      85, 320,  60,  12, 255,   0,   0,      0,   0,   0, "Maximum units", NULL, NULL  },
	{ d_text_proc,      85, 334,  60,  12, 255,   0,   0,      0,   0,   0, "Units at start", NULL, NULL  },
	{ d_text_proc,      85, 348,  60,  12, 255,   0,   0,      0,   0,   0, "Resources at start", NULL, NULL  },
	{ d_text_proc,      85, 362,  60,  12, 255,   0,   0,      0,   0,   0, "Points at start", NULL, NULL  },
	{ d_text_proc,      85, 376,  60,  12, 255,   0,   0,      0,   0,   0, "Unit effectiveness", NULL, NULL  },
	{ d_edit_proc,     130, 250, 100,  12, 255,   0,   0,      0,  15,   0, play.name, NULL, NULL },
	{ d_edit_proc,     200, 264,  30,  12, 255,   0,   0,      0,   4,   0, play.reser_t, NULL, NULL  },
	{ d_edit_proc,     200, 278,  30,  12, 255,   0,   0,      0,   4,   0, play.reser_c, NULL, NULL  },
	{ d_edit_proc,     200, 292,  30,  12, 255,   0,   0,      0,   4,   0, play.resou_t, NULL, NULL  },
	{ d_edit_proc,     200, 306,  30,  12, 255,   0,   0,      0,   4,   0, play.resou_c, NULL, NULL  },
	{ d_edit_proc,     200, 320,  30,  12, 255,   0,   0,      0,   4,   0, play.reser_max, NULL, NULL  },
	{ d_edit_proc,     200, 334,  30,  12, 255,   0,   0,      0,   4,   0, play.reserves, NULL, NULL  },
	{ d_edit_proc,     200, 348,  30,  12, 255,   0,   0,      0,   4,   0, play.resources, NULL, NULL  },
	{ d_edit_proc,     200, 362,  30,  12, 255,   0,   0,      0,   4,   0, play.points, NULL, NULL  },
	{ d_edit_proc,     200, 376,  30,  12, 255,   0,   0,      0,   4,   0, play.effect, NULL, NULL  },
	{ d_button_proc,   280, 322,  80,  18, 255,   0, 'c', D_EXIT,   1,   0, "&Copy to Next", NULL, NULL },
	{ d_button_proc,   280, 346,  80,  18, 255,   0, 'r', D_EXIT,   1,   0, "Copy to &Rest", NULL, NULL },
	{ d_button_proc,   280, 370,  80,  18, 255,   0, 'a', D_EXIT,   1,   0, "Copy to &All", NULL, NULL },
	{ d_button_proc,   280, 250,  80,  18, 255,   0, 'p', D_EXIT,   1,   0, "&Previous", NULL, NULL },
	{ d_button_proc,   280, 274,  80,  18, 255,   0, 'n', D_EXIT,   1,   0, "&Next", NULL, NULL },

	{ NULL,              0,   0,   0,   0,   0,   0,   0,      0,   0,   0, NULL, NULL, NULL  }
};

DIALOG mapedit_dialog[] =
{
	/* (dialog proc)    (x)  (y)  (w)  (h) (fg) (bg)(key) (flags) (d1) (d2) (dp) (dp2) (dp3) */
	{ d_box_proc,      480,   0, 160, 480,   0,   0,   0,      0,   0,   0, NULL, NULL, NULL  },
	{ map_edit_proc,     0,   0, 480, 480,   0,   0,   0,      0,   0,   0, NULL, NULL, NULL  },
	{ d_button_proc,   520,  10,  80,  18,   0,   0, 'd', D_EXIT,   0,   0, "&Done", NULL, NULL  },
	{ d_button_proc,   520,  34,  80,  18,   0,   0, 'w', D_EXIT,   0,   0, "&World Map", NULL, NULL  },
	{ d_button_proc,   520,  58,  80,  18,   0,   0, 'g', D_EXIT,   0,   0, "&Generate Map", NULL, NULL  },
	{ d_button_proc,   520,  82,  80,  18,   0,   0, 's', D_EXIT,   0,   0, "Map &Settings", NULL, NULL  },
	{ d_list_proc,     500, 130, 120,  52,   0,   0,   0,      0,   0,   0, tiles_list, NULL, NULL  },
	{ d_list_proc,     500, 200, 120, 104,   0,   0,   0,      0,   0,   0, player_list, NULL, NULL  },

	{ NULL,              0,   0,   0,   0,   0,   0,   0,      0,   0,   0, NULL, NULL, NULL  }
};

/*
 * Allegro GUI procedure for editing the map.
 */
int map_edit_proc(int msg, DIALOG *d, int c) {
	switch(msg) {
	// Simply calls the graph's draw_proc
	case MSG_DRAW:
		for (c=0; c<game.tiles; c++) {
			matrix[c].info |= 0x40;
			matrix[c].fog = 0xff;
		}
		draw_proc(d);
		return D_O_K;

	// Checks if the mouse has been moved and update the map accordently
	case MSG_IDLE:
		{
			int x, y;
	
			if ((d->flags & D_GOTMOUSE)==0) return D_O_K;
	
			x = (mouse_x - d->x)/24;
			y = (mouse_y - d->y)/24;
	
			if (y==0 && game.pane_y>0) {
				game.pane_y--;
				d->flags |= D_DIRTY;
			}
			if (y==19 && game.pane_y<game.row_y-20) {
				game.pane_y++;
				d->flags |= D_DIRTY;
			}
			if (x==0 && game.pane_x>0) {
				game.pane_x--;
				d->flags |= D_DIRTY;
			}				
			if (x==19 && game.pane_x<game.row_x-20) {
				game.pane_x++;
				d->flags |= D_DIRTY;
			}
			return D_O_K;
		}

	case MSG_WANTFOCUS:
		return D_WANTFOCUS;

	// Changes the tile by the listboxes selections
	case MSG_CLICK:
		{
			int x, y, i;
	
			x = (mouse_x - d->x)/24 + game.pane_x;
			y = (mouse_y - d->y)/24 + game.pane_y;
			i = y*game.row_x+x;
	
			matrix[i].info = mapedit_dialog[7].d1;
			switch (mapedit_dialog[6].d1) {
			case 0:
				matrix[i].info |= MATRIX_WATER;
				break;
			case 1:
				matrix[i].info |= MATRIX_VOID;
				break;
			case 2:
				matrix[i].info |= MATRIX_ARMY;
				break;
			case 3:
				matrix[i].info |= MATRIX_CITY;
				break;
			}
			d->flags |= D_DIRTY;
			game.random=0xffff;
			return D_O_K;
		}
	}
	return D_O_K;
}

char *player_list(int index, int *list_size) {
	if (index<0) {
		*list_size = 8;
		return NULL;
	}
	return players[index].name;
}

char *tiles_list(int index, int *list_size) {
	static char *s[] = {"Water", "Void", "Army", "City"};

	if (index<0) {
		*list_size = 4;
		return NULL;
	}
	return s[index];
}

void save_frame() {
	int i;

	game.pane_x = game.pane_y = 0;
	game.row_x = (unsigned short)strtol(x_sectors, NULL, 10);
	if (game.row_x>50) game.row_x = 50;
	game.row_y = (unsigned short)strtol(y_sectors, NULL, 10);
	if (game.row_y>50) game.row_y = 50;
	game.tiles = game.row_x * game.row_y;
	game.turns = (unsigned char)strtol(turns, NULL, 10);
	game.points_tile = (unsigned char)strtol(sector_points, NULL, 10);
	game.points_city = (unsigned char)strtol(base_points, NULL, 10);
	game.fog = ((edit_dialog[B_FOG].flags & D_SELECTED) ? 0 : 0xff);
	for (i=0; i<game.tiles; i++) matrix[i].fog = game.fog;
	game.hide = ((edit_dialog[B_HIDE].flags & D_SELECTED) ? 0 : 0xff);
	game.cross = ((edit_dialog[B_CROSS].flags & D_SELECTED) ? 1 : 0);

	strcpy(players[player].name, play.name);
	players[player].reser_t = (unsigned char)strtol(play.reser_t, NULL, 10);
	players[player].resou_t = (unsigned char)strtol(play.resou_t, NULL, 10);
	players[player].reser_c = (unsigned char)strtol(play.reser_c, NULL, 10);
	players[player].resou_c = (unsigned char)strtol(play.resou_c, NULL, 10);
	players[player].resources = (unsigned short)strtol(play.resources, NULL, 10);
	players[player].reserves = (unsigned short)strtol(play.reserves, NULL, 10);
	players[player].points = (unsigned int)strtol(play.points, NULL, 10);
	players[player].reser_max = (unsigned short)strtol(play.reser_max, NULL, 10);
	players[player].effect = (unsigned char)strtol(play.effect, NULL, 10);
}

void start_editor() {
	int ret;

// Colors initialization
	for (ret=1; edit_dialog[ret].proc; ret++) {
		edit_dialog[ret].fg = colors[black];
		edit_dialog[ret].bg = ((edit_dialog[ret].proc==d_edit_proc) ? colors[white] : colors[gray]);
	}
	edit_dialog[0].bg = colors[darkgray];
	edit_dialog[5].dp = datafile[DATA_SCREEN].dat;

	for (ret=1; mapedit_dialog[ret].proc; ret++) {
		mapedit_dialog[ret].fg = colors[black];
		mapedit_dialog[ret].bg = ((mapedit_dialog[ret].proc==d_list_proc) ? colors[white] : colors[gray]);
	}
	mapedit_dialog[0].bg = colors[darkgray];

// Default game values
	game.row_x = game.row_y = 25;
	game.tiles = game.row_x * game.row_y;
	game.turns = 10;
	game.points_tile = 1;
	game.points_city = 10;
	game.fog = game.hide = 0xff;
	game.cross = 0;
	game.turn = -1;
	game.turns = 10;
	game.player = 0;
	game.pane_x = game.pane_y = 0;
	game.powerup_i=(unsigned short)-1;
	game.powerup_v=0;
	game.random=0xffff;
	strcpy(game.name, "Scenario");
	strcpy(game.description, "Scenario description");

// Default players values
	for (ret=0; ret<8; ret++) {
		sprintf(players[ret].name, "Player %d", ret);
		players[ret].reser_t = players[ret].resou_t = players[ret].reser_c = players[ret].resou_c = players[ret].resources = players[ret].reserves = players[ret].points = 0;
		players[ret].reser_max = 50;
		players[ret].effect = 100;
	}
	player=0;

	for (ret=0; ret<2500; ret++) {
		matrix[ret].info = 0x08;
		matrix[ret].fog = 0xff;
	}

	while (1) {
		sprintf(x_sectors, "%d", game.row_x);
		sprintf(y_sectors, "%d", game.row_y);
		sprintf(turns, "%d", game.turns);
		sprintf(sector_points, "%d", game.points_tile);
		sprintf(base_points, "%d", game.points_city);

		sprintf(strbuf, "Name #%d", player+1);
		strcpy(play.name, players[player].name);
		sprintf(play.reser_t, "%d", players[player].reser_t);
		sprintf(play.resou_t, "%d", players[player].resou_t);
		sprintf(play.reser_c, "%d", players[player].reser_c);
		sprintf(play.resou_c, "%d", players[player].resou_c);
		sprintf(play.resources, "%d", players[player].resources);
		sprintf(play.reserves, "%d", players[player].reserves);
		sprintf(play.points, "%d", players[player].points);
		sprintf(play.reser_max, "%d", players[player].reser_max);
		sprintf(play.effect, "%d", players[player].effect);

		edit_dialog[B_FOG].flags = (game.fog ? 0 : D_SELECTED);
		edit_dialog[B_HIDE].flags = (game.hide ? 0 : D_SELECTED);
		edit_dialog[B_CROSS].flags = (game.cross ? D_SELECTED : 0);

		show_mouse(screen);

		ret = do_dialog(edit_dialog, -1);
		switch (ret) {
		case B_SAVE:
			save_frame();
			save_scenario(SL_SCENARIO);
		break;
		case B_LOAD:
			load_scenario(SL_SCENARIO);
			break;
		case B_EXIT:
			if (alert("Save the current scenario?", NULL, NULL, "&Yes", "&No", 'y', 'n')==1) save_scenario(SL_SCENARIO);
			return;
		case B_MAP:
			save_frame();
			for(ret=0; ret<game.tiles; ret++) matrix[ret].fog = 0xFF;
			start_mapedit();
			for(ret=0; ret<game.tiles; ret++) matrix[ret].fog = 0;
			break;
		case B_2NEXT:
			save_frame();
			players[(player+1)&0x07] = players[player];
			break;
		case B_2REST:
			save_frame();
			for (ret=player+1; ret<8; ret++) players[ret]=players[player];
			break;
		case B_2ALL:
			save_frame();
			for (ret=0; ret<8; ret++) players[ret]=players[player];
			break;
		case B_NEXT:
			save_frame();
			player++;
			if (player>7) player=0;
			break;
		case B_PREV:
			save_frame();
			player--;
			if (player<0) player=7;
			break;
		}
	}
}

void start_mapedit() {
	while(1) {
		switch(do_dialog(mapedit_dialog, 2)) {
		case 3:
			start_minimap(1);
			break;
		case 4:
			start_mapgen(1);
			break;
		case 5:
			start_mapgen(0);
			break;
		default:
			return;
		}
	}
}
