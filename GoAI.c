/* 
 * File:   GoAI.c
 * Author: user
 *
 * Created on October 31, 2015, 3:42 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>

/*
 * 
 */

void printBoard(char board[26][26], int size);
void resetBoard(char board [26][26], int size);
bool positionInBounds(int size, char row, char col);
int charToIndex(char input);
bool configBoard(char board [26][26], int size);
bool checkLegalInDirection(char board [26][26], int size, char color, char row, char col, int dRow, int dCol);
bool movesExist(char board[26][26], int size, char color);
bool validMove(char board [26][26], int size, char color, char row, char col);
void playMove(char board [26][26], int size, char color, char row, char col);

void computerMove(char board [26][26], int size, char color);
int valueMove(char board[26][26], int size, char color, int rowIndex, int colIndex);
int valueBoard(char board [26][26], int size, char color);

double priority(char board [26][26], int size, char color, char row, char col);
int gameLength (char board [26][26], int size);
int potValue(char board [26][26], int size, char color, char row, char col, int moves );
double valueMovePosition(char board[26][26], int size, char color, char row, char col);
bool isCorner(int size, char row, char col);
bool isAdjCorner (int size, char row, char col);
bool isEdge (int size, char row, char col);
bool isAdjEdge(int size, char row, char col);
char oppositeColor(char color);
bool capAdjEdge(char board [26][26], int size, char color, char row, char col);

int main(void) {
    char board[26][26], color, row, col, computer, player;
    int size;
    bool computerTurn, defaultWin = false;


    printf("Enter the board dimension: ");
    scanf("%d", &size);
    resetBoard(board, size);
    printf("Computer plays (B/W) : ");
    scanf(" %c", &computer);
    printBoard(board, size);
    if (computer == 'B') {
        player = 'W';
        computerTurn = true;
    } else {
        player = 'B';
        computerTurn = false;
    }

    while (movesExist(board, size, computer) || movesExist(board, size, player)) {

        if (computerTurn) {
            computerMove(board, size, computer);
        } else {
            printf("Enter move for colour %c (RowCol): ", player);
            scanf(" %c%c", &row, &col);
            if (validMove(board, size, player, row, col)) {
                playMove(board, size, player, row, col);

            } else {
                printf("Invalid Move.\n");
                defaultWin = true;
                break;
            }

        }
        computerTurn = !computerTurn;

        if (computerTurn && !movesExist(board, size, computer)) {
            computerTurn = false;
            printf("%c player has no valid move.\n", computer);
        } else if (!computerTurn && !movesExist(board, size, player)) {
            computerTurn = true;
            printf("%c player has no valid move.\n", player);
        }
        printBoard(board, size);
    }
    if (!defaultWin) {

        if (valueBoard(board, size, computer) > valueBoard(board, size, player))
            printf("%c player wins.", computer);
        else if (valueBoard(board, size, computer) < valueBoard(board, size, player))
            printf("%c player wins.", player);
        else
            printf("It is a tie.");
    } else
        printf("%c player wins", computer);
    return (0);
}

void printBoard(char board [26][26], int size) {
    int i, j;
    for (i = 0; i <= size; i++) {

        for (j = 0; j <= size; j++) {

            if (i > 0 && j > 0)
                printf("%c", board[i - 1][j - 1]);
            else if (i == 0 && j == 0)
                printf("  ");
            else if (i == 0)
                printf("%c", 'a' + j - 1);
            else if (j == 0)
                printf("%c ", 'a' + i - 1);

        }
        printf("\n");
    }
}

void resetBoard(char board [26][26], int size) {
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            board[i][j] = 'U';
        }
    }
    board[size / 2 - 1][size / 2 - 1] = 'W';
    board[size / 2][size / 2] = 'W';

    board[size / 2][size / 2 - 1] = 'B';
    board[size / 2 - 1][size / 2] = 'B';
}

