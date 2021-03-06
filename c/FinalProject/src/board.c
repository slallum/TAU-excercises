#include <stdlib.h>
#include "board.h"

/*
 * retuns an initialized board ith n rows and m columns
 */ 
Board *new_board(int n, int m) {
    Board *board = (Board*)malloc(sizeof(Board));
    if (board == NULL) {
        printf("Board cannot be initialized");
        return NULL;
    }
    int i;
    // allocate all the cells
    if ((board->cells = (int**)calloc(n, sizeof(int*))) == NULL) {
        printf("Board cannot be initialized");
        return NULL;
    }
    for (i=0; i<n; i++) {
        if ((board->cells[i] = (int*)calloc(m, sizeof(int))) == NULL) {
        	printf("Board cannot be initialized");
            return NULL;
        }
    }
    board->n = n;
    board->m = m;
    return board;
}


/* 
 * gets an source board and an initialized destination board
 * copies the source board to the destinatiojn board
 */
void copy_board(Board *source_board, Board *destination_board) {
    int i, j;
    for (i=0; i<source_board->n; i++) {
        for (j=0; j<source_board->m; j++) {
            destination_board->cells[i][j] = source_board->cells[i][j];
        }
    }
}

/**
 * If at least one cell in the board has no value in it,
 * return true - means there are more moves
 */
int board_full(Board* board) {
	int i, j;
	for (i = 0; i < board->n; i++) {
		for (j = 0; j < board->m; j++) {
			if (board->cells[i][j] == 0) {
				return 0;
			}
		}
	}
	return 1;
}

/**
 * Frees each and every dynamic memory allocated for the board
 */
void free_board(Board* board) {
	int i;
	for (i = 0; i < board->n; i++) {
		free(board->cells[i]);
	}
	free(board->cells);
	free(board);
}
