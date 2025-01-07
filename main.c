#include "minelib.h"

int StopFlag = 0;

int main () {

    SetConsoleOutputCP(CP_UTF8);

    board_t *Board = initialize_game(15, 15, 10, 15);

    reveal(&Board->Cells[18]);
    
    print_board(Board);

    free_board (Board);

    return EXIT_SUCCESS;

}

