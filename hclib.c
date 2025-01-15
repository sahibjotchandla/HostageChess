#include "hclib.h"

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <ctype.h>              

#define MAX_PRISON_SIZE 16      // Defining prison size
#define MAX_AIRFIELD_SIZE 16    // Defining airfield size

// Function prototypes to move pieces
move_t **knightmoves(exboard_t *board, int from_i, int from_j, int colour);
move_t **bishopmoves(exboard_t *board, int from_i, int from_j, int colour);
move_t **rookmoves(exboard_t *board, int from_i, int from_j, int colour);
move_t **queenmoves(exboard_t *board, int from_i, int from_j, int colour);
move_t **kingmoves(exboard_t *board, int from_i, int from_j, int colour);
move_t **pawnmoves(exboard_t *board, int from_i, int from_j, int colour);

// PART I
// Helper functions

// Initializes the board 
void initializeBoard(board_t board) 
{
    // Default position
    char defaultBoard[8][8] = 
    {
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},  // White pieces  (rank 1) (row 0)
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},  // White pawns   (rank 2) (row 1)
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},  // Empty squares (rank 3) (row 2)
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},  // Empty squares (rank 4) (row 3)
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},  // Empty squares (rank 5) (row 4)
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},  // Empty squares (rank 6) (row 5)
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},  // Black pawns   (rank 7) (row 6)
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}   // Black pieces  (rank 8) (row 7)
    };

    // Copy positions to board array
    memcpy(board, defaultBoard, sizeof(board_t));
}

// Initializes prison and airfield strings to default (empty strings)
void initPrisonAirfields(exboard_t *board) 
{
    // Not all spaces were completely defined (was having weird characters being displayed on prison and airfields), therefore, used memset (idea found from https://stackoverflow.com/questions/1536006/what-is-the-difference-between-memset-and-memcpy-in-c and w3schools)
    memset(board->bprison, '\0', sizeof(board->bprison));       // Black Prison
    memset(board->bairfield, '\0', sizeof(board->bairfield));   // Black Airfield
    memset(board->wprison, '\0', sizeof(board->wprison));       // White Prison
    memset(board->wairfield, '\0', sizeof(board->wairfield));   // White Airfield
}

// Copies one exboard_t to another board.
void copyExboard(exboard_t *dest, const exboard_t *src) 
{
    memcpy(dest, src, sizeof(exboard_t));
}

// NEWBOARD
// Function makes a new chess board and returns a pointer
exboard_t *newboard(void) 
{
    // Allocating memory
    exboard_t *board = (exboard_t *)malloc(sizeof(exboard_t));
    if (board == NULL) 
    {
        return NULL;  // NULL if malloc fails
    }

    // Initialize board with starting positions
    initializeBoard(board->board);

    // Initialize empty strings for prisons and airfields
    initPrisonAirfields(board);

    // Return pointer to new board
    return board;
}

// COPYBOARD
// Function copies existing board and returns pointer to new copy
exboard_t *copyboard(exboard_t *board) 
{
    // Allocating memory for new board
    exboard_t *newCopy = (exboard_t *)malloc(sizeof(exboard_t));
    if (newCopy == NULL) 
    {
        return NULL;  // NULL if malloc fails
    }

    // Copy data of original board onto new board
    copyExboard(newCopy, board);

    // Return pointer to new board
    return newCopy;
}

// STRINGBOARD
char *stringboard(exboard_t *board) 
{
    // Allocate enough memory for the entire stringboard
    char *str = (char *)malloc(162 * sizeof(char));
    if (str == NULL) 
    {
        return NULL;  // Return NULL if malloc fails
    }

    int index = 0;

    // Copying bprison (16 characters)
    for (int i = 0; i < 16; i++) 
    {
        str[index++] = (board->bprison[i] != '\0') ? board->bprison[i] : ' ';
        if ((i + 1) % 8 == 0) 
        {
            str[index++] = '\n';
        }
    }

    // Copying bairfield (16 characters)
    for (int i = 0; i < 16; i++) 
    {
        str[index++] = (board->bairfield[i] != '\0') ? board->bairfield[i] : ' ';
        if ((i + 1) % 8 == 0) 
        {
            str[index++] = '\n';
        }
    }

    // Adding dashes
    for (int i = 0; i < 8; i++) 
    {
        str[index++] = '-';
    }
    str[index++] = '\n';

    // Copying the board rows from top (row 7) to bottom (row 0)
    for (int i = 7; i >= 0; i--) 
    {
        for (int j = 0; j < 8; j++) 
        {
            str[index++] = board->board[i][j];
        }
        str[index++] = '\n';
    }

    // Adding dashes again
    for (int i = 0; i < 8; i++) 
    {
        str[index++] = '-';
    }
    str[index++] = '\n';

    // Copying wairfield (16 characters)
    for (int i = 0; i < 16; i++) 
    {
        str[index++] = (board->wairfield[i] != '\0') ? board->wairfield[i] : ' ';
        if ((i + 1) % 8 == 0) 
        {
            str[index++] = '\n';
        }
    }

    // Copying wprison (16 characters)
    for (int i = 0; i < 16; i++) 
    {
        str[index++] = (board->wprison[i] != '\0') ? board->wprison[i] : ' ';
        if ((i + 1) % 8 == 0 && i != 15) 
        {
            str[index++] = '\n';
        }
    }

    // Null terminator
    str[index] = '\0';

    return str;
}

