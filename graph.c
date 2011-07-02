#include "graph.h"

void graph_init() {
	colors[red] = makecol(255, 0, 0);
	colors[lightgray] = makecol(192, 192, 192);
	colors[green] = makecol(0, 255, 0);
	colors[cyan] = makecol(0, 255, 255);
	colors[yellow] = makecol(255, 255, 0);
	colors[blue] = makecol(0, 0, 255);
	colors[gray] = makecol(212, 208, 200);
	colors[white] = makecol(255, 255, 255);
	colors[black] = makecol(0, 0, 0);
	colors[darkgray] = makecol(128, 128, 128);
	
	buffer = create_bitmap(480, 480);
}

void rect3d(BITMAP *bmp, int x1, int y1, int x2, int y2, int col) {
	rectfill(bmp, x1, y1, x2, y2, col);
	line(bmp, x1, y1, x2, y1, colors[white]);
	line(bmp, x1, y1, x1, y2, colors[white]);
	line(bmp, x2, y2, x2, y1, colors[black]);
	line(bmp, x2, y2, x1, y2, colors[black]);
}

void text3d(BITMAP *bmp, int x, int y, char *s) {
	textout(bmp, datafile[DATA_BOLDFONT].dat, s, x+2, y+2, colors[black]);
	textout(bmp, datafile[DATA_BOLDFONT].dat, s, x, y, colors[white]);
}

int d_text3d_proc(int msg, DIALOG *d, int c) {
	if (msg==MSG_DRAW) {
		int rtm;
		FONT *oldfont = font;
		
		font = datafile[DATA_BOLDFONT].dat;
		
		rtm = text_mode(-1);
		gui_textout(screen, d->dp, d->x+1, d->y+1, colors[white], FALSE);
		gui_textout(screen, d->dp, d->x, d->y, colors[black], FALSE);
		text_mode(rtm);
		
		font = oldfont;
	}
	return D_O_K;
}

void draw_proc(DIALOG *d) {
	int a, b, i, j;

	j = game.pane_y*game.row_x;
	for (b=1; b<buffer->h; b+=24, j+=game.row_x) for (a=1, i=game.pane_x+j; a<buffer->h; a+=24, i++) {
		if (MATRIX_FOG(i, game.player)) {
			switch (MATRIX_TYPE(i)) {
				case MATRIX_WATER:
					rectfill(buffer, a, b, a+23, b+23, colors[blue]);
					break;
				case MATRIX_VOID:
					rect3d(buffer, a, b, a+23, b+23, colors[MATRIX_PLAYER(i)]);
					break;
				case MATRIX_ARMY:
					rect3d(buffer, a, b, a+23, b+23, colors[MATRIX_PLAYER(i)]);
					if (game.hide || MATRIX_HIDE(i)) masked_blit(datafile[DATA_ARMY].dat, buffer, 0, 0, a+1, b+1, 22, 22);
					break;
				case MATRIX_DEATH_VOID:
					rect3d(buffer, a, b, a+23, b+23, colors[MATRIX_PLAYER(i)]);
					rectfill(buffer, a+2, b+2, a+20, b+20, colors[black]);
					break;
				case MATRIX_DEATH_ARMY:
					rect3d(buffer, a, b, a+23, b+23, colors[MATRIX_PLAYER(i)]);
					masked_blit(datafile[DATA_DEATHARMY].dat, buffer, 0, 0, a+1, b+1, 22, 22);
					break;
				case MATRIX_CITY:
					rect3d(buffer, a, b, a+23, b+23, colors[MATRIX_PLAYER(i)]);
					masked_blit(datafile[DATA_CITY].dat, buffer, 0, 0, a+1, b+1, 22, 22);
					break;
			}
		} else {
			// Fog of War
			if (matrix[i].info & 0x80) {
				rect3d(buffer, a, b, a+23, b+23, colors[MATRIX_PLAYER(i)]);
				blit(datafile[DATA_CROSS].dat, buffer, 0, 0, a+1, b+1, 22, 22);
			} else rectfill(buffer, a, b, a+23, b+23, colors[black]);
		}
	}
	blit(buffer, screen, 0, 0, d->x, d->y, d->w, d->h);
}

DIALOG minimap_dialog[] = {
	/* (dialog proc)	(x)  (y)	(w)  (h) (fg) (bg)(key)  (flags) (d1) (d2) (dp) (dp2) (dp3) */
   { d_box_proc,    480,   0, 160, 480,   0,   0,   0,      0,   0,   0, NULL, NULL, NULL  },
	{ minimap_proc,    0,   0, 480, 480,   0,   0,   0,      0,   0,   0, NULL, NULL, NULL  },
   { d_button_proc, 520,  10,  80,  18,   0,   0, 'b', D_EXIT,   0,   0, "&Back", NULL, NULL  },
   { NULL,            0,   0,   0,   0,   0,   0,   0,      0,   0,   0, NULL, NULL, NULL  }	
};

