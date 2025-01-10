#include "minelib.h"

int main () {

    SetConsoleOutputCP(CP_UTF8);
    char InputBuffer[1024];

    printf("Select difficulty:\n0 - EASY, 1 - NORMAL, 2 - HARD, 3 - CUSTOM\n");
    int Difficulty = 0;
    while (Difficulty != 48 && Difficulty != 49 && Difficulty != 50 && Difficulty != 51) {
        Difficulty = getch();
    }

    unsigned int BoardWidth = 0;
    unsigned int BoardHeight = 0;
    unsigned int MineCount = 0;
    unsigned int StartingCell = 0;

    switch (Difficulty) {
        case 48:
            BoardWidth = 9;
            BoardHeight = 9;
            MineCount = EASY;
            break;
        case 49:
            BoardWidth = 16;
            BoardHeight = 16;
            MineCount = NORMAL;
            break;
        case 50:
            BoardWidth = 30;
            BoardHeight = 16;
            MineCount = HARD;
            break;
        case 51:
            while(BoardHeight == 0) {
                printf("Enter board height:\n");
                if (!fgets(InputBuffer, 1024, stdin)) {
                    continue;
                }
                BoardHeight = atoi(InputBuffer);
            }
            while(BoardWidth == 0) {
                printf("Enter board width:\n");
                if (!fgets(InputBuffer, 1024, stdin)) {
                    continue;
                }
                BoardWidth = atoi(InputBuffer);
            }
            while(MineCount == 0) {
                printf("Enter mine count:\n");
                if (!fgets(InputBuffer, 1024, stdin)) {
                    continue;
                }
                MineCount = atoi(InputBuffer);
            }
            break;
    }

    unsigned int StartingCellRowIndex = 0;
    unsigned int StartingCellColumnIndex = 0;
    while(StartingCellRowIndex == 0) {
            printf("Enter starting cell row:\n");
            if (!fgets(InputBuffer, 1024, stdin)) {
                continue;
            }
            StartingCellRowIndex = atoi(InputBuffer);
    }
    while(StartingCellColumnIndex == 0) {
            printf("Enter starting cell column:\n");
            if (!fgets(InputBuffer, 1024, stdin)) {
                continue;
            }
            StartingCellColumnIndex = atoi(InputBuffer);
    }

    unsigned int StartingCellIndex = StartingCellRowIndex * BoardWidth + StartingCellColumnIndex;

    board_t *Board = initialize_game(BoardHeight, BoardWidth, StartingCellIndex, MineCount);

    /* make_move(Board, 0, 0);
    make_move(Board, 0, 1);
    
    flag_cell(&Board->Cells[0]);
    */ 
    print_board(Board);

    free_board(Board);

    return EXIT_SUCCESS;

}

