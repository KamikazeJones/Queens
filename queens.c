#include <stdio.h>
#include <stdlib.h>



int *columns;
int colCount;


/*

Für jede Spalte gibt es 

1. die Position der Dame, falls sie in der Spalte steht
2. die Felder in der Spalte, auf denen noch eine Dame platziert werden kann
3. die Anzahl der Felder in der Spalte, auf denen noch eine Dame platziert werden darf

Ist in einer Spalte noch keine Dame platziert, aber auch keine Dame mehr platzierbar, 
so wird per Backtracking die letzte Damenplatzierung rückgängig gemacht.
Es wird dann mit der nächsten Platzierung fortgefahren.



-1 -1 -1 -1 -1
-1 -1 -1 -1 -1
-1 -1 -1 -1 -1
-1 -1 -1 -1 -1



*/

int placeQueen(int row, int col) {
    columns[col] = row;
}

int takeBack(int col) {
    columns[col] = 0;
}

void *queens(int n) {
    columns = (int *)malloc((n+1) * sizeof(int));
    // backtracking(1,n+1);
    return columns;
}
/*
  1 2 3 4 5 6      1 2 3 4 5 6
1 0  -2          1 2 3 4
2 1 0  -2        2 3 4 
3 2 1 0          3 4 
4   2 1 0        4 
5     2 1 0      5 
6       2 1 0    6

*/

int isSafe(int row, int col) {
    for (int i = 1; i < col; i++) {
        if (columns[i] == row) {
            return 0;
        }
        if (columns[i] - i == row - col) {
            return 0;
        }
        if (columns[i] + i == row + col) {
            return 0;
        }
    }
    return 1;
}

int naiveBacktracking(int col) {
    if (col == colCount+1) {
        return 1;
    }
    for (int i = 1; i < colCount+1; i++) {
        if (isSafe(i, col)) {
            columns[col] = i;
            if (naiveBacktracking(col+1)) {
                return 1;
            }
            else { 
                columns[col] = 0;
            }
        }
    }
    return 0;
}

void naiveQueens(int n) {
    
    colCount = n;
    columns = (int *)malloc((colCount+1) * sizeof(int));

    
    for(int i = 0; i < colCount+1; i++) {
        columns[i] = 0;
    }
    if(naiveBacktracking(1)) {
        printf("Solution found\n");
        for (int i = 1; i < n+1; i++) {
            printf("%d ", columns[i]);
        }
        printf("\n");
    }
    else {
        printf("No solution found\n");
    }
}


int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    naiveQueens(n);
    return 0;
}