int minimap_proc(int msg, DIALOG *d, int c) {
	if (msg==MSG_DRAW) {
		int x, y, i, sx, sy;
	
		scare_mouse_area(d->x, d->y, d->w, d->h);
		vsync();
		rectfill(screen, d->x, d->y, d->w-1, d->h-1, d->bg);

		sx = (d->w/2)+d->x-(game.row_x<<2);
		sy = (d->h/2)+d->y-(game.row_y<<2);

		rectfill(screen, sx-1, sy-1, sx-1+(game.row_x<<3), sy-1+(game.row_y<<3), d->fg);
		rect(screen, sx-1+(game.pane_x<<3), sy-1+(game.pane_y<<3), sx+7+((game.pane_x+19)<<3), sy+7+((game.pane_y+19)<<3), colors[white]);
		
		for (i=0, y=0; y<game.row_y; y++) for (x=0; x<game.row_x; x++, i++) {
			if (MATRIX_FOG(i, game.player)==0) continue;
			switch(MATRIX_TYPE(i)) {
				case MATRIX_WATER:
					rectfill(screen, sx+x*8, sy+y*8, sx+6+x*8, sy+6+y*8, colors[blue]);
					break;
				case MATRIX_VOID:
					rectfill(screen, sx+x*8, sy+y*8, sx+6+x*8, sy+6+y*8, colors[MATRIX_PLAYER(i)]);
					break;
				case MATRIX_DEATH_VOID:
					rectfill(screen, sx+1+x*8, sy+1+y*8, sx+5+x*8, sy+5+y*8, colors[black]);
					rect(screen, sx+x*8, sy+y*8, sx+6+x*8, sy+6+y*8, colors[MATRIX_PLAYER(i)]);
					break;
				case MATRIX_ARMY:
					rectfill(screen, sx+x*8, sy+y*8, sx+6+x*8, sy+6+y*8, colors[MATRIX_PLAYER(i)]);
					if (game.hide || MATRIX_HIDE(i)) rectfill(screen, sx+x*8+2, sy+y*8+2, sx+4+x*8, sy+4+y*8, d->fg);
					break;
				case MATRIX_DEATH_ARMY:
					rectfill(screen, sx+x*8, sy+y*8, sx+6+x*8, sy+6+y*8, colors[black]);
					if (MATRIX_HIDE(i)) {
						rectfill(screen, sx+x*8+2, sy+y*8+2, sx+4+x*8, sy+4+y*8, colors[MATRIX_PLAYER(i)]);
					} else {
						rect(screen, sx+x*8, sy+y*8, sx+6+x*8, sy+6+y*8, colors[MATRIX_PLAYER(i)]);
					}
					break;
				case MATRIX_CITY:
					rectfill(screen, sx+x*8, sy+y*8, sx+6+x*8, sy+6+y*8, colors[MATRIX_PLAYER(i)]);
					line(screen, sx+x*8, sy+y*8, sx+6+x*8, sy+6+y*8, d->fg);
					line(screen, sx+x*8, sy+6+y*8, sx+6+x*8, sy+y*8, d->fg);
					break;
			}
		}
		unscare_mouse();
	}
	return D_O_K;	
}

int start_minimap() {
	int ret;
	
	minimap_dialog[0].bg = colors[darkgray];
	minimap_dialog[1].bg = minimap_dialog[2].bg = colors[gray];
	for (ret=0; minimap_dialog[ret].proc; ret++) minimap_dialog[ret].fg = colors[black];
	
	return do_dialog(minimap_dialog, 2);
}

int d_frame_proc(int msg, DIALOG *d, int c) {
	if (msg==MSG_DRAW) {
		int fg = (d->flags & D_DISABLED) ? gui_mg_color : d->fg;

      rectfill(screen, d->x+1, d->y+1, d->x+d->w-3, d->y+d->h-3, d->bg);
      rect(screen, d->x, d->y, d->x+d->w-2, d->y+d->h-2, fg);
      vline(screen, d->x+d->w-1, d->y+1, d->y+d->h-1, colors[black]);
      hline(screen, d->x+1, d->y+d->h-1, d->x+d->w-1, colors[black]);
		
		rectfill(screen, d->x+1, d->y+1, d->x+d->w-3, d->y+16, colors[black]);
		if (d->dp) textout(screen, datafile[DATA_BOLDFONT].dat, d->dp, d->x+3, d->y+3, colors[white]);
	}
	return D_O_K;
}

int d_masked_proc(int msg, DIALOG *d, int c) {
   BITMAP *b = (BITMAP *)d->dp;

   if (msg==MSG_DRAW)
      masked_blit(b, screen, 0, 0, d->x, d->y, d->w, d->h);

   return D_O_K;
}
