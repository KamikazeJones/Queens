#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int *columns;
int colCount;
int solCount=1;
int solutionNumber = 0;
/*

Die Differenz (row - col) ist für Zellen auf der gleichen Diagonalen 
(von links oben nach rechts unten) gleich.

Die Summe (row + col) ist für Zellen auf der gleichen Diagonalen 
(von Links unten nach rechts oben) gleich.

  1 2 3 4 5 6      1 2 3 4 5 6
1 0  -2          1 2 3 4
2 1 0  -2        2 3 4 
3 2 1 0          3 4         9 
4   2 1 0        4         9
5     2 1 0      5       9
6       2 1 0    6     9  

*/

int isSafe(int row, int col) {
    for (int i = 1; i < col; i++) {
        if (columns[i] == row) {
            return 0;
        }
        // Test ob auf der gleichen Diagonalen (von links oben nach rechts unten) schon eine Dame steht
        if (columns[i] - i == row - col) {
            return 0;
        }
        // Test ob auf der gleichen Diagonalen (von links unten nach rechts oben) schon eine Dame steht
        if (columns[i] + i == row + col) {
            return 0;
        }
    }
    return 1;
}

int naiveBacktracking(int col) {
    if (col == colCount+1) {
        solutionNumber++;
        printf("Solution found %d:\n", solutionNumber);
        for (int i = 1; i < colCount+1; i++) {
            printf("%d ", columns[i]);
        }
        printf("\n");
        return --solCount == 0;
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
    naiveBacktracking(1); 
    printf("No more solutions found\n");
    free(columns);
}

int main(int argc, char *argv[]) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();
 
    if(argc < 2) {
        printf("Usage: %s <n>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    if(argc > 2) {
        solCount = atoi(argv[2]);
    }
    naiveQueens(n);
    end = clock();
    
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Laufzeit: %f Sekunden\n", cpu_time_used);    

    return 0;
}