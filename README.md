# Hostage Chess

Hostage Chess is a chess variant introducing **prisons** and **airfields**. Captured pieces are held in a "prison" and can be exchanged for other hostages or dropped onto designated "airfields" to re-enter play. This adds layers of strategy to traditional chess.

## Features
- **Prisons**: Captured pieces are stored in a separate space and can be exchanged or reintroduced.
- **Airfields**: Special squares where redeemed pieces can re-enter the board.
- **Board Representation**: Fully functional chessboard with standard moves, castling, promotions, and Hostage Chess rules.

## How to Build and Run
1. Clone the repository.
2. Open a terminal in the directory.
3. Compile the program using:
   
   ```bash
   make
   ```
5. Run the test file:
   
   ```bash
   ./test1
   ```

## Example Usage
The provided `test1.c` demonstrates:
- Initializing a chessboard with `newboard()`.
- Applying moves using `apply_move()`.
- Printing the board state with `stringboard()`.

### Output Example
The program simulates:
1. Moving a knight and bishop to clear the castling path.
2. Performing a queen-side castle for the white king.
3. Printing the board states before and after castling.

## Writing Custom Tests
You can create your own scenarios:
1. Include `hclib.h`.
2. Initialize the board:
   
   ```c
   exboard_t *board = newboard();
   ```
4. Define moves and apply them:
   
   ```c
   move_t move = {from_i, from_j, to_i, to_j, promotion, hostage};
   exboard_t *newBoard = apply_move(board, &move);
   ```
6. Print the board:
   
   ```c
   char *boardStr = stringboard(newBoard);
   printf("%s\n", boardStr);
   ```
8. Compile and run:
   
   ```bash
   gcc -o test test.c hclib.c
   ./test
   ```

You can use this setup to further expand the board functionality, experiment with new rules, or create custom gameplay scenarios for testing and development.

## Key Concepts
- **Prisons and Airfields**: Manage captured and redeemed pieces strategically.
- **Dynamic Gameplay**: Adds complexity and creativity to traditional chess.

For more details, explore the code and test cases provided in the repository, and feel free to experiment with your own implementations and improvements.
