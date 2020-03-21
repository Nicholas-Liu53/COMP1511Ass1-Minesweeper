// Assignment 1 20T1 COMP1511: Minesweeper
// minesweeper.c
//
// This program was written by NICHOLAS LIU (z5310207)
// on 10th~28th MARCH 2020
//
// Version 1.0.0 (2020-03-08): Assignment released.
// Version 1.0.1 (2020-03-08): Fix punctuation in comment.
// Version 1.0.2 (2020-03-08): Fix second line of header comment to say minesweeper.c

#include <stdio.h>
#include <stdlib.h>

// Possible square states.
#define VISIBLE_SAFE    0
#define HIDDEN_SAFE     1
#define HIDDEN_MINE     2

// The size of the starting grid.
#define SIZE 8

// The possible command codes.
#define DETECT_ROW              1
#define DETECT_COL              2
#define DETECT_SQUARE           3
#define REVEAL_SQUARE           4
#define GAMEPLAY_MODE           5
#define DEBUG_MODE              6
#define REVEAL_RADIAL           7

// Add any extra #defines here.

void initialise_field(int minefield[SIZE][SIZE]);
void print_debug_minefield(int minefield[SIZE][SIZE]);
void print_gameplay_minefield(int minefield[SIZE][SIZE]);
void print_deadplay_minefield(int minefield[SIZE][SIZE]);

// Place your function prototyes here.
void plant_mines(int minefield[SIZE][SIZE]);
void detectRowColumn(int minefield[SIZE][SIZE], int command, int rowColumn);
int detectSquare(int minefield[SIZE][SIZE], int row, int column, int size);
int reveal_winStatus(int minefield[SIZE][SIZE], int row, int column);
void revealRadial(int minefield[SIZE][SIZE], int row, int column);
void safeFirstTurn(int minefield[SIZE][SIZE], int row, int column, int firstTurn);