// Apply Move
exboard_t *apply_move(exboard_t *board, move_t *move) 
{
    // Copy board
    exboard_t *newboard = copyboard(board);
    if (newboard == NULL) 
    {
        return NULL;  // Return NULL if malloc fails
    }

    char piece;
    char capturedPiece = newboard->board[move->to_i][move->to_j];

    // Determine if move is a drop from airfield or prison
    int isDrop = (move->from_i < 0 || move->from_i > 7);

    if (!isDrop) 
    {
        // Piece being moved
        piece = newboard->board[move->from_i][move->from_j];
    }
    else 
    {
        // For drops, the piece is specified in promotion
        piece = move->promotion;
    }

    // Castling logic
    if (!isDrop && (piece == 'K' || piece == 'k') && abs(move->to_j - move->from_j) > 1) 
    {
        // King-Side Castling
        if (move->to_j > move->from_j) 
        {
            newboard->board[move->from_i][move->to_j - 1] = newboard->board[move->from_i][7];
            newboard->board[move->from_i][7] = ' ';
        } 
        else 
        {  // Queen-Side Castling
            newboard->board[move->from_i][move->to_j + 1] = newboard->board[move->from_i][0];
            newboard->board[move->from_i][0] = ' ';
        }
    }

    // En Passant logic
    if (!isDrop && (piece == 'P' || piece == 'p') && capturedPiece == ' ') 
    {
        if (abs(move->from_j - move->to_j) == 1 && abs(move->from_i - move->to_i) == 1) 
        {
            // To capture pawn
            int capturePawn;
            if (piece == 'P') 
            {
                capturePawn = move->to_i - 1;
            } 
            else 
            {
                capturePawn = move->to_i + 1;
            }
            char target_piece = newboard->board[capturePawn][move->to_j];
            
            // Ensure en passant target is opponent's pawn
            if ((piece == 'P' && target_piece == 'p') || (piece == 'p' && target_piece == 'P')) 
            {
                char *prison;
                if (piece == 'P') 
                {
                    prison = newboard->bprison;
                } 
                else 
                {
                    prison = newboard->wprison;
                }
                if (strlen(prison) < MAX_PRISON_SIZE - 1) 
                {
                    strncat(prison, &target_piece, 1);
                }
                // Remove captured pawn from the board
                newboard->board[capturePawn][move->to_j] = ' ';
            }
        }
    }

    // Drop from airfield
    if (move->from_i == 8 || move->from_i == -1) 
    {
        char *airfield;
        if (move->from_i == 8) 
        {
            airfield = newboard->bairfield;
        } 
        else 
        {
            airfield = newboard->wairfield;
        }

        int len = strlen(airfield);
        for (int i = 0; i < len; i++) 
        {
            if (airfield[i] == move->promotion) 
            {
                // Remove piece from the airfield
                memmove(&airfield[i], &airfield[i + 1], len - i);
                break;
            }
        }

        newboard->board[move->to_i][move->to_j] = move->promotion;
    }
    // Drop from prison
    else if (move->from_i == 9 || move->from_i == -2) 
    {
        char *prison;
        if (move->from_i == 9) 
        {
            prison = newboard->bprison;
        } 
        else 
        {
            prison = newboard->wprison;
        }

        int len = strlen(prison);
        for (int i = 0; i < len; i++)
        {
            if (prison[i] == move->promotion) 
            {
                // Remove piece from the prison
                memmove(&prison[i], &prison[i + 1], len - i);
                break;
            }
        }

        newboard->board[move->to_i][move->to_j] = move->promotion;

        // Move hostage from opponent's prison to the same player's airfield
        char *opponentPrison;
        char *sameAirfield;

        if (move->from_i == 9) 
        {
            opponentPrison = newboard->wprison;
            sameAirfield = newboard->bairfield;
        } 
        else 
        {
            opponentPrison = newboard->bprison;
            sameAirfield = newboard->wairfield;
        }

        len = strlen(opponentPrison);
        for (int i = 0; i < len; i++) 
        {
            if (opponentPrison[i] == move->hostage) 
            {
                // Remove the hostage from opponent's prison
                memmove(&opponentPrison[i], &opponentPrison[i + 1], len - i);
                // Add the hostage to the airfield
                if (strlen(sameAirfield) < MAX_AIRFIELD_SIZE - 1) 
                {
                    strncat(sameAirfield, &move->hostage, 1);
                }
                break;
            }
        }
    }

    // Last piece move
    if (!isDrop) 
    {
        // Clear the from position only if not a drop
        newboard->board[move->from_i][move->from_j] = ' ';
    }
    
    // Handle promotion
    if ((piece == 'P' && move->to_i == 7) || (piece == 'p' && move->to_i == 0)) 
    {
        // Promotion condition met
        if (move->promotion != ' ') 
        {
            // Promoted piece
            newboard->board[move->to_i][move->to_j] = move->promotion;
        }
        else 
        {   
            // Default promotion to Queen
            newboard->board[move->to_i][move->to_j] = (piece == 'P') ? 'Q' : 'q';
        }
    } 
    else 
    {
        // Regular move
        newboard->board[move->to_i][move->to_j] = piece;
    }

    // Capture logic
    if (capturedPiece != ' ') 
    {
        char *prison;
        if (capturedPiece >= 'A' && capturedPiece <= 'Z') 
        {
            prison = newboard->bprison;
        } 
        else if (capturedPiece >= 'a' && capturedPiece <= 'z') 
        {
            prison = newboard->wprison;
        }
        else
        {
            prison = NULL;
        }

        if (prison != NULL && strlen(prison) < MAX_PRISON_SIZE - 1) 
        {
            strncat(prison, &capturedPiece, 1);
        }
    
        // Move the capturing piece to the target square
        newboard->board[move->to_i][move->to_j] = piece;

        // Clear the piece's original square if not a drop
        if (!isDrop) 
        {
            newboard->board[move->from_i][move->from_j] = ' ';
        }
    }

    return newboard;
}

