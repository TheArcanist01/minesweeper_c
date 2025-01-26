#include "minelib.h"

int generate_random_int (int Min, int Max) {

    int RandomInt = rand();
    int RandomRangedInt = RandomInt % (Max - Min + 1) + Min;
    
    return RandomRangedInt;
}

unsigned int get_cell_index (unsigned int RowIndex, unsigned int ColumnIndex, board_t *Board) {
    
    if (Board == NULL) {
        fprintf(stderr, "[!] Error: Can't access game board!\n");
        return 1;
    }

    if (RowIndex >= Board->Height || ColumnIndex >= Board->Width) {
        fprintf(stderr, "[!] Error: Index outside of game board!\n");
        return 1;
    }

    return RowIndex * Board->Width + ColumnIndex;
}

void print_board (board_t *Board) {

    if (Board == NULL) {
        fprintf(stderr, "[!] Error: Can't access game board!\n");
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
                printf("| <I ");
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

void free_board (board_t *Board) {

    if (Board == NULL) {
        fprintf(stderr, "[!] Error: Can't access game board!\n");
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

    if (Board == NULL) {
        fprintf(stderr, "[!] Error: Can't access game board!\n");
        return;
    }

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
    Board->bWin = false;

    Board->Cells = malloc(Height * Width * sizeof(cell_t));
    for (int i = 0; i < Height * Width; i++) {
        Board->Cells[i] = generate_cell(i / Width, i % Width);
    }

    generate_connections(Board);

    return Board;

}

void fill_mines (board_t *Board, unsigned int MineCount, unsigned int StartingCellIndex) {

    if (Board == NULL) {
        fprintf(stderr, "[!] Error: Can't access game board!\n");
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

int reveal (cell_t *Cell, board_t *Board) {

    if (Cell == NULL) {
        fprintf(stderr, "[!] Error: Can't access cell!\n");
        return 0;
    }

    if (Cell->bRevealed == true) {
        return 0;
    }

    Cell->bRevealed = true;
    Board->RevealedCells += 1;

    if (Cell->bHasMine == true) {
        return 1;
    }

    if (Cell->MinesAround == 0) {
        for (int i = 0; i < 8; i++) {
            if (Cell->Surroundings[i] != NULL) {
                reveal(Cell->Surroundings[i], Board);
            }
        }
    }

    return 0;
}

void flag_cell (cell_t *Cell) {

    if (Cell == NULL) {
        fprintf(stderr, "[!] Error: Can't access cell!\n");
        return;
    }

    if (Cell->bRevealed == true) {
        fprintf(stderr, "[!] Error: Can't flag revealed cell!\n");
        return;
    }

    if (Cell->bHasFlag == false) {
        Cell->bHasFlag = true;
    } else {
        Cell->bHasFlag = false;
    }
}

int make_move (board_t *Board, unsigned int RowIndex, unsigned int ColumnIndex) {

    if (Board == NULL) {
        fprintf(stderr, "[!] Error: Can't access game board!\n");
        return 2;
    }

    int Temp = 0;
    Temp += reveal(&Board->Cells[get_cell_index(RowIndex, ColumnIndex, Board)], Board);

    return Temp;
}

board_t *initialize_game (unsigned int BoardHeight, unsigned int BoardWidth, unsigned int StartingCellIndex, unsigned int MineCount) {

    if (BoardWidth == 0 && BoardHeight == 0 && MineCount == 0) {
        fprintf(stderr, "[!] Error: Board dimensions must be greater than 0!\n");
        return NULL;
    }

    if (StartingCellIndex >= BoardHeight * BoardWidth) {
        fprintf(stderr, "[!] Error: Starting cell index outside of game board!\n");
        return NULL;
    }

    if (MineCount >= BoardHeight * BoardWidth - 1) {
        fprintf(stderr, "[!] Error: Mine count larger than board size!");
        return NULL;
    }

    board_t *Board = generate_board(BoardHeight, BoardWidth);

    fill_mines(Board, MineCount, StartingCellIndex);

    count_mines(Board);

    reveal(&Board->Cells[StartingCellIndex], Board);

    Board->RevealedCells = 1;

    return Board;
}

///////////////////////////////////////////////////////////////////////////
int score (board_t *Board, int level){
    int score = 0;
    for (int i=0; i<Board->Height*Board->Width; i++){
        if (Board->Cells[i].bRevealed){
            score++;
        }
    }
    score *= (level-47);
    printf("Your current score: %u\n", score);
    return score;
}

int HowManyToEnd (board_t *Board, int level){
    int sum = 0;
    for (int i=0; i<Board->Height*Board->Width; i++){
        if (Board->Cells[i].bRevealed){
            sum++;
        }
    }
    return sum;
}

void flag_mode (board_t *Board, unsigned int ColumnIndex, unsigned int RowIndex){
    flag_cell(&Board->Cells[get_cell_index(RowIndex, ColumnIndex, Board)]);
}

void write_to_file (board_t* Board, int points){ // DO DOKONCZENIA
    char FileName[100];
    printf("Enter name of the file:\n");
    scanf("%s", FileName);
    FILE* file = fopen(FileName, "w");
    if (file == NULL){
        printf("[!] Error: Can't access file!\n");
    }
    fprintf(file, "%d %d", Board->bWin, points);
    fclose(file);
}
