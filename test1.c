#include <stdio.h>
#include <stdlib.h>
#include "hclib.h" // Assuming this header contains the necessary definitions

void printBoard(char *stringboard) {
    printf("%s\n", stringboard);
}

int main(void) {
    // Initialize a new chessboard
    exboard_t *board = newboard();
    if (!board) {
        printf("Failed to initialize the board.\n");
        return 1;
    }

    // Move white knight from b1 to c3 to clear the castling path
    move_t move1;
    move1.from_i = 0; // b1 row index
    move1.from_j = 1; // b1 column index
    move1.to_i = 2;   // c3 row index
    move1.to_j = 2;   // c3 column index
    move1.promotion = ' ';

    exboard_t *afterFirstMoveBoard = apply_move(board, &move1);
    if (!afterFirstMoveBoard) {
        printf("Failed to apply the first move.\n");
        free(board);
        return 1;
    }

    // Move white bishop from c1 to d2 to clear the castling path
    move_t move2;
    move2.from_i = 0; // c1 row index
    move2.from_j = 2; // c1 column index
    move2.to_i = 1;   // d2 row index
    move2.to_j = 3;   // d2 column index
    move2.promotion = ' ';

    exboard_t *afterSecondMoveBoard = apply_move(afterFirstMoveBoard, &move2);
    if (!afterSecondMoveBoard) {
        printf("Failed to apply the second move.\n");
        free(afterFirstMoveBoard);
        free(board);
        return 1;
    }

    // Generate and print the board state before castling
    char *beforeCastlingStringBoard = stringboard(afterSecondMoveBoard);
    if (!beforeCastlingStringBoard) {
        printf("Failed to generate the stringboard before castling.\n");
        free(afterSecondMoveBoard);
        free(afterFirstMoveBoard);
        free(board);
        return 1;
    }

    printf("Board before castling:\n");
    printBoard(beforeCastlingStringBoard);

    // Perform the castling move: white king castles queen-side
    move_t move3;
    move3.from_i = 0; // e1 row index (king's original position)
    move3.from_j = 4; // e1 column index
    move3.to_i = 0;   // c1 row index (king's new position)
    move3.to_j = 2;   // c1 column index
    move3.promotion = ' '; // Not used for castling

    exboard_t *afterCastlingBoard = apply_move(afterSecondMoveBoard, &move3);
    if (!afterCastlingBoard) {
        printf("Failed to apply the castling move.\n");
        free(beforeCastlingStringBoard);
        free(afterSecondMoveBoard);
        free(afterFirstMoveBoard);
        free(board);
        return 1;
    }

    // Generate and print the board state after castling
    char *afterCastlingStringBoard = stringboard(afterCastlingBoard);
    if (!afterCastlingStringBoard) {
        printf("Failed to generate the stringboard after castling.\n");
        free(beforeCastlingStringBoard);
        free(afterCastlingBoard);
        free(afterSecondMoveBoard);
        free(afterFirstMoveBoard);
        free(board);
        return 1;
    }

    printf("Board after castling (White queen-side castle):\n");
    printBoard(afterCastlingStringBoard);

    // Free allocated memory
    free(beforeCastlingStringBoard);
    free(afterCastlingStringBoard);
    free(afterCastlingBoard);
    free(afterSecondMoveBoard);
    free(afterFirstMoveBoard);
    free(board);

    return 0;
}
