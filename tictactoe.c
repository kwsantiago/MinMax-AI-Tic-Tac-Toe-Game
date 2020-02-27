#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {NOUGHTS, CROSSES, BORDER, EMPTY};
enum {HUMANWIN, COMPWIN, DRAW};

const int directions[4] = {1,5,4,6};

const int ConvertTo25[9] = {
    6,7,8,
    11,12,13,
    16,17,18
};

const int inMiddle = 4;
const int corners[4] = {0,2,6,8};

int getNumForDir(int startSq, const int dir, const int *board, const int us){
    int found = 0;
    while(board[startSq] != BORDER){
        if(board[startSq] != us){
            break;
        }
        found++;
        startSq += dir;
    }
    return found;
}

int findThreeInARow(const int *board, const int ourindex, const int us){
    int dirIndex = 0;
    int dir =0;
    int threeCount = 1;

    for(dirIndex = 0; dirIndex < 4; ++dirIndex){
        dir = directions[dirIndex];
        threeCount += getNumForDir(ourindex + dir, dir, board, us);
        threeCount += getNumForDir(ourindex + dir * -1, dir * -1, board, us);
        if(threeCount == 3)
            break;
        threeCount = 1;
    }
    return threeCount;
}

void initializeBoard(int *board){
    int index = 0;
    for(index = 0; index < 25; ++index){
        board[index] = BORDER;
    }

    for(index = 0; index < 9; ++index){
        board[ConvertTo25[index]] = EMPTY;
    }
}

void printBoard(const int *board){
    int index = 0;
    char pceChars[] = "OX|-";

    printf("\n");
    for(index = 0; index < 9; ++index){
        if(index != 0 && index % 3 == 0){
            printf("\n\n");
        }
        printf("%4c", pceChars[board[ConvertTo25[index]]]);
    }
    printf("\n");
}

int hasEmpty(const int *board){
    int index = 0;

    for(index = 0; index < 9; ++index){
        if(board[ConvertTo25[index]] == EMPTY)
            return 1;
    }
    return 0;
}

int makeMove(int *board, const int sq, const side){
    board[sq] = side;
}

int getNextBest(const int *board){
    int ourMove = ConvertTo25[inMiddle];
    if(board[ourMove] == EMPTY){
        return ourMove;
    }

    int index = 0;
    ourMove = -1;

    for(index = 0; index < 4; index++){
        ourMove = ConvertTo25[corners[index]];
        if(board[ourMove] == EMPTY){
            break;
        }
        ourMove = -1;
    }
    return ourMove;
}

int getWinningMove(int *board, const int side){
    int ourMove = -1;
    int winFound = 0;
    int index = 0;

    for(index = 0; index < 9; ++index){
        if(board[ConvertTo25[index]] == EMPTY){
            ourMove = ConvertTo25[index];
            board[ourMove] = side;

            if(findThreeInARow(board, ourMove, side) == 3){
                winFound = 1;
            }
            board[ourMove] = EMPTY;
            if(winFound == 1){
                break;
            }
            ourMove = -1;
        };
    }
    return ourMove;
}

int getComputerMove(int *board, const int side){
    int index = 0;
    int numFree = 0;
    int availableMoves[9];
    int randMove = 0;

    randMove = getWinningMove(board, side);
    if(randMove != -1)
        return randMove;

    randMove = getWinningMove(board, side ^ 1);
    if(randMove != -1)
        return randMove;

    randMove = getNextBest(board);
    if(randMove != -1)
        return randMove;

    randMove = 0;
    for(index = 0; index < 9; ++index){
        if(board[ConvertTo25[index]] == EMPTY){
            availableMoves[numFree++] = ConvertTo25[index];
        };
    }

    randMove = (rand() % numFree);
    return availableMoves[randMove];
}

int getHumanMove(const int *board){
    char userInput[4];
    int moveOK = 0;
    int move = -1;

    while(moveOK == 0){
        printf("\nPlease enter a move from 1 to 9: ");
        fgets(userInput, 3, stdin);
        fflush(stdin);

        if(strlen(userInput) != 2){
            printf("Invalid strlen()\n");
            continue;
        }

        if(sscanf(userInput, "%d", &move) != 1){
            move = -1;
            printf("Invalid sscanf()\n");
            continue;
        }

        if(move < 1 || move > 9){
            move = -1;
            printf("Invalid range\n");
            continue;
        }

        move--; // Zero indexing

        if(board[ConvertTo25[move]] != EMPTY){
            move = -1;
            printf("Square not available\n");
            continue;
        }
        moveOK = 1;
    }

    printf("Making move...%d\n", (move+1));
    return ConvertTo25[move];
}

void runGame(){
    int gameOver = 0;
    int side = NOUGHTS;
    int lastMoveMade = 0;
    int board[25];

    initializeBoard(&board[0]);
    printBoard(&board[0]);

    while(!gameOver){
        if(side == NOUGHTS){
            lastMoveMade = getHumanMove(&board[0]);
            makeMove(&board[0], lastMoveMade, side);
            side = CROSSES;
        }else{
            lastMoveMade = getComputerMove(&board[0], side);
            makeMove(&board[0], lastMoveMade, side);
            side = NOUGHTS;
            printBoard(&board[0]);
        }

        if(findThreeInARow(board, lastMoveMade, side ^ 1) == 3){
            printf("\nGame over!\n");
            gameOver = 1;
            if(side == NOUGHTS)
                printf("Computer Wins\n");
            else
                printf("Human Wins\n");
        }

        if(!hasEmpty(board)){
            printf("\nGame over!\n");
            gameOver = 1;
            printf("It's a draw\n");
        }
    }
    printBoard(&board[0]);
}

int main(){
    srand(time(NULL));
    runGame();

    return 0;
}
