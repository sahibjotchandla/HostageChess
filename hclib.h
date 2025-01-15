#ifndef HCLIB_H
#define HCLIB_H
#define MAX_PRISON_SIZE 16
#define MAX_AIRFIELD_SIZE 16

#include <stdlib.h>

// Defining type of board
typedef char board_t[8][8];
/* row - rank; 0-7 corresponods to 1-8
 * column - file; 0-7 corresponds to a-h
 */

// Defining structure of extended board
typedef struct
{
char bprison[16];   // Black pieces in prison
char bairfield[16]; // Black pieces in airfield

board_t board;      // 8x8 board

char wprison[16];   // White pieces in prison
char wairfield[16]; // White pieces in airfield
} exboard_t;

// Defining structure for moves 
typedef struct
{
int from_i, from_j; // From (row, column)
int to_i, to_j;     // To (row, column)
char promotion;     // Pawn promotion
char hostage;       // Pieces in hostage
} move_t;

// Defining prototypes of board
exboard_t *newboard(void);
exboard_t *copyboard(exboard_t *board);
char *stringboard(exboard_t *board);
exboard_t *apply_move(exboard_t *board, move_t *move);
move_t **knightmoves(exboard_t *board, int from_i, int from_j, int colour);
move_t **bishopmoves(exboard_t *board, int from_i, int from_j, int colour);
move_t **rookmoves(exboard_t *board, int from_i, int from_j, int colour);
move_t **queenmoves(exboard_t *board, int from_i, int from_j, int colour);
move_t **kingmoves(exboard_t *board, int from_i, int from_j, int colour);
move_t **pawnmoves(exboard_t *board, int from_i, int from_j, int colour);


#endif
