#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct sudoku {
    int n;
    int **ar;
} sudoku;

typedef enum checkReturn { INVALID, INCOMPLETE, COMPLETE } check;

check check_list(int *ar, int length) { //counter for repeating values, check for 0
    bool zero_found = false;
    int count;
    for (int nums = 1; nums <= length; nums++) { //for each possible number in sudoku
        count = 0;
        for (int k = 0; k < length; k++) {
            if (ar[k] == 0) { //checks to see if incomplete
                zero_found = true;
            }
            if (nums == ar[k]) {
                count++; //counts how many times number is found
            }
        }
        if (count > 1) { //if number is repeated 
            break; //break out of loop and return invalid
        }
    }
    if (count > 1) return INVALID;
    if (zero_found) return INCOMPLETE;
    else return COMPLETE;
}

int* makeCol(int col, int n, int** ar) { // makes column for check-list
    int *array = malloc(sizeof(int)*n*n);
    for (int row = 0; row < n*n; row++) {
        array[row] = ar[row][col];
    }
    return array;
    free(array);
}

int* makeBox(int rowStart, int colStart, int n, int** ar) { //deal with memory
    int row, col, rowEnd = rowStart + n, colEnd = colStart + n, count = 0;
    int *array = malloc(sizeof(int)*n*n);
    for (row = rowStart; row < rowEnd; row++) {
        for (col = colStart; col < colEnd; col++) {
            array[count] = ar[row][col];
            count++;
        }
    }
    return array;
    free(array);
}

check check_sudoku(int** ar, int n) {
    int length = n * n; //rbc = rows boxes cols
    bool incomplete = false;
    check ans, ans2;
    for (int row = 0; row < length; row++) {
        ans = check_list(ar[row], length); // checks rows
        ans2 = check_list(makeCol(row, n, ar), length); //checks cols using row count as col count
        if (ans == INVALID || ans2 == INVALID) {
            return INVALID;
        }
        if (ans == INCOMPLETE || ans2 == INCOMPLETE) {
            incomplete = true;
        }
    }
    for (int getRow = 0; getRow < length; getRow += n) { //checks boxes
        for (int getCol = 0; getCol < length; getCol += n) {
            ans = check_list(makeBox(getRow, getCol, n, ar), length);
            if (ans == INVALID) {
                return INVALID;
            }
        }
    }
    if (incomplete) {
        return INCOMPLETE;
    }
    else {
        return COMPLETE;
    }
}