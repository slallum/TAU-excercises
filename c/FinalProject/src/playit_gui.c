/*
 * playit_gui.c
 *
 *  Created on: Mar 1, 2014
 *      Author: shir
 */

#include "playit_gui.h"
#include "game_framework.h"

char* current_tiles[3] = { "" };

Link* create_button_menu(int height, int width, int num, char* pics[],
		int (*handles[])(Control*), int widths[], int heights[]) {

	int i, total_height = 0;
	int spacing, current = MARGIN_H;
	Link *head = (Link*) calloc(1, sizeof(Link));
	Link *buttons_next = head;

	for (i = 0; i < num; i++) {
		total_height += heights[i];
	}
	spacing = ((height - current * 2 - total_height) / num) / 2;
	for (i = 0; i < num - 1; i++) {
		buttons_next->value = create_button((width - widths[i]) / 2,
				current + spacing, i, 0, widths[i], heights[i], pics[i],
				handles[i]);
		if (buttons_next->value == NULL ) {
			return 0;
		}
		current += heights[i] + spacing;
		buttons_next->next = (Link*) calloc(1, sizeof(Link));
		buttons_next = buttons_next->next;
	}
	// Adding last link in list
	buttons_next->value = create_button((width - widths[i]) / 2,
			current + spacing, i, 0, widths[i], heights[i], pics[i],
			handles[i]);
	if (buttons_next->value == NULL ) {
		return 0;
	}
	buttons_next->next = NULL;
	return head;
}

int create_button_menu_window(int num, char* pics[], int (*handles[])(Control*),
		int widths[], int heights[], Control* window) {
	clear_window(window);
	window->children_head = (Link*) calloc(1, sizeof(Link));
	window->children_head->value = create_fs_panel(BG_IMG,
			create_button_menu(window->height, window->width, num, pics,
					handles, widths, heights));
	window->children_head->value->parent = window;
	window->children_head->next = NULL;
	return draw(window);
}

int show_main_menu(Control* window, int (*handles[])(Control*)) {
	char* pics[4] = { PLAYIT_IMG, NEW_GAME_IMG, LOAD_GAME_IMG, QUIT_IMG };
	int widths[4] = { LOGO_DIM, BUT_W, BUT_W, BUT_W };
	int heights[4] = { LOGO_DIM, BUT_H, BUT_H, BUT_H };
	return create_button_menu_window(4, pics, handles, widths, heights, window);
}

int show_game_menu(Control* window, int (*handles[])(Control*)) {
	char* pics[NUM_GAMES + 2] = { SELECT_G_IMG, TIC_TAC_TOE_IMG, CONNECT4_IMG,
			REVERSI_IMG, CANCEL_IMG };
	int widths[NUM_GAMES + 2] = { TITLE_W, BUT_W, BUT_W, BUT_W, BUT_W };
	int heights[NUM_GAMES + 2] = { TITLE_H, BUT_H, BUT_H, BUT_H, BUT_H };
	return create_button_menu_window(NUM_GAMES + 2, pics, handles, widths,
			heights, window);
}

int show_player_select(Control* window, int (*empty)(Control*),
		int (*player)(Control*), int (*cancel)(Control*)) {
	int i;
	char* pics[NUM_PLAY_TYPES + 2] = { SELECT_P_IMG, P_VS_P_IMG, P_VS_A_IMG,
			A_VS_P_IMG, A_VS_A_IMG, CANCEL_IMG };
	int widths[NUM_PLAY_TYPES + 2] = { TITLE_W, BUT_W, BUT_W, BUT_W, BUT_W,
			BUT_W };
	int heights[NUM_PLAY_TYPES + 2] = { TITLE_H, BUT_H, BUT_H, BUT_H, BUT_H,
			BUT_H };
	int (*handles[NUM_PLAY_TYPES + 2])(Control*) = {empty };
	for (i = 1; i <= NUM_PLAY_TYPES; i++) {
		handles[i] = player;
	}
	handles[i] = cancel;
	return create_button_menu_window(NUM_PLAY_TYPES + 2, pics, handles, widths,
			heights, window);
}