int main(void) {

    // Game boots up :o
    int minefield[SIZE][SIZE];
    initialise_field(minefield);
    printf("Welcome to minesweeper!\n");
    
    // Scan in the number of mines, and
    printf("How many mines? ");
    // scan in the coordinates of mines and place them on the grid.
    plant_mines(minefield);
    
    printf("Game Started\n");
    int command;
    int gamemode = 0; // debug mode (0) or gameplay mode (1), debug mode is the default/initial mode
    int hintCounter = 0; // Counter to see how many hints been used
    int firstTurn = 0; // Counter to see if the first turn has been used or not (If > 0, then been used)
    print_debug_minefield(minefield);

    // The following while loop scans in commands to play the game until the game ends.
    // A game ends when the player wins, loses, or enters EOF (Ctrl+D).
    while (scanf("%d", &command) == 1) {
        if (gamemode == 0) { // debug mode 

            // For detecting mines in row/column (Command 1 or 2)
            if (command == DETECT_ROW || command == DETECT_COL) {
                int rowColumn;
                scanf("%d", &rowColumn);
                if (hintCounter < 3) { // Hint restriction (Stage 03)
                    detectRowColumn(minefield, command, rowColumn);
                    hintCounter++;
                } else { // Out of hints :c (Stage 03)
                    printf("Help already used\n");
                }    
                print_debug_minefield(minefield);
            }
            
            // For detecting square (command 3)
            if (command == DETECT_SQUARE) {
                if (hintCounter < 3) { // Hint restriction (Stage 03)
                    int row, column, size;
                    scanf("%d", &row);
                    scanf("%d", &column);
                    scanf("%d", &size);
                    detectSquare(minefield, row, column, size);
                    int detected = detectSquare(minefield, row, column, size);
                    printf(
                        "There are %d mine(s) in the square centered at row %d, column %d of size %d\n",
                        detected, row, column, size
                    );
                    hintCounter++;
                } else { // Out of hints (Stage 03)
                    printf("Help already used\n");
                }
                print_debug_minefield(minefield);
            }
            
            // For revealing squares (Command 4)
            if (command == REVEAL_SQUARE) {
                int row;
                scanf("%d", &row);
                int column;
                scanf("%d", &column);
                
                // Safe First Turn :o
                safeFirstTurn(minefield, row, column, firstTurn);
                
                int winStatus = reveal_winStatus(minefield, row, column);
                if (winStatus == 1) { // if you win
                    print_debug_minefield(minefield);
                    return 1;
                } else if (winStatus == 2) { // if you lose
                    print_debug_minefield(minefield);
                    return 1;
                } else if (winStatus == 0) { // game continues...
                    minefield[row][column] = 0;
                }
                print_debug_minefield(minefield);
                firstTurn++;
            }

            // To switch to Gameplay mode (Command 5)
            if (command == GAMEPLAY_MODE) {
                gamemode = 1;
                printf("Gameplay mode activated\n");
            }

            // For revealing radials (Command 7)
            if (command == REVEAL_RADIAL) {
                int row, column;
                scanf("%d %d", &row, &column);
                revealRadial(minefield, row, column);

                // Safe First Turn :o
                safeFirstTurn(minefield, row, column, firstTurn);
                
                int winStatus = reveal_winStatus(minefield, row, column);
                if (winStatus == 1) { // if you win
                    print_debug_minefield(minefield);
                    return 1;
                } else if (winStatus == 2) { // if you lose
                    print_debug_minefield(minefield);
                    return 1;
                } else if (winStatus == 0) { // game continues...
                    minefield[row][column] = 0;
                }
                print_debug_minefield(minefield);
                firstTurn++;
            }
        }
        if (gamemode == 1) { // gameplay mode
            if (command != DEBUG_MODE) { // if not going back to debug mode

                // For detecting mines in row/column (Command 1 or 2)
                if (command == DETECT_ROW || command == DETECT_COL) {
                    int rowColumn;
                    scanf("%d", &rowColumn);
                    detectRowColumn(minefield, command, rowColumn);
                }
                
                // For detecting square (command 3)
                if (command == DETECT_SQUARE) {
                    int row, column, size;
                    scanf("%d", &row);
                    scanf("%d", &column);
                    scanf("%d", &size);
                    detectSquare(minefield, row, column, size);
                    int detected = detectSquare(minefield, row, column, size);
                    printf(
                        "There are %d mine(s) in the square centered at row %d, column %d of size %d\n",
                        detected, row, column, size
                    );
                }
                
                // For revealing squares (Command 4)
                if (command == REVEAL_SQUARE) {
                    int row;
                    scanf("%d", &row);
                    int column;
                    scanf("%d", &column);

                    // Safe First Turn :o
                    safeFirstTurn(minefield, row, column, firstTurn);
                    
                    int winStatus = reveal_winStatus(minefield, row, column);
                    if (winStatus == 1) { // if you win
                        print_gameplay_minefield(minefield);
                        return 1;
                    } else if (winStatus == 2) { // if you lose
                        print_deadplay_minefield(minefield);
                        return 2;
                    } else if (winStatus == 0) { // game continues...
                        minefield[row][column] = 0;
                    }
                    firstTurn++;
                }

                // For revealing radials (Command 7)
                if (command == REVEAL_RADIAL) {
                    int row, column;
                    scanf("%d %d", &row, &column);
                    revealRadial(minefield, row, column);

                    // Safe First Turn :o
                    safeFirstTurn(minefield, row, column, firstTurn);

                    int winStatus = reveal_winStatus(minefield, row, column);
                    if (winStatus == 1) { // if you win
                        print_gameplay_minefield(minefield);
                        return 1;
                    } else if (winStatus == 2) { // if you lose
                        print_deadplay_minefield(minefield);
                        return 2;
                    } else if (winStatus == 0) { // game continues...
                        minefield[row][column] = 0;
                    }
                    firstTurn++;
                }
                print_gameplay_minefield(minefield);

            } else if (command == DEBUG_MODE) { // To go back to Debug mode (command 6)
                gamemode = 0;
                printf("Debug mode activated\n");
                print_debug_minefield(minefield);
            }
        }
    }

    return 0;
}

// Set the entire minefield to HIDDEN_SAFE.
void initialise_field(int minefield[SIZE][SIZE]) {
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            minefield[i][j] = HIDDEN_SAFE;
            j++;
        }
        i++;
    }
}

