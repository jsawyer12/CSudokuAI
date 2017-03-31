#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sudoku_io_ext.c"

sudoku p;
int n, solCount, n_sqr;
int *array;
int **solution;
bool completed;
check status;

int readSudokuFromFile(char* file) {
    FILE *fp;
    int num, it = 0;
    fp = fopen(file , "r");
    if(fp == NULL) {
        perror("Error opening file");
    }
    array = malloc(sizeof(int));
    while (!feof(fp)) {
        fscanf(fp, "%d", &num);
        if (n == 0) {
            n = num; //sets n as first read integer
            n_sqr = n * n;
            free(array);
            array = malloc(sizeof(int)*n_sqr*n_sqr);
        }
        else {
            array[it] = num;        
            it++;
        }
    }
    fclose(fp);
    return n;
}

int sudoku_solver(int **oldAr) {
    int **newAr;
    status = check_sudoku(oldAr, p.n);
    if (status == COMPLETE) {
        if (!completed) {
            completed = true;
            solution = (int **)malloc(n_sqr * sizeof(int *));
            for (int i = 0; i < n_sqr; i++) {
                solution[i] = (int *)malloc(n_sqr * sizeof(int));
            }
            for (int i = 0; i <  n_sqr; i++) {
                for (int j = 0; j < n_sqr; j++) {
                    solution[i][j] = oldAr[i][j]; 
                }
            }   
            solCount++;
            return solCount;
        }
        else {
            bool newSolution = false;
            for (int i = 0; i <  n_sqr; i++) {
                for (int j = 0; j < n_sqr; j++) {
                    if (oldAr[i][j] != solution[i][j]) {
                        newSolution = true;
                    } 
                }
            }
            if (newSolution) solCount++;
        }
        return solCount;
    }
    else if (status == INVALID) {
        return solCount;
    }
    else if (status == INCOMPLETE) {
        for (int row = 0; row < n_sqr; row++) {
            for (int col = 0; col < n_sqr; col++) {
                if (oldAr[row][col] == 0) {
                    newAr = (int **)malloc(n_sqr * sizeof(int *));
                    for (int i = 0; i < n_sqr; i++) {
                        newAr[i] = (int *)malloc(n_sqr * sizeof(int));
                    }
                    for (int i = 0; i <  n_sqr; i++) {
                        for (int j = 0; j < n_sqr; j++) {
                            newAr[i][j] = oldAr[i][j]; 
                        }
                    }
                    bool foundLast = false;
                    int lastVal = getLastVal(oldAr[row], n_sqr); //checks for last value in row
                    if (lastVal > 0) {
                        newAr[row][col] = lastVal;
                        foundLast = true;
                    }
                    if (!foundLast) {
                        lastVal = getLastVal(makeCol(col, n, oldAr), n_sqr); //checks for last value in column
                        if (lastVal > 0) {
                            newAr[row][col] = lastVal;
                            foundLast = true;
                        }
                        else {
                            int getRow = row - (row % n); // finds first cell row in box 
                            int getCol = col - (col % n); // finds first cell col in box
                            lastVal = getLastVal(makeBox(getRow, getCol, n, oldAr), n_sqr); //checks for last val in box
                            if (lastVal > 0) {
                                newAr[row][col] = lastVal;
                                foundLast = true;
                            }
                        }
                    }
                    if (lastVal > 0) {
                        sudoku_solver(newAr);
                        if (status == COMPLETE) return solCount;
                    }
                    if (lastVal == 0) { // if more than one zero in row, box, or col
                        int *possibleValues = malloc(sizeof(int)*n_sqr);
                        possibleValues = getOtherVals(oldAr[row], n_sqr);
                        for (int k = 1; k <= n_sqr; k++) {
                            if (possibleValues[k] != 0) {
                                newAr[row][col] = possibleValues[k];
                                sudoku_solver(newAr);
                                if (status == COMPLETE) return solCount;  
                            }
                        }
                        free(possibleValues);
                    }                        
                }
            }
        }
    }
    free(newAr);
}

int** oneDtoTwoD() {
    int j, i, count = 0;
    int **arr = (int **)malloc(n_sqr * sizeof(int *));
    for (i = 0; i < n_sqr; i++) {
         arr[i] = (int *)malloc(n_sqr * sizeof(int));
    }
    for (i = 0; i <  n_sqr; i++) {
        for (j = 0; j < n_sqr; j++) {
            arr[i][j] = array[count];
            count++;
        }
    }
    return arr;
    free(arr);
}

int main(int argc, char  *argv[]) {
    p.n = readSudokuFromFile(argv[1]);
    p.ar = oneDtoTwoD();
    free(array);
    completed = false;
    solCount = 0;
    status = INCOMPLETE;
    sudoku_solver(p.ar);
    if (solCount == 0) printf("UNSOLVABLE\n");
    if (solCount == 1) {
        for (int i = 0; i <  n_sqr; i++) {
            for (int j = 0; j < n_sqr; j++) {
                if (solution[i][j] > 9) {
                    printf(" %d", solution[i][j]);
                }
                else {
                    printf("  %d", solution[i][j]);
                }
            }
            printf("\n");
        }
    }
    if (solCount > 1) {
        printf("MULTIPLE\n");
    }
    free(solution);
    free(p.ar);
    return 0;
}