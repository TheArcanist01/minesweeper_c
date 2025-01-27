#include "minelib.h"

int main () {

    SetConsoleOutputCP(CP_UTF8);
    srand(time(NULL);
    char InputBuffer[1024];

    printf("Press 0 to play game, press 1 to load from file\n");
    int FileCheck = 2;
    while (FileCheck != 48 && FileCheck != 49) {
        FileCheck = getch();
    }

    if (FileCheck == 49) {
        printf("Enter file name:\n");
        scanf("%s", InputBuffer);
        FILE *Stream = fopen(InputBuffer, "r");

        game_from_file(Stream);
        return EXIT_SUCCESS;
    }

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
    system("cls");
    print_board(Board);

    int CurrentScore = score (Board, Difficulty);
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
                system("cls");
                printf("Game over!\n");
                CurrentScore = score (Board, Difficulty);
                int EndScore = HowManyToEnd(Board, Difficulty)*(Difficulty-47);
                printf("Your current score: %u\n", CurrentScore);
                for (int i = 0; i < Board->Height * Board->Width; i++) {
                    if (Board->Cells[i].bRevealed == false) {
                        Board->Cells[i].bRevealed = true;
                    }
                }
                print_board(Board);
                best_results(EndScore);
                break;
            }
        } else if (mode == 2) {
            flag_mode(Board, Column, Row);
        }

        system("cls");
        print_board(Board);
        CurrentScore = score (Board, Difficulty);
        printf("Your current score: %u\n", CurrentScore);
    }

    if (HowManyToEnd(Board, Difficulty) == Board->Height * Board->Width - MineCount){
        Board->bWin = true;
        best_results(HowManyToEnd(Board, Difficulty)*(Difficulty-47));
    }

    ///////////////////////////////////////////////////////////////////////////

    free_board(Board);

    return EXIT_SUCCESS;

}