// PART II
// Helper functions for moving restrictions and logic
int withinBounds(int i, int j) 
{
    return i >= 0 && i < 8 && j >= 0 && j < 8;
}

int isEnemyPiece(char piece, int colour) 
{
    if (piece == ' ') return 0;
    if (colour == 0) 
    {
        return piece >= 'a' && piece <= 'z';
    } 
    else 
    {
        return piece >= 'A' && piece <= 'Z';
    }
}

int isAllyPiece(char piece, int colour) 
{
    if (piece == ' ') return 0;
    if (colour == 0) 
    {
        return piece >= 'A' && piece <= 'Z';
    } 
    else 
    {
        return piece >= 'a' && piece <= 'z';
    }
}

void addMove(move_t ***moveArray, int *moveCount, int from_i, int from_j, int to_i, int to_j) 
{
    // Allocate or reallocate the moves array
    *moveArray = realloc(*moveArray, (*moveCount + 2) * sizeof(move_t *));
    
    // Create a new move
    move_t *newMove = malloc(sizeof(move_t));
    newMove->from_i = from_i;
    newMove->from_j = from_j;
    newMove->to_i = to_i;
    newMove->to_j = to_j;
    newMove->promotion = ' ';
    newMove->hostage = ' ';

    // Add the new move to the array
    (*moveArray)[*moveCount] = newMove;
    (*moveCount)++;

    // Null-terminate the array
    (*moveArray)[*moveCount] = NULL;
}

// Logic for pieces that can move across multiple squares
move_t **slidingMoves(exboard_t *board, int from_i, int from_j, int colour, int directions[][2], int numDirections) 
{
    move_t **validMoves = NULL;
    int moveCount = 0;

    for (int d = 0; d < numDirections; d++) 
    {
        int to_i = from_i;
        int to_j = from_j;

        while (1) 
        {
            to_i += directions[d][0];
            to_j += directions[d][1];

            if (!withinBounds(to_i, to_j)) break;

            char dest_piece = board->board[to_i][to_j];

            if (isAllyPiece(dest_piece, colour)) break;

            addMove(&validMoves, &moveCount, from_i, from_j, to_i, to_j);

            if (isEnemyPiece(dest_piece, colour)) break;
        }
    }

    // Null-terminate the move array
    if (validMoves != NULL)
    {
        validMoves = realloc(validMoves, (moveCount + 1) * sizeof(move_t *));
        validMoves[moveCount] = NULL;
    }

    return validMoves;
}

