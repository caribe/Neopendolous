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

extern int colors[COLORS];

volatile short timer;

void game_timer() {
	timer=1;
}
END_OF_FUNCTION(game_timer);

#define DMAIN_START 3
#define DMAIN_EDIT 4
#define DMAIN_EXIT 5

DIALOG main_dialog[] = {
	/* (dialog proc)    (x)  (y)  (w)  (h) (fg) (bg)(key) (flags) (d1) (d2) (dp) (dp2) (dp3) */
	{ d_box_proc,      480,   0, 480, 480,   0,   0,   0,      0,   0,   0, NULL, NULL, NULL  },
	{ d_bitmap_proc,     0,   0, 480, 480,   0,   0,   0,      0,   0,   0, NULL, NULL, NULL  },
	{ d_masked_proc,   110, 240, 263,  30,   0,   0,   0,      0,   0,   0, NULL, NULL, NULL  },

	{ d_button_proc,   520,  10,  80,  18,   0,   0, 's', D_EXIT,   0,   0, "&Start Game", NULL, NULL  },
	{ d_button_proc,   520,  34,  80,  18,   0,   0, 'e', D_EXIT,   0,   0, "Scenario &Editor", NULL, NULL  },
	{ d_button_proc,   520,  58,  80,  18,   0,   0, 'x', D_EXIT,   0,   0, "E&xit", NULL, NULL  },

	{ NULL,              0,   0,   0,   0,   0,   0,   0,      0,   0,   0, NULL, NULL, NULL  }
};

int main(int argc, char **argv) {
	int ret;
	char strbuf[256];
	
	allegro_init();
	
	set_color_depth(24);
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0)) {
      set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Allegro says: %s\n", allegro_error);
		return 1;
	}

	install_timer();
	install_keyboard();
	install_mouse();
	if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL)==-1) printf("No sounds avaible.\n");
	graph_init();

	srand(time(0));

	set_keyboard_rate(100, 150);
	gui_bg_color = colors[gray];
	gui_fg_color = colors[black];

	LOCK_VARIABLE(counter);
	LOCK_FUNCTION(game_timer);
	install_int_ex(game_timer, BPS_TO_TIMER(6));
	
	if (argv[0][0]!='\0') {
		char *path;

		for (path=argv[0]; *path; path++) /* nothing */;
		if (path!=argv[0]) {
			for (;*path!='/' && path>=argv[0]; path--);
			path++;
			*path = '\0';
			chdir(argv[0]);
		}
	}
	
	if ((datafile = load_datafile("data.dat"))==NULL) {
      set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Datafile data.dat not found\n");
		return 1;
	}
	
	font = datafile[DATA_FONT].dat;
	gui_font_baseline = 1;

	main_dialog[0].bg = colors[darkgray];
	main_dialog[1].dp = datafile[DATA_SCREEN].dat;
	main_dialog[2].dp = datafile[DATA_LOGO].dat;

	for (ret=1; main_dialog[ret].proc; ret++) {
		main_dialog[ret].fg = colors[black];
		main_dialog[ret].bg = colors[gray];
	}
	
	while(1) {
		text_mode(-1);

		switch (do_dialog(main_dialog, -1)) {
		case DMAIN_START:
			ret = start_game();
		
			if (ret==GAME_ABORT) break;

			if (ret==GAME_END) {
				int i;
				ret=0;
				for (i=1; i<8; i++) if (players[i].points > players[ret].points) ret = i;
			}
			scare_mouse();
			clear_to_color(screen, colors[darkgray]);
			unscare_mouse();

			sprintf(strbuf, "The winner is %s!!!", players[ret].name);
			alert("GAME OVER", strbuf, NULL, "OK", NULL, 0, 0);

			report();
			break;
		case DMAIN_EDIT:
			start_editor();
			break;
		case DMAIN_EXIT:
			remove_sound();
			return 0;
		}
	}
	return 0;
}
END_OF_MAIN()

void foo() {}
