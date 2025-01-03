#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <Windows.h>

int StopFlag = 0;

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

int generate_random_int (int Min, int Max) {

    int RandomInt = rand();
    int RandomRangedInt = RandomInt % (Max - Min + 1) + Min;
    
    return RandomRangedInt;
}

void print_board (board_t *Board) {

    if (Board == NULL) {
        fprintf(stderr, "[!] Error: can't access game board\n");
        return;
    }

    int CurrentCell = 0;

    for (int i = 0; i < Board->Width; i++) {
        printf("+ ―― ");
    }
    printf("+\n");
    for (int j = 0; j < Board->Height; j++) {
        for (int k = 0; k < Board->Width; k++) {
            if (Board->Cells[CurrentCell].bHasFlag == true) {
                printf("| <> ");
            } else if (Board->Cells[CurrentCell].bRevealed == false) {
                printf("| [] ");
            } else {
                if (Board->Cells[CurrentCell].bHasMine == false) {
                    if (Board->Cells[CurrentCell].MinesAround == 0) {
                        printf("|    ");
                    } else {
                        printf("|  %d ", Board->Cells[CurrentCell].MinesAround);
                    }
                } else {
                    printf("| () ");
                }
            }
            CurrentCell++;
        }
        printf("|\n");
        for (int l = 0; l < Board->Width; l++) {
            printf("+ ―― ");
        }
        printf("+\n");
    }

}

void fill_mines (board_t *Board, unsigned int MineCount, unsigned int StartingCellIndex) {

    if (Board == NULL) {
        fprintf(stderr, "[!] Error: Can't access game board!\n");
        return;
    }

    if (MineCount >= Board->Height * Board->Width - 1) {
        fprintf(stderr, "[!] Error: Mine count larger than board size!\n");
        return;
    }

    if (StartingCellIndex >= Board->Height * Board->Width) {
        fprintf(stderr, "[!] Error: Starting cell index outside of game board!\n");
        return;
    }

    int i = 0;
    while (i < MineCount) {
        int RandomCellIndex = generate_random_int(0, (Board->Height * Board->Width) - 1);
        if (Board->Cells[RandomCellIndex].bHasMine == false && RandomCellIndex != StartingCellIndex) {
            Board->Cells[RandomCellIndex].bHasMine = true;
            i++;
        }
    }
}

void free_board (board_t *Board) {

    if (Board == NULL) {
        fprintf(stderr, "[!] Error: can't access game board\n");
        return;
    }

    for (int i = 0; i < Board->Width * Board->Height; i++) {
        free(Board->Cells[i].Surroundings);
    }
    free(Board->Cells);
    free(Board);
} 

cell_t generate_cell (unsigned int RowIndex, unsigned int ColumnIndex) {

    cell_t Cell;
    
    Cell.RowIndex = RowIndex;
    Cell.ColumnIndex = ColumnIndex;

    Cell.Surroundings = malloc (8 * sizeof (cell_t *));
    for (int i = 0; i < 8; i++) {
        Cell.Surroundings[i] = NULL;
    }

    Cell.bHasFlag = false;
    Cell.bHasMine = false;
    Cell.bRevealed = false;
    Cell.MinesAround = 0;

    return Cell;
}

void generate_connections (board_t *Board) {
    for (int i = 0; i < Board->Height * Board->Width; i++) {
        if (Board->Cells[i].RowIndex != 0) {
            Board->Cells[i].Surroundings[NORTH] = &Board->Cells[i - Board->Width];
            if (Board->Cells[i].ColumnIndex != Board->Width - 1) {
                Board->Cells[i].Surroundings[NORTHEAST] = &Board->Cells[(i - Board->Width) + 1];
            }
            if (Board->Cells[i].ColumnIndex != 0) {
                Board->Cells[i].Surroundings[NORTHWEST] = &Board->Cells[(i - Board->Width) - 1];
            }
        }
        if (Board->Cells[i].RowIndex != Board->Height - 1) {
            Board->Cells[i].Surroundings[SOUTH] = &Board->Cells[i + Board->Width];
            if (Board->Cells[i].ColumnIndex != Board->Width - 1) {
                Board->Cells[i].Surroundings[SOUTHEAST] = &Board->Cells[i + Board->Width + 1];
            }
            if (Board->Cells[i].ColumnIndex != 0) {
                Board->Cells[i].Surroundings[SOUTHWEST] = &Board->Cells[(i + Board->Width) - 1];
            }
        }
        if (Board->Cells[i].ColumnIndex != Board->Width - 1) {
            Board->Cells[i].Surroundings[EAST] = &Board->Cells[i + 1];
        }
        if (Board->Cells[i].ColumnIndex != 0) {
            Board->Cells[i].Surroundings[WEST] = &Board->Cells[i - 1];
        }
    }
}

board_t *generate_board (unsigned int Height, unsigned int Width) {

    board_t *Board = malloc(sizeof(board_t));

    if (Board == NULL) {
        fprintf (stderr, "[!] Error: memory allocation for game board failed\n");
        return NULL;
    }

    Board->Height = Height;
    Board->Width = Width;

    Board->Cells = malloc(Height * Width * sizeof(cell_t));
    for (int i = 0; i < Height * Width; i++) {
        Board->Cells[i] = generate_cell(i / Width, i % Width);
    }

    generate_connections(Board);

    return Board;

}

void count_mines (board_t *Board) {

    if (Board == NULL) {
        fprintf(stderr, "[!] Error: Can't access game board!\n");
        return;
    }

    for (int i = 0; i < Board->Height * Board->Width; i++) {
        if (Board->Cells[i].bHasMine == true) {
            continue;
        } else {
            for (int j = 0; j < 8; j++) {
                if (Board->Cells[i].Surroundings[j] != NULL) {
                    if (Board->Cells[i].Surroundings[j]->bHasMine == true) {
                        Board->Cells[i].MinesAround++;
                    }
                }
            }
        }
    }
}

void reveal (cell_t *Cell) {

    if (Cell == NULL) {
        fprintf(stderr, "[!] Error: Can't access cell!\n");
        return;
    }

    if (Cell->bRevealed == true) {
        return;
    }

    Cell->bRevealed = true;

    if (Cell->bHasMine == true) {
        StopFlag = 1;
        return;
    }

    if (Cell->MinesAround == 0) {
        for (int i = 0; i < 8; i++) {
            if (Cell->Surroundings[i] != NULL) {
                reveal(Cell->Surroundings[i]);
            }
        }
    }


}

int main () {

    SetConsoleOutputCP(CP_UTF8);

    board_t *Board = generate_board (6, 6);

    fill_mines(Board, 15, 7);
    count_mines(Board);

    reveal(&Board->Cells[1]);

    print_board(Board);

    free_board (Board);

    return EXIT_SUCCESS;

}

