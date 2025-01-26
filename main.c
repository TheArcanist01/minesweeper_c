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
            MineCount = MEDIUM;
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

    int StartingCellRowIndex = -1;
    int StartingCellColumnIndex = -1;
    while(StartingCellRowIndex < 0) {
            printf("Enter starting cell row:\n");
            if (!fgets(InputBuffer, 1024, stdin)) {
                continue;
            }
            StartingCellRowIndex = atoi(InputBuffer);
    }
    while(StartingCellColumnIndex < 0) {
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

    score (Board, Difficulty);
    ///////////////////////////////////////////////////////////////////////////
    char name[50];
    int tmp;

    while (HowManyToEnd(Board, Difficulty) < Board->Height * Board->Width - MineCount){
        int mode = 0;
        while (mode == 0){
            printf("Enter mode:\n");
            tmp = getch();
            if (tmp == 114){
                mode = 1;
            } else if (tmp == 102){
                mode = 2;
            } else {
                printf("[!] Error: Incorrect mode!\n");
            }
        }

        int Row = -1;
        int Column = -1;        
        while (Row < 0){
            printf("Enter cell row:\n");
            if (!fgets(InputBuffer, 1024, stdin)) {
                continue;
            }
            Row = atoi(InputBuffer);
        }
        while (Column < 0){
            printf("Enter cell column:\n");
            if (!fgets(InputBuffer, 1024, stdin)) {
                continue;
            }
            Column = atoi(InputBuffer);
        }

        if (mode == 1) {
            if (make_move(Board, Row, Column) == 1){
                printf("[!] This cell contains a bomb!\n");
                print_board(Board);
                score (Board, Difficulty);
                break;
            }
        } else if (mode == 2) {
            flag_mode(Board, Column, Row);
        }

        print_board(Board);
        score (Board, Difficulty);
    }

    if (HowManyToEnd(Board, Difficulty) == Board->Height * Board->Width - MineCount){
        Board->bWin = true;
    }

    printf("Enter your nick:\n");
    fgets(name, 50, stdin);

    write_to_file(Board, HowManyToEnd(Board, Difficulty));

    ///////////////////////////////////////////////////////////////////////////

    free_board(Board);

    return EXIT_SUCCESS;

}

