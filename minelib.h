#ifndef MINELIB_H
#define MINELIB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <Windows.h>

/* enum containing directions for easier reference */
typedef enum direction {
    NORTH,      // 0
    NORTHEAST,  // 1
    EAST,       // 2
    SOUTHEAST,  // 3
    SOUTH,      // 4
    SOUTHWEST,  // 5
    WEST,       // 6
    NORTHWEST   // 7
} direction_t;

/* enum containing difficulty settings for easier reference */
typedef enum difficulty {
    EASY,       // 0
    MEDIUM,     // 1
    HARD,       // 2
    CUSTOM      // 3
} difficulty_t;

/* structure containing a single board cell */
typedef struct cell { 

    bool             bRevealed;    // bool determining if the cell has been revealed
    bool              bHasMine;    // bool determining if there is a mine in the cell
    bool              bHasFlag;    // bool determining if the cell has been flagged
    unsigned int   MinesAround;    // number of mines around the cell

    unsigned int      RowIndex;    // row index of the cell on the game board
    unsigned int   ColumnIndex;    // column index of the cell on the game board

    struct cell **Surroundings;    // pointers to the 8 cells surrounding the cell

} cell_t;

/* structure containing the game board */
typedef struct board {

    unsigned int Height;    // height of the game board
    unsigned int  Width;    // width of the game board

    cell_t       *Cells;    // array containing the game board's cells

} board_t;

int generate_random_int (int Min, int Max);

void print_board (board_t *Board);

void print_new (board_t *Board);

void free_board (board_t *Board);

cell_t generate_cell (unsigned int RowIndex, unsigned int ColumnIndex);

void generate_connections (board_t *Board);

board_t *generate_board (unsigned int Height, unsigned int Width);

void fill_mines (board_t *Board, unsigned int MineCount, unsigned int StartingCellIndex);

void count_mines (board_t *Board);

int reveal (cell_t *Cell);

board_t *initialize_game (unsigned int BoardHeight, unsigned int BoardWidth, unsigned int StartingCellIndex, unsigned int MineCount);

#endif