// Print out the actual values of the minefield.
void print_debug_minefield(int minefield[SIZE][SIZE]) {
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            printf("%d ", minefield[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }
}

// Print out the minefield in gameplay mode (Stage 03)
void print_gameplay_minefield(int minefield[SIZE][SIZE]) {
    printf("..\n\\/\n");
    printf("    ");
    int rowCounter = 0;
    while (rowCounter < SIZE) {
        if (rowCounter < 10) {
            printf("0%d ", rowCounter);
        } else {
            printf("%d ", rowCounter);
        }
        rowCounter++;
    }
    printf("\n   ");
    rowCounter = 0;
    while (rowCounter < 3 * SIZE + 1) {
        printf("-");
        rowCounter++;
    }
    printf("\n");
    int columnCounter = 0;
    while (columnCounter < SIZE) {
        rowCounter = 0;
        if (columnCounter < 10) {
            printf("0%d |", columnCounter);
        } else {
            printf("%d |", columnCounter);
        }
        while (rowCounter < SIZE - 1) {
            if (minefield[columnCounter][rowCounter] == 1 || minefield[columnCounter][rowCounter] == 2) {
                printf("## ");
            }
            if (minefield[columnCounter][rowCounter] == 0) {
                if (detectSquare(minefield, rowCounter, columnCounter, 3) == 0) {
                    printf("   ");
                } else {
                    printf("0%d ", detectSquare(minefield, rowCounter, columnCounter, 3));
                }
            }
            rowCounter++;
        }
        if (rowCounter == SIZE - 1) {
            if (minefield[columnCounter][rowCounter] == 1 || minefield[columnCounter][rowCounter] == 2) {
                printf("##|\n");
            }
            if (minefield[columnCounter][rowCounter] == 0) {
                if (detectSquare(minefield, rowCounter, columnCounter, 3) == 0) {
                    printf("  |\n");
                } else {
                    printf("0%d|", detectSquare(minefield, rowCounter, columnCounter, 3));
                }
            }
            rowCounter++;
            columnCounter++;
        }
    }
    printf("   ");
    rowCounter = 0;
    while (rowCounter < 3 * SIZE + 1) {
        printf("-");
        rowCounter++;
    }
    printf("\n");
}

// Print out the gameplay minefield when the player dies :c (Stage 03)
void print_deadplay_minefield(int minefield[SIZE][SIZE]) {
    printf("xx\n/\\\n");
    printf("    ");
    int i = 0;
    while (i < SIZE) {
        if (i < 10) {
            printf("0%d ", i);
        } else {
            printf("%d ", i);
        }
        i++;
    }
    printf("\n   ");
    i = 0;
    while (i < 3 * SIZE + 1) {
        printf("-");
        i++;
    }
    printf("\n");
    int columnCounter = 0;
    while (columnCounter < SIZE) {
        i = 0;
        if (columnCounter < 10) {
            printf("0%d |", columnCounter);
        } else {
            printf("%d |", columnCounter);
        }
        while (i < SIZE - 1) {
            if (minefield[columnCounter][i] == 1) {
                printf("## ");
            }
            if (minefield[columnCounter][i] == 2) {
                printf("() ");
            }
            if (minefield[columnCounter][i] == 0) {
                if (detectSquare(minefield, i, columnCounter, 3) == 0) {
                    printf("   ");
                } else {
                    printf("0%d ", detectSquare(minefield, i, columnCounter, 3));
                }
            }
            i++;
        }
        if (i == SIZE - 1) {
            if (minefield[columnCounter][i] == 1) {
                printf("##|\n");
            }
            if (minefield[columnCounter][i] == 2) {
                printf("()|\n");
            }
            if (minefield[columnCounter][i] == 0) {
                if (detectSquare(minefield, i, columnCounter, 3) == 0) {
                    printf("  |\n");
                } else {
                    printf("0%d|", detectSquare(minefield, i, columnCounter, 3));
                }
            }
            i++;
            columnCounter++;
        }
    }
    printf("   ");
    i = 0;
    while (i < 3 * SIZE + 1) {
        printf("-");
        i++;
    }
    printf("\n");
}

// Function to plant mines into minefield (Stage 01)
void plant_mines(int minefield[SIZE][SIZE]) {
    int numMines;
    scanf("%d", &numMines);

    int mineCounter = 0;
    printf("Enter pairs:\n");
    while (mineCounter < numMines) {
        int row, column;
        scanf("%d %d", &row, &column);
        if (!(row < 0 || row >= SIZE || column < 0 || column >= SIZE)) {
            minefield[row][column] = HIDDEN_MINE;
        }
        mineCounter++;
    }
}

// Function to detect square (Stage 01, command 1 and 2)
void detectRowColumn(int minefield[SIZE][SIZE], int command, int rowColumn) {
    int detectCounter = 0;
    int detected = 0;
    if (command == DETECT_ROW) {
        while (detectCounter < SIZE) {
            if (minefield[rowColumn][detectCounter] == 2) {
                detected++;
            }
            detectCounter++;
        }
        printf("There are %d mine(s) in row %d\n", detected, rowColumn);
        detectCounter = 0;
        detected = 0;
    } else if (command == DETECT_COL) {
        while (detectCounter < SIZE) {
            if (minefield[detectCounter][rowColumn] == 2) {
                detected++;
            }
            detectCounter++;
        }
        printf("There are %d mine(s) in column %d\n", detected, rowColumn);
        detectCounter = 0;
        detected = 0;
    }
} 

// Function to Detect Square (Stage 02, command 3)
int detectSquare(int minefield[SIZE][SIZE], int row, int column, int size) {
    int detected = 0;

    int detectCounter = row - (size - 1) / 2;
    int columnCounter = column - (size - 1) / 2;
    if (detectCounter < 0) {
        detectCounter = 0;
    }
    if (columnCounter < 0) {
        columnCounter = 0;
    }
    while (columnCounter >= 0 && columnCounter <= column + (size - 1) / 2 && columnCounter < SIZE) {
        while (detectCounter >= 0 && detectCounter <= row + (size - 1) / 2 && detectCounter < SIZE) {
            if (minefield[detectCounter][columnCounter] == 2) {
                detected++;
            }
            detectCounter++;
        }
        columnCounter++;
        detectCounter = row - (size - 1) / 2;
        if (detectCounter < 0) {
            detectCounter = 0;
        }
    }
    return detected;
}

// Function to reveal 3x3 squares and see if you won the game or not (Stage 02, command 4)
int reveal_winStatus(int minefield[SIZE][SIZE], int row, int column) {
    int detected = 0;
    int detectCounter = row - 1;
    int columnCounter = column - 1;
    while (columnCounter <= column + 1) { // This while loop reveals the squares...
        while (detectCounter <= row + 1) {
            if(detectCounter >= 0 && detectCounter < SIZE && columnCounter >= 0 && columnCounter < SIZE) {
                if (minefield[detectCounter][columnCounter] == 2) {
                    detected++;
                }
            }
            detectCounter++;
        }
        columnCounter++;
        detectCounter = row - 1;
    }
    detectCounter = row - 1;
    columnCounter = column - 1;
    if (detected == 0) {    
        while (columnCounter <= column + 1) {
            while (detectCounter <= row + 1) {
                if(detectCounter >= 0 && detectCounter < SIZE && columnCounter >= 0 && columnCounter < SIZE) {
                    minefield[detectCounter][columnCounter] = 0;
                }
                detectCounter++;
            }
            columnCounter++;
            detectCounter = row - 1;
        }
        detectCounter = 0;
        columnCounter = 0;
        while (columnCounter < SIZE){
            while (detectCounter < SIZE) {
                if (minefield[detectCounter][columnCounter] == 0 || minefield[detectCounter][columnCounter] == 2) {
                    detected++;
                }
                detectCounter++; 
            }
            columnCounter++;
            detectCounter = 0;    
        }
    }
    // The following if/else if statements return a value, indicating whether you win the game or not
    if (detected == SIZE * SIZE) { // If you clear the minefield without dying :>
        printf("Game Won!\n");
        return 1;
    } else if (minefield[row][column] == 2) { // If you step on a mine directly :c
        printf("Game over\n");
        return 2;
    } else { // Game continues...
        return 0;
    }
    return 0;
}

// Function to reveal radials (Stage 04, command 7)
void revealRadial(int minefield[SIZE][SIZE], int row, int column) {
    if (minefield[row][column] != 2) {
        // First do up branch of radial
        int displacementCounter = 0;
        while (detectSquare(minefield, row, column - displacementCounter, 3) == 0 && column - displacementCounter > -1) {
            minefield[row][column - displacementCounter] = 0;
            displacementCounter++;
        }
        if (detectSquare(minefield, row, column - displacementCounter, 3) != 0 && column - displacementCounter > -1) {
            minefield[row][column - displacementCounter] = 0;
        }
        // Then do down branch of radial
        displacementCounter = 0;
        while (detectSquare(minefield, row, column + displacementCounter, 3) == 0 && column + displacementCounter < SIZE) {
            minefield[row][column + displacementCounter] = 0;
            displacementCounter++;
        }
        if (detectSquare(minefield, row, column + displacementCounter, 3) != 0 && column + displacementCounter < SIZE) {
            minefield[row][column + displacementCounter] = 0;
        }
        // left branch of radial
        displacementCounter = 0;
        while (detectSquare(minefield, row - displacementCounter, column, 3) == 0 && row - displacementCounter > -1) {
            minefield[row - displacementCounter][column] = 0;
            displacementCounter++;
        }
        if (detectSquare(minefield, row - displacementCounter, column, 3) != 0 && row - displacementCounter > -1) {
            minefield[row - displacementCounter][column] = 0;
        }
        // right branch of radial
        displacementCounter = 0;
        while (detectSquare(minefield, row + displacementCounter, column, 3) == 0 && row + displacementCounter < SIZE) {
            minefield[row + displacementCounter][column] = 0;
            displacementCounter++;
        }
        if (detectSquare(minefield, row + displacementCounter, column, 3) != 0 && row + displacementCounter < SIZE) {
            minefield[row + displacementCounter][column] = 0;
        }
        // Northeast(?) branch of radial
        int horizontalCounter = 0, verticalCounter = 0;
        while (detectSquare(minefield, row + horizontalCounter, column - verticalCounter, 3) == 0 && row + horizontalCounter < SIZE && column - verticalCounter > -1) {
            minefield[row + horizontalCounter][column - verticalCounter] = 0;
            horizontalCounter++;
            verticalCounter++;
        }
        if (detectSquare(minefield, row + horizontalCounter, column - verticalCounter, 3) != 0 && row + horizontalCounter < SIZE && column - verticalCounter > -1) {
            minefield[row + horizontalCounter][column - verticalCounter] = 0;
        }
        // Southeast branch of radial
        horizontalCounter = 0, verticalCounter = 0;
        while (detectSquare(minefield, row + horizontalCounter, column + verticalCounter, 3) == 0 && row + horizontalCounter < SIZE && column + verticalCounter < SIZE) {
            minefield[row + horizontalCounter][column + verticalCounter] = 0;
            horizontalCounter++;
            verticalCounter++;
        }
        if (detectSquare(minefield, row + horizontalCounter, column + verticalCounter, 3) != 0 && row + horizontalCounter < SIZE && column + verticalCounter < SIZE) {
            minefield[row + horizontalCounter][column + verticalCounter] = 0;
        }
        // Southwest branch of radial
        horizontalCounter = 0, verticalCounter = 0;
        while (detectSquare(minefield, row - horizontalCounter, column + verticalCounter, 3) == 0 && row - horizontalCounter > -1 && column + verticalCounter < SIZE) {
            minefield[row - horizontalCounter][column + verticalCounter] = 0;
            horizontalCounter++;
            verticalCounter++;
        }
        if (detectSquare(minefield, row - horizontalCounter, column + verticalCounter, 3) != 0 && row - horizontalCounter > -1 && column + verticalCounter < SIZE) {
            minefield[row - horizontalCounter][column + verticalCounter] = 0;
        }
        // Northwest branch of radial 7:^)
        horizontalCounter = 0, verticalCounter = 0;
        while (detectSquare(minefield, row - horizontalCounter, column - verticalCounter, 3) == 0 && row - horizontalCounter > -1 && column - verticalCounter > -1) {
            minefield[row - horizontalCounter][column - verticalCounter] = 0;
            horizontalCounter++;
            verticalCounter++;
        }
        if (detectSquare(minefield, row - horizontalCounter, column - verticalCounter, 3) != 0 && row - horizontalCounter > -1 && column - verticalCounter > -1) {
            minefield[row - horizontalCounter][column - verticalCounter] = 0;
        }
    }
}

// Function enacts Safe First Term by shifting the entire field downwards by 1
void safeFirstTurn(int minefield[SIZE][SIZE], int row, int column, int firstTurn) {
    if (minefield[row][column] == 2 && firstTurn == 0) {
        while (minefield[row][column] == 2) {
            // create temporary row (for the last row to come back up)
            int tempRow[SIZE];
            
            // store last row into the temporary row
            int columnCounter = 0;
            while (columnCounter < SIZE) {
                tempRow[columnCounter] = minefield[SIZE - 1][columnCounter];
                columnCounter++;
            }

            // store 2nd last row into last row, 3rd last row into 2nd last row,... 
            columnCounter = 0;
            int rowCounter = SIZE - 1;
            while (rowCounter > 0) {
                while (columnCounter < SIZE) {
                    minefield[rowCounter][columnCounter] = minefield[rowCounter - 1][columnCounter];
                    columnCounter++;
                }
                columnCounter = 0;
                rowCounter--;
            }

            // store last row into first row
            columnCounter = 0;
            while (columnCounter < SIZE) {
                minefield[0][columnCounter] = tempRow[columnCounter];
                columnCounter++;
            }
        }
    }
}