bool positionInBounds(int size, char row, char col) {
    if (charToIndex(row) >= size || charToIndex(col) >= size || charToIndex(row) < 0 || charToIndex(col) < 0)
        return false;
    else
        return true;
}

int charToIndex(char input) {
    return input - 'a';
}

bool configBoard(char board [26][26], int size) {
    char color, row, col;

    scanf(" %c%c%c", &color, &row, &col);
    if (color == '!' && row == '!' && col == '!')
        return false;
    else {

        if (positionInBounds(size, row, col))
            board[charToIndex(row)][charToIndex(col)] = color;
        return true;
    }

}

bool checkLegalInDirection(char board [26][26], int size, char color, char row, char col, int dRow, int dCol) {
    int i = 2;
    bool firstFound = false, secondFound = false;
    char opposite;

    if (color == 'W')
        opposite = 'B';
    else
        opposite = 'W';

    if (board[charToIndex(row + dRow)][charToIndex(col + dCol)] == 'U' || board[charToIndex(row + dRow)][charToIndex(col + dCol)] == color || board[charToIndex(row)][charToIndex(col)] != 'U')
        return false;
    else
        firstFound = true;

    while (positionInBounds(size, row + i * dRow, col + i * dCol)) {


        if (firstFound && board[charToIndex(row + i * dRow)][charToIndex(col + i * dCol)] == 'U') {
            secondFound = false;
            break;
        } else if (firstFound && board[charToIndex(row + i * dRow)][charToIndex(col + i * dCol)] == color) {
            secondFound = true;
            break;
        }

        i++;
    }
    if (firstFound && secondFound)
        return true;
    else
        return false;

}

bool movesExist(char board [26][26], int size, char color) {
    int i, j;
    bool hasMoves = false;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {

            if (checkLegalInDirection(board, size, color, i + 'a', j + 'a', -1, -1)) {
                //printf("%c%c\n", i+'a',j+'a');
                hasMoves = true;
            } else if (checkLegalInDirection(board, size, color, i + 'a', j + 'a', -1, 0)) {
                //printf("%c%c\n", i+'a',j+'a');
                hasMoves = true;
            } else if (checkLegalInDirection(board, size, color, i + 'a', j + 'a', -1, 1)) {
                //printf("%c%c\n", i+'a',j+'a');
                hasMoves = true;
            } else if (checkLegalInDirection(board, size, color, i + 'a', j + 'a', 0, -1)) {
                //printf("%c%c\n", i+'a',j+'a');
                hasMoves = true;
            } else if (checkLegalInDirection(board, size, color, i + 'a', j + 'a', 0, 1)) {
                //printf("%c%c\n", i+'a',j+'a');
                hasMoves = true;
            } else if (checkLegalInDirection(board, size, color, i + 'a', j + 'a', 1, -1)) {
                //printf("%c%c\n", i+'a',j+'a');
                hasMoves = true;
            } else if (checkLegalInDirection(board, size, color, i + 'a', j + 'a', 1, 0)) {
                //printf("%c%c\n", i+'a',j+'a');
                hasMoves = true;
            } else if (checkLegalInDirection(board, size, color, i + 'a', j + 'a', 1, 1)) {
                //printf("%c%c\n", i+'a',j+'a');
                hasMoves = true;
            }

            //else printf("swag\n");
        }
    }
    return hasMoves;
}

bool validMove(char board [26][26], int size, char color, char row, char col) {
    bool u, d, l, r, ur, ul, dr, dl;
    dl = checkLegalInDirection(board, size, color, row, col, -1, -1);
    d = checkLegalInDirection(board, size, color, row, col, -1, 0);
    dr = checkLegalInDirection(board, size, color, row, col, -1, 1);
    l = checkLegalInDirection(board, size, color, row, col, 0, -1);
    r = checkLegalInDirection(board, size, color, row, col, 0, 1);
    ul = checkLegalInDirection(board, size, color, row, col, 1, -1);
    u = checkLegalInDirection(board, size, color, row, col, 1, 0);
    ur = checkLegalInDirection(board, size, color, row, col, 1, 1);
    if (dl || d || dr || l || r || ul || u || ur)
        return true;
    else
        return false;
}