int show_game_arena(Control* window, Game *game, int (*handle)(Control*),
		int (*menu_handles[])(Control*)) {

	Link *buttons_next;
	clear_window(window);
	window->children_head = (Link*) calloc(1, sizeof(Link));
	buttons_next = window->children_head;

	buttons_next->value = create_board(BOARD_PANEL_W, window->height,
			game->board->n, game->board->m, game->board->cells, handle);
	buttons_next->value->parent = window;
	buttons_next->next = (Link*) calloc(1, sizeof(Link));
	buttons_next = buttons_next->next;
	buttons_next->value = create_game_panel(window->height,
			window->width - (BOARD_PANEL_W), menu_handles);
	buttons_next->value->parent = window;
	buttons_next->next = NULL;

	return draw(window);
}

Control* create_game_panel(int height, int width, int (*menu_handles[])(Control*)) {
	char* pics[4] = { RESTART_GAME_IMG, SAVE_GAME_IMG, MAIN_MENU_IMG, QUIT_IMG };
	int widths[4] = { BUT_W, BUT_W, BUT_W, BUT_W };
	int heights[4] = { BUT_H, BUT_H, BUT_H, BUT_H };
	return create_panel(WIN_W - width, WIN_H - height, 0, 1, width, height,BG_IMG,
			create_button_menu(height, width, 4, pics, menu_handles, widths, heights));
}

Control* create_board(int width, int height, int n, int m, int** cells,
		int (*handle)(Control*)) {

	int i, j, current_x, current_y;
	Link *head = (Link*) calloc(1, sizeof(Link));
	Link *buttons_next = head;

	current_y = (height - (n * TILE_H) - ((n - 1) * TILE_SPACE)) / 2;
	current_x = (width - (m * TILE_W) - ((m - 1) * TILE_SPACE)) / 2;
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			buttons_next->value = create_button(current_x, current_y, i, j,
					TILE_W, TILE_H, current_tiles[cells[i][j] % 3], handle);
			current_x += TILE_W + TILE_SPACE;
			if ((i == n - 1) && (j == m - 1)) {
				buttons_next->next = NULL;
			} else {
				buttons_next->next = (Link*) calloc(1, sizeof(Link));
				buttons_next = buttons_next->next;
			}
		}
		current_y += TILE_H + TILE_SPACE;
		current_x = (width - (m * TILE_W) - ((m - 1) * TILE_SPACE)) / 2;
	}
	return create_panel(0, 0, 0, 0, width, height, BG_IMG, head);
}

void set_reversi_tiles() {
	current_tiles[0] = RN_IMG;
	current_tiles[1] = RW_IMG;
	current_tiles[2] = RB_IMG;
}

void set_tictactoe_tiles() {
	current_tiles[0] = TTTN_IMG;
	current_tiles[1] = TTTX_IMG;
	current_tiles[2] = TTTO_IMG;
}

void set_connect4_tiles() {
	current_tiles[0] = C4N_IMG;
	current_tiles[1] = C4P1_IMG;
	current_tiles[2] = C4P2_IMG;
}

int show_files_menu(Control* window, int (*empty)(Control*),
		int (*file_han)(Control*), int (*cancel)(Control*)) {

	int i;
	char* pics[FILES_NUM + 2] = { FILE_SELECT };
	int widths[FILES_NUM + 2] = { TITLE_W };
	int heights[FILES_NUM + 2] = { TITLE_H };
	int (*handles[FILES_NUM + 2])(Control*) = {empty };
	// According to number of files,
	// add names, sizes and functions to structs.
	// Giving a name to a button, as oppose to a valid path
	// will create it on the fly using text
	for (i = 1; i <= FILES_NUM; i++) {
		pics[i] = (char*) malloc(strlen(FILE_NAME) + sizeof(char) * 2 + 1);
		sprintf(pics[i], "%s %d", FILE_NAME, i);
		widths[i] = BUT_W;
		heights[i] = BUT_H;
		handles[i] = file_han;
	}
	pics[i] = CANCEL_IMG;
	widths[i] = BUT_W;
	heights[i] = BUT_H;
	handles[i] = cancel;
	return create_button_menu_window(FILES_NUM + 2, pics, handles, widths,
			heights, window);
}

void clear_window(Control* window) {
	if (window->children_head != NULL ) {
		free_UI_tree(window->children_head->value);
		free(window->children_head);
	}
}
