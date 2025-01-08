#include "minelib.h"

int main () {

    SetConsoleOutputCP(CP_UTF8);

    board_t *Board = initialize_game(6, 6, 10, 15);

    make_move(Board, 0, 0);
    make_move(Board, 0, 1);
    
    flag_cell(&Board->Cells[0]);

    print_board(Board);

    free_board (Board);

    return EXIT_SUCCESS;

}