void playMove(char board [26][26], int size, char color, char row, char col) {
    bool u, d, l, r, ur, ul, dr, dl;
    int i;

    dl = checkLegalInDirection(board, size, color, row, col, -1, -1);
    d = checkLegalInDirection(board, size, color, row, col, -1, 0);
    dr = checkLegalInDirection(board, size, color, row, col, -1, 1);
    l = checkLegalInDirection(board, size, color, row, col, 0, -1);
    r = checkLegalInDirection(board, size, color, row, col, 0, 1);
    ul = checkLegalInDirection(board, size, color, row, col, 1, -1);
    u = checkLegalInDirection(board, size, color, row, col, 1, 0);
    ur = checkLegalInDirection(board, size, color, row, col, 1, 1);

    row = charToIndex(row);
    col = charToIndex(col);

    if (dl) {
        i = 0;
        while (board[row - (1 * i)][col - (1 * i)] != color || i == 0) {
            board[row - (1 * i)][col - (1 * i)] = color;
            i++;
        }
    }

    if (d) {
        i = 0;
        while (board[row - (1 * i)][col] != color || i == 0) {
            board[row - (1 * i)][col] = color;
            i++;
        }
    }

    if (dr) {
        i = 0;
        while (board[row - (1 * i)][col + (1 * i)] != color || i == 0) {
            board[row - (1 * i)][col + (1 * i)] = color;
            i++;
        }
    }

    if (l) {
        i = 0;
        while (board[row][col - (1 * i)] != color || i == 0) {
            board[row][col - (1 * i)] = color;
            i++;
        }
    }

    if (r) {
        i = 0;
        while (board[row][col + (1 * i)] != color || i == 0) {
            board[row][col + (1 * i)] = color;
            i++;
        }
    }

    if (ul) {
        i = 0;
        while (board[row + (1 * i)][col - (1 * i)] != color || i == 0) {
            board[row + (1 * i)][col - (1 * i)] = color;
            i++;
        }
    }

    if (u) {
        i = 0;
        while (board[row + (1 * i)][col] != color || i == 0) {
            board[row + (1 * i)][col] = color;
            i++;
        }
    }

    if (ur) {
        i = 0;
        ;
        while (board[row + (1 * i)][col + (1 * i)] != color || i == 0) {
            board[row + (1 * i)][col + (1 * i)] = color;
            i++;
        }
    }
}

void computerMove(char board [26][26], int size, char color) {
    int i,j, indexRow, indexCol;
    double highest = -2000;
    for (i = 0; i < size; i++){
        for (j = 0; j < size; j++){
            if (validMove(board, size, color, i + 'a', j + 'a')){
                if (potValue(board, size, color, i + 'a', j + 'a', 0) + priority(board, size, color, i + 'a', j + 'a') > highest){
                    highest = potValue(board, size, color, i + 'a', j + 'a', 0) + priority(board, size, color, i + 'a', j + 'a');
                    indexRow = i;
                    indexCol = j;
                }
            }
                
        }
    }
    playMove(board, size, color, indexRow + 'a', indexCol + 'a');
    printf("Computer places %c at %c%c\n", color, indexRow + 'a', indexCol + 'a');

}

int valueMove(char board[26][26], int size, char color, int rowIndex, int colIndex) {

    char temp [26][26];
    int i, j, initVal = 0, newVal = 0, value;

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            temp[i][j] = board[i][j];
        }
    }
    playMove(temp, size, color, rowIndex + 'a', colIndex + 'a');
    initVal = valueBoard(board, size, color);
    newVal = valueBoard(temp, size, color);
    value = newVal - initVal;
    return value;

}

