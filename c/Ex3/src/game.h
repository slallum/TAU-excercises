#ifndef GAME_H_
#define GAME_H_

#include "tree.h"
#include "board.h"

#define BOARD_HEIGHT 6
#define BOARD_WIDTH 7

#define DEPTH_NOT_SET -1

typedef struct game_s {
    board_t current_board;
    int is_comp_turn;
    minmax_tree *tree;
    int depth;
    int game_over;
} game;

int new_game(game *current_game, int depth);

/*
 * returns pointer to the column that the computer will add disc to
 * returns NULL if there was an error while executing some allocationg command
 */
int get_computer_move(game *current_game);
/*
 * gets the best column to insert a disc if the current player is the user
 */
int get_best_move_for_player(game *current_game);

int won_board(board_t board);
int make_connect4_move(int** cells, int n, int i, int value);

#endif /* GAME_H_ */