// Move function
// Function to create moves for a piece at a given position
move_t **moves(exboard_t *board, int from_i, int from_j) 
{
    // Get the piece at the specified position
    char piece = board->board[from_i][from_j];
    
    // Determine the colour of the piece
    int colour;
    if (piece >= 'A' && piece <= 'Z') 
    {
        colour = 0; // White
    } 
    else 
    {
        colour = 1; // Black
    }
    
    // Call the appropriate function
    // Switch case with toupper (considers both white and black pieces)
    switch (toupper(piece))
    {
        case 'N': // Knight
            return knightmoves(board, from_i, from_j, colour);
        case 'B': // Bishop
            return bishopmoves(board, from_i, from_j, colour);
        case 'R': // Rook
            return rookmoves(board, from_i, from_j, colour);
        case 'Q': // Queen
            return queenmoves(board, from_i, from_j, colour);
        case 'K': // King
            return kingmoves(board, from_i, from_j, colour);
        case 'P': // Pawn
            return pawnmoves(board, from_i, from_j, colour);
        default:
            // Invalid piece
            return NULL;
    }
}

// Knight moves
move_t **knightmoves(exboard_t *board, int from_i, int from_j, int colour) 
{
    // Possible knight moves (delta row, delta column)
    int directions[8][2] = 
    {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };

    move_t **validMoves = NULL;
    int moveCount = 0;

    for (int k = 0; k < 8; k++) 
    {
        int to_i = from_i + directions[k][0];
        int to_j = from_j + directions[k][1];

        if (withinBounds(to_i, to_j)) 
        {
            char dest_piece = board->board[to_i][to_j];
            if (!isAllyPiece(dest_piece, colour)) 
            {
                addMove(&validMoves, &moveCount, from_i, from_j, to_i, to_j);
            }
        }
    }

    // Null-terminate the move array
    if (validMoves != NULL)
    {
        validMoves = realloc(validMoves, (moveCount + 1) * sizeof(move_t *));
        validMoves[moveCount] = NULL;
    }

    return validMoves;
}

// Bishop moves
move_t **bishopmoves(exboard_t *board, int from_i, int from_j, int colour) 
{
    int directions[4][2] = 
    {
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };
    return slidingMoves(board, from_i, from_j, colour, directions, 4);
}

// Rook moves
move_t **rookmoves(exboard_t *board, int from_i, int from_j, int colour) 
{
    int directions[4][2] = 
    {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };
    return slidingMoves(board, from_i, from_j, colour, directions, 4);
}

// Queen moves
move_t **queenmoves(exboard_t *board, int from_i, int from_j, int colour) 
{
    int directions[8][2] = 
    {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };
    return slidingMoves(board, from_i, from_j, colour, directions, 8);
}

// King moves
move_t **kingmoves(exboard_t *board, int from_i, int from_j, int colour) 
{
    int directions[8][2] = 
    {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };

    move_t **validMoves = NULL;
    int moveCount = 0;

    for (int k = 0; k < 8; k++) 
    {
        int to_i = from_i + directions[k][0];
        int to_j = from_j + directions[k][1];

        if (withinBounds(to_i, to_j)) 
        {
            char dest_piece = board->board[to_i][to_j];
            if (!isAllyPiece(dest_piece, colour)) 
            {
                addMove(&validMoves, &moveCount, from_i, from_j, to_i, to_j);
            }
        }
    }

    // Null-terminate the move array
    if (validMoves != NULL)
    {
        validMoves = realloc(validMoves, (moveCount + 1) * sizeof(move_t *));
        validMoves[moveCount] = NULL;
    }

    return validMoves;
}

// Pawn moves
move_t **pawnmoves(exboard_t *board, int from_i, int from_j, int colour) 
{
    move_t **validMoves = NULL;
    int moveCount = 0;

    // Determine direction of movement (white pawns move up the board, black pawns move down)
    int direction;
    if (colour == 0) 
    {
        direction = 1;
    } 
    else 
    {
        direction = -1;
    }
    int to_i = from_i + direction;

    // Check one square forward
    if (withinBounds(to_i, from_j) && board->board[to_i][from_j] == ' ') 
    {
        addMove(&validMoves, &moveCount, from_i, from_j, to_i, from_j);

        // Check two squares forward (only from starting rank)
        int startRank;
        if (colour == 0) {
            startRank = 1;
        } 
        else {
            startRank = 6;
        }
        if (from_i == startRank) 
        {
            int to_iForward = from_i + 2 * direction;
            if (withinBounds(to_iForward, from_j) && board->board[to_iForward][from_j] == ' ') 
            {
                addMove(&validMoves, &moveCount, from_i, from_j, to_iForward, from_j);
            }
        }
    }

    // Check diagonal captures
    for (int j = -1; j <= 1; j += 2) 
    {
        int to_j = from_j + j;
        if (withinBounds(to_i, to_j)) 
        {
            char dest_piece = board->board[to_i][to_j];
            if (isEnemyPiece(dest_piece, colour)) 
            {
                addMove(&validMoves, &moveCount, from_i, from_j, to_i, to_j);
            }
        }
    }

    // Null-terminate the move array
    if (validMoves != NULL)
    {
        validMoves = realloc(validMoves, (moveCount + 1) * sizeof(move_t *));
        validMoves[moveCount] = NULL;
    }

    return validMoves;
}