int valueBoard(char board [26][26], int size, char color) {
    int i, j, value = 0;

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (board[i][j] == color)
                value++;
        }
    }
    return value;
}

double priority(char board [26][26], int size, char color, char row, char col){
    int i,j, cornerValue = 10, edgeValue = 8, cornerAdj = -3, edgeAdj = -1, normal = 0, cornerDecay = 4, edgeDecay = 3;
    double cornerDenom = 52.0, edgeDenom = 48.0;
    char temp [26][26];
    bool adj = false;
    
    for (i = 0; i <size; i++){
        for (j = 0; j < size; j++){
            temp[i][j] = board[i][j];
        }
    }
    playMove(temp, size, color, row, col);
    
    if (isCorner(size, row, col))
        return cornerValue - (cornerDecay * gameLength(board,size)/cornerDenom);
    else if (isAdjCorner(size, row, col)){
        return cornerAdj;
    }
    else if (isEdge(size, row, col)){
        if (capAdjEdge(temp, size, color, row, col))
            return 2.0*edgeAdj;
        return edgeValue - (edgeDecay * gameLength(board,size)/edgeDenom);
    }
    else if (isAdjEdge(size, row, col)){
        return edgeAdj;
    }
    else 
        return normal;
}

int gameLength(char board[26][26], int size){
    int i, j, counter = 0;
    for (i = 0; i < size; i++){
        for (j = 0; j < size; j++){
            if (board[i][j] != 'U')
                counter ++;
        }
    }
    return counter;
}

int potValue(char board [26][26], int size, char color, char row, char col, int moves){
    int value = valueMove(board, size, color, charToIndex(row), charToIndex(col));
    int i, j, counter, highest = INT_MIN;
    char temp [26][26], curColor = color;
    
    for (i = 0; i < size; i++){
        for (j = 0; j < size; j ++){
            temp[i][j] = board[i][j];
        }
    }
    
   playMove(temp, size, curColor, row, col);
   curColor = oppositeColor(curColor);
   for (counter = 0; counter < moves; counter++){
        for (i = 0; i < size; i++){
            for (j = 0; j < size; j ++){
                if(highest < valueMove(temp, size, curColor, i,j))
                highest = valueMove(temp, size, curColor, i,j);
            }
        }
        if (curColor != color)
            value -= highest;
        else 
            value += highest;
        highest = INT_MIN;
        curColor = oppositeColor(curColor);
   }
   return value;
}

double valueMovePosition(char board[26][26], int size, char color, char row, char col){
    
}
bool isCorner(int size, char row, char col){
    return (row == col && (charToIndex(row) == 'a' || charToIndex(row) == size-1)) || (charToIndex(row) == 0 && charToIndex(col) == size -1) ||(charToIndex(col) == 0 && charToIndex(row) == size -1);
}

bool isAdjCorner(int size, char row, char col){
    int i, j;
    for (i = -1; i <= 1; i++){
        for (j = -1; j <= 1; j++){
            if (i != 0 && j!= 0 && isCorner(size, row + i, col + j))
                return true;
                
        }
    }
    return false;
}

bool isEdge(int size, char row, char col){
    return (charToIndex(row) == 0 ||charToIndex(row) == size - 1 ||charToIndex(col) == 0 ||charToIndex(col) == size -1 );
}

bool isAdjEdge(int size, char row, char col){
    int i, j;
    for (i = -1; i <= 1; i++){
        for (j = -1; j <= 1; j++){
            if (i != 0 && j!= 0 && isEdge(size, row + i, col + j))
                return true;
                
        }
    }
    return false;
}

char oppositeColor(char color){
    if (color == 'B')
        return 'W';
    else 
        return 'B';
}

bool capAdjEdge(char board [26][26], int size, char color, char row, char col){
    int i, j;
    for (i = -1; i <= 1; i++){
        for (j = -1; j <= 1; j++){
            if (validMove(board, size, oppositeColor(color), row + i, col + j))
                return true;
                
        }
    }
    return false;
}