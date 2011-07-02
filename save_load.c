/*
 * Description: Load scenario routines and GUI
 * Version; 1.0
 * Author: Vincenzo Buttazzo
 * eMail: <selfesteem@virgilio.it>
 */

#include "save_load.h"

#define LOAD_LISTBOX 1
#define LOAD_OKAY 2
#define LOAD_CANCEL 3

#define SAVE_FRAME 0
#define SAVE_LISTBOX 2
#define SAVE_EDIT 4
#define SAVE_OKAY 6
#define SAVE_CANCEL 7

DIALOG load_dialog[] =
{
   /* (dialog proc)      (x)   (y)   (w)   (h)(fg)(bg)(key) (flags)(d1)(d2) (dp) (dp2) (dp3) */
	{ d_frame_proc,      170,  165,  300,  150,  0,  0,   0,      0,  0,  0,  "Load a Scenario or Game", NULL, NULL },
	{ d_list_proc,       175,  185,  205,  125,  0,  0,   0, D_EXIT,  0,  0,  file_list, NULL, NULL },
	{ d_button_proc,     385,  185,   80,   18,  0,  0, 'l', D_EXIT,  0,  0,  "&Load", NULL, NULL },
	{ d_button_proc,     385,  209,   80,   18,  0,  0, 'c', D_EXIT,  0,  0,  "&Cancel", NULL, NULL },
   { NULL,                0,    0,    0,    0,  0,  0,   0,      0,  0,  0,  NULL, NULL, NULL }
};

DIALOG save_dialog[] =
{
   /* (dialog proc)      (x)   (y)   (w)   (h)(fg)(bg)(key) (flags)(d1)(d2) (dp) (dp2) (dp3) */
	{ d_frame_proc,      170,  145,  300,  193,  0,  0,   0,      0,  0,  0,  NULL, NULL, NULL },
	{ d_text_proc,       175,  165,  200,   12,  0,  0,   0,      0,  0,  0,  "Select a file to override", NULL, NULL },
	{ d_list_proc,       175,  177,  205,  125,  0,  0,   0, D_EXIT,  0,  0,  file_list, NULL, NULL },
	{ d_text_proc,       175,  307,  200,   12,  0,  0,   0,      0,  0,  0,  "Or enter new file name", NULL, NULL },
	{ d_edit_proc,       175,  319,  170,   12,  0,  0,   0,      0, 11,  0,  filename, NULL, NULL },
	{ d_text_proc,       350,  319,   25,   12,  0,  0,   0,      0,  0,  0,  ext, NULL, NULL },
	{ d_button_proc,     385,  165,   80,   18,  0,  0, 'v', D_EXIT,  0,  0,  "Sa&ve", NULL, NULL },
	{ d_button_proc,     385,  189,   80,   18,  0,  0, 'c', D_EXIT,  0,  0,  "&Cancel", NULL, NULL },
   { NULL,                0,    0,    0,    0,  0,  0,   0,      0,  0,  0,  NULL, NULL, NULL }
};

/*
 * The list function required by an Allegro d_list_proc procedure
 */
char *file_list(int index, int *list_size) {
	if (index < 0) {
		*list_size = file_index;
		return NULL;
	}
	return files[index].name;
}

/*
 * Adds a filename into the files array. Required bu Allegro's for_each_file()
 */
void add_file(const char *filename, int attrib, int param) {
	strncpy(files[file_index].name, filename, 31);
	files[file_index].name[31] = '\0';
	file_index++;
}

int load_scenario(int kind) {
	int i;

   for (i=0; load_dialog[i].proc; i++) {
      load_dialog[i].fg = colors[black];
      load_dialog[i].bg = colors[gray];
   }
	load_dialog[LOAD_LISTBOX].bg = colors[white];

	file_index = for_each_file("*.sce", FA_ARCH, foo, 0);
	if (kind==SL_GAME) file_index += for_each_file("*.sav", FA_ARCH, foo, 0);
	files = (files_t *)malloc(sizeof(files_t)*file_index);
	file_index=0;
	for_each_file("*.sce", FA_ARCH, add_file, 0);
	if (kind==SL_GAME) for_each_file("*.sav", FA_ARCH, add_file, 0);

	i=1;
	if (do_dialog(load_dialog, -1)==LOAD_OKAY) {
		PACKFILE *file;
		unsigned short magic;
		
		file = pack_fopen(files[load_dialog[LOAD_LISTBOX].d1].name, "rb");
		if (file) {
			pack_fread(&game, sizeof(game_t), file);
			pack_fread(matrix, sizeof(tile_t)*game.tiles, file);
			pack_fread(players, sizeof(players_t)*8, file);
			pack_fread(&magic, sizeof(unsigned short), file);
			pack_fclose(file);

			if (magic==MAGIC) {
				printf("Game loaded.\n");
				i=0;
			}
		}
		if (i==1) alert("!ERROR", "Not a valid scenario file", NULL, "OK", NULL, 'o', 0);
	}
	
	free(files);
	return i;
}

int save_scenario(int kind) {
	int i;

	for (i=0; save_dialog[i].proc; i++) {
      save_dialog[i].fg = colors[black];
      save_dialog[i].bg = colors[gray];
   }
	save_dialog[SAVE_LISTBOX].bg = colors[white];
	save_dialog[SAVE_EDIT].bg = colors[white];
	
	if (kind) strcpy(ext, "*.sce"); else strcpy(ext, "*.sav");

	file_index = for_each_file(ext, FA_ARCH, foo, 0);
	files = (files_t *)malloc(sizeof(files_t)*file_index);
	file_index=0;
	for_each_file(ext, FA_ARCH, add_file, 0);
	
	filename[0]='\0';
	
	if (kind==SL_SCENARIO) {
		save_dialog[SAVE_FRAME].dp = "Save Scenario";
		strcpy(ext, ".sce");
	} else {
		save_dialog[SAVE_FRAME].dp = "Save Game";
		strcpy(ext, ".sav");
	}
	
	i=1;
	if (do_dialog(save_dialog, -1)==SAVE_OKAY) {
		PACKFILE *file;
		unsigned short magic = MAGIC;
		
		if (filename[0]=='\0') {
			file = pack_fopen(files[save_dialog[SAVE_LISTBOX].d1].name, "wb");
		} else {
			char fn[16];
			sprintf(fn, "%s%s", filename, ext);
			file = pack_fopen(fn, "wb");
		}	
		
		if (file) {
			pack_fwrite(&game, sizeof(game_t), file);
			pack_fwrite(matrix, sizeof(tile_t)*game.tiles, file);
			pack_fwrite(players, sizeof(players_t)*8, file);
			pack_fwrite(&magic, sizeof(unsigned short), file);
			pack_fclose(file);

			printf("Game saved.\n");
			i=0;
		}
		if (i==1) alert("!ERROR", "Could not save the scenario", NULL, "OK", NULL, 'o', 0);
	}

	free(files);
	return i;
}
