#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sudoku_io.c"

sudoku p;
int n;
int n_sqr;
int *array;

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
}

void main(int argc, char** argv) {
    p.n = readSudokuFromFile(argv[1]);
    p.ar = oneDtoTwoD();
    free(array);
    for (int i = 0; i <  n_sqr; i++) {
        for (int j = 0; j < n_sqr; j++) {
            if (p.ar[i][j] > 9) {
                printf(" %d", p.ar[i][j]);
            }
            else {
                printf("  %d", p.ar[i][j]);
            }
        }
        printf("\n");
    }
    check result = check_sudoku(p.ar, p.n);
    if (result == 0) {
        printf("INVALID\n");
    }
    if (result == 1) {
        printf("INCOMPLETE\n");
    }
    if (result == 2) {
        printf("COMPLETE\n");
    }
}