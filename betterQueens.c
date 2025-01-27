#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <limits.h>

// #define DEBUG

// generierte Bilder unter:
//    https://imgur.com/a/yRSSDjU
//    https://imgur.com/a/1024-queens-haeaxHx
// 
// besonders schön: 
//    https://i.imgur.com/YhgrzDa.png
//    https://i.imgur.com/Rhgdfce.jpeg


int isBitmaskSet(int col, int row);

int *columns;
int colCount;
int solCount=1;
int solutionNumber = 0;

int *freeFieldsCount;
int *_freeFieldsBitmask;
int **attackBitMap;

int **freeColumns;

int doTests = 0;

int try = 0;
int minDepth=INT_MAX;
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


Für jede Spalte werden folgende Informationen gespeichert:
1. Die Zeile in der die Dame steht
2. Die Anzahl der freien Felder in der Spalte
3. Die freien Felder in der Spalte (als Bitmaske)

Bei jeder Rekursion erstelle ich eine Liste der freien Spalten, sortiert
nach der Anzahl der freien Felder (aufsteigend).
Beim Backtracking wird dann aus den freien Spalten der Reihe nach 
eine ausgewählt.

Sind alle Damen platziert, wird die Lösung ausgegeben.

*/

/* in arr[0] wird die Anzahl der Elemente im Array gespeichert */

int cb(int *a, int idx, int line) {
    if(idx >= 0 && idx <= colCount) {
        return a[idx];
    }
    else {
        printf("line %d: Index %d out of range!\n", line, idx);
        exit(1);
    }
}

int *cba(int **a, int idx, int line) {
    if(idx >= 0 && idx <= colCount) {
        return a[idx];
    }
    else {
        printf("line %d: Index %d out of range!\n", line, idx);
        exit(1);
    }
}
void printBoard() {
    for(int c=1; c<=colCount; c++) {
        if(columns[c] == 0) {
            printf(". ");
        }
        else {
            printf("W ");
        }
    }
    printf("\n\n");

    int bm;
    for(int r=1; r<=colCount; r++) {
        for(int c=1; c<=colCount; c++) {
            if(columns[c] == r) {
                printf("# ");
            }
            else if (bm = isBitmaskSet(c, r)) {
                printf("%d ", bm % 10);               
            }
            else {
                printf(". ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void printFreeColumns(int *freeCols) {
        for(int i=1; i<=freeCols[0]; i++) {
            cb(freeCols, i, __LINE__);
            printf("freeCols[%d]\n", freeCols[i]);
        }
        printf("\n");
}

int insert(int *arr, int col) {
    int i = 1;
    int n = arr[0]+1;
    int key;

    cb(freeFieldsCount, col, __LINE__);
    key = freeFieldsCount[col];

#ifdef DEBUG
    printf("in insert: col=%d, key=%d\n", col, key);   
#endif 

    if(key == 0) { // keine freien Felder mehr in der Spalte
#ifdef DEBUG
        printf("Keine freien Felder mehr in Spalte %d, Anzahl Felder: %d\n", col, key);
#endif
        return -1;
    }
    if (key < 0) { // In der Spalte steht schon eine Dame
        printf("ERROR: Anzahl freie Felder in der Spalte %d ist: %d\n", col, key);
        return n;
    }
    
    while (i < n && freeFieldsCount[arr[i]] < key) {
        i++;
    }

    while(i<n && freeFieldsCount[arr[i]] == key) {
        if(rand() % 3 == 0) {
            break;
        }
        i++;
    }

    for (int j = n; j > i; j--) {
        cb(arr, j, __LINE__);
        cb(arr, j-1, __LINE__);
        arr[j] = arr[j-1];
    }
    cb(arr, i, __LINE__);
    arr[i] = col;
    arr[0] = n;
    return n;
}

int isBitmaskSet(int col, int row) {
    int *bms = attackBitMap[col];

    return(bms[row]);
}

void printBitmask(int col) {
    int *bms = attackBitMap[col];
    for(int i=0; i<colCount; i++) {
        printf("%d ", bms[i]);
    }
    printf("\n");
}

int setBitmask(int col, int row) {
    cba(attackBitMap, col, __LINE__);
    int *bms = attackBitMap[col];

    cb(bms, row, __LINE__);
    if(bms[row] == 0) {
        cb(freeFieldsCount, col, __LINE__);
        freeFieldsCount[col] = freeFieldsCount[col] - 1;
    }
    bms[row] += 1;
    
    return freeFieldsCount[col];
}

int decrBitmask(int col, int row) {
    cba(attackBitMap, col, __LINE__);
    int *bms = attackBitMap[col];

    cb(bms, row, __LINE__);
    bms[row] -= 1;
    if(bms[row] == 0) {
        cb(freeFieldsCount, col, __LINE__);
        freeFieldsCount[col] = freeFieldsCount[col] + 1;
    }
    
    return freeFieldsCount[col];
}



void setColumn(int col, int row, int *freeCols) {

    columns[col] = row;
    // und jetzt noch die Contraints aktualisieren
    
    int faktor = 1;
    
    for(int i=1; i<=freeCols[0]; i++) {
        cb(freeCols, i, __LINE__);
        int freeCol = freeCols[i];
        if(freeCol == col) {
            continue;
        }

        // Die gleiche Reihe ist nicht mehr frei
        setBitmask(freeCol, row);

        // markiere in den freien Spalten die Felder auf den Diagonalen als bedroht
        if(freeCol < col) {
            faktor = -1;
        }
        int diff = freeCol - col;
        int r = row + faktor * diff;
        if(r > 0 && r <= colCount) {
            setBitmask(freeCol, r);
        }
        r = row - faktor * diff;
        if(r > 0 && r <= colCount) {
            setBitmask(freeCol, r);
        } 
    }
}

void clearColumn(int col, int *freeCols) {

    int row = columns[col];
    columns[col] = 0;
    // und jetzt noch die Contraints aktualisieren
    
    int faktor = 1;
    
    for(int i=1; i<=freeCols[0]; i++) {
        cb(freeCols, i, __LINE__);
        int freeCol = freeCols[i];
        if(freeCol == col) {
            continue;
        }

        // Die gleiche Reihe ist nicht mehr frei
        decrBitmask(freeCol, row);

        // markiere in den freien Spalten die Felder auf den Diagonalen als bedroht
        if(freeCol < col) {
            faktor = -1;
        }
        int diff = freeCol - col;
        int r = row + faktor * diff;
        if(r > 0 && r <= colCount) {
            decrBitmask(freeCol, r);
        }
        r = row - faktor * diff;
        if(r > 0 && r <= colCount) {
            decrBitmask(freeCol, r);
        } 
    }
}

void setNextFreeColumns(int col, int *freeCols, int *nextFreeCols) {
#ifdef DEBUG
    printf("in setNextFreeColumns: col=%d\n", col);
#endif
    nextFreeCols[0] = 0;
    for(int i=1; i<=freeCols[0]; i++) {
        cb(freeCols, i, __LINE__);
        int freeCol = freeCols[i];
        if(columns[freeCol] == 0) {
            insert(nextFreeCols, freeCol);
        }
        else {
            // printf("Spalte %d ist schon belegt\n", freeCol);
        }
    }
    // Ein bischen Zufall muss sein
    if(nextFreeCols[0] > 1 && (rand() % 2) == 0) {
        int tmp = nextFreeCols[1];
        nextFreeCols[1] = nextFreeCols[2];
        nextFreeCols[2] = tmp;
    }
}

void repchar(char ch, int count) {
    for (int i = 0; i < count; i++) {
        fprintf(stderr, "%c", ch);
    }
}

int intervall;
// clock_t intervall_start, intervall_end;
struct timespec intervall_start, intervall_end;

int constraintBacktracking(int depth) {

    
    try++;
    if(depth<minDepth) {
        minDepth = depth;
    }

    if(try % intervall == 0) {
        clock_gettime(CLOCK_MONOTONIC, &intervall_end);

        double intervall_time = (intervall_end.tv_sec - intervall_start.tv_sec) +
                                (intervall_end.tv_nsec - intervall_start.tv_nsec) / 1e9;
        double intervall_rate = intervall / intervall_time;
        int new_intervall = (((int) (intervall_rate)) + intervall) / 2;
        // maximal 10% Änderung
        if(new_intervall > intervall * 1.1) {
            intervall = intervall * 1.1;
        }
        else if(new_intervall < intervall * 0.9) {
            intervall = intervall * 0.9;
        }
        else {
            intervall = new_intervall;
        }

        clock_gettime(CLOCK_MONOTONIC, &intervall_start);
        fprintf(stderr, "\rtry=%9d", try);
        fflush(stderr);

        int val = colCount+1 - minDepth;

        div_t d2 = div(val, 25);
        div_t d1 = div(d2.quot, 25);

        repchar('#', d1.quot);
        repchar('>', d1.rem);
        repchar('.', d2.rem);
        repchar(' ', 80-(d1.quot+d1.rem+d2.rem));
        minDepth = colCount+2;    
        fflush(stderr); // Erzwingt die Ausgabe des Puffers
    }

#ifdef DEBUG
    printf("in constraintBacktracking: depth=%d\n", depth);
    fflush(stdout); // Erzwingt die Ausgabe des Puffers
#endif
    
    if (depth == colCount) {
        solutionNumber++;
        printBoard();
        printf("Solution found %d:\n", solutionNumber);
        for (int i = 1; i <= colCount; i++) {
            printf("%d ", columns[i]);
        }
        printf("\n");
        printf("try=%8d\n", try);
        fflush(stdout); // Erzwingt die Ausgabe des Puffers
        return --solCount == 0;
    }
    int *freeCols = freeColumns[depth];

#ifdef DEBUG
    printf("freeCols: depth=%d, freeCols[0]=%d\n", depth, freeCols[0]);
    fflush(stdout); // Erzwingt die Ausgabe des Puffers
#endif
    // Sind noch genug freie Spalten übrig?
    if(freeCols[0] + depth != colCount) {
#ifdef DEBUG        
        printf("Es gibt nicht mehr genug freie Spalten für eine Lösung!\n");
#endif
        return 0;
    }
    int col = freeCols[1];
    for(int row = 1; row <= colCount; row++) {
        if(isBitmaskSet(col, row)) {
            continue;
        }
#ifdef DEBUG  
        printf("vor setColumn depth=%d, col=%d, row=%d\n", depth, col, row);
        fflush(stdout); // Erzwingt die Ausgabe des Puffers
#endif
        setColumn(col, row, freeCols);
#ifdef DEBUG
        printf("nach setColumn depth=%d, col=%d, row=%d\n", depth, col, row);
        fflush(stdout); // Erzwingt die Ausgabe des Puffers
#endif
        if(depth < colCount) {
#ifdef DEBUG            
            printf("vor setNextFreeColumns depth=%d, col=%d\n", depth, col);
            fflush(stdout); // Erzwingt die Ausgabe des Puffers
            cba(freeColumns, depth+1, __LINE__);
#endif
            setNextFreeColumns(col, freeCols, freeColumns[depth+1]);
        }
#ifdef DEBUG             
        printf("nach setNextFreeColumns: depth=%d, col=%d, row=%d\n", depth, col, row);
        fflush(stdout); // Erzwingt die Ausgabe des Puffers

        printBoard();
#endif
        if (constraintBacktracking(depth+1)) {
            return 1;
        }
        else {                    
            clearColumn(col, freeCols);
#ifdef DEBUG                   
            printf("tracking back:\n");
            printBoard();
            fflush(stdout); // Erzwingt die Ausgabe des Puffers
#endif
        }
    }
    return 0;
}

void initialize() {

    for(int i = 0; i <= colCount; i++) {
        columns[i] = 0;
        freeFieldsCount[i] = colCount;
        for (int j = 0; j <= colCount; j++) {
            attackBitMap[i][j] = 0;
        }
        for (int j = 0; j <= colCount; j++) {
            freeColumns[i][j] = 0;
        }
    }

    for(int i = 1; i <= colCount; i++) {
        insert(freeColumns[0], i);
        insert(freeColumns[1], i);
    }
}

void testInsert() {
    printf("in testInsert\n");
    fflush(stdout); // Erzwingt die Ausgabe des Puffers
    initialize();

    freeColumns[1][0]=0;

    // freeFieldsCount[1]=2;
    // freeFieldsCount[2]=2;
    // freeFieldsCount[3]=5;
    // freeFieldsCount[4]=4;
    // freeFieldsCount[5]=3;

    for(int i=1; i<6; i++) {
        printf("testInsert for i=%d\n", i);
        insert(freeColumns[1], i);
        printFreeColumns(freeColumns[1]);
        printf("\n");
    }
    printFreeColumns(freeColumns[1]);
    
    printf("testInsert Ende\n");
}

void allocateFreeColumns() {
    int n = colCount+2;
    freeColumns = (int **)malloc(n * sizeof(int *));
    if (freeColumns == NULL) {
        fprintf(stderr, "Memory allocation failed for freeColumns\n");
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        freeColumns[i] = (int *)malloc(n * sizeof(int));
        if (freeColumns[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for freeColumns[%d]\n", i);
            exit(1);
        }
        for (int j = 0; j < n; j++) {
            freeColumns[i][j] = 0;
        }
    }
}

void init() {
    columns = (int *)malloc((colCount + 1) * sizeof(int));
    if (columns == NULL) {
        fprintf(stderr, "Memory allocation failed for columns\n");
        exit(1);
    }
    freeFieldsCount = (int *)malloc((colCount + 1) * sizeof(int));
    if (freeFieldsCount == NULL) {
        fprintf(stderr, "Memory allocation failed for freeFieldsCount\n");
        exit(1);
    }

    attackBitMap = (int **)malloc((colCount+1) * sizeof(int *));
    for (int i = 0; i < colCount+1; i++) {
        attackBitMap[i] = (int *)malloc((colCount+1) * sizeof(int));
        for (int j = 0; j <= colCount; j++) {
            attackBitMap[i][j] = 0;
        }
    }

    allocateFreeColumns();
    initialize();

    fflush(stdout); // Erzwingt die Ausgabe des Puffers
}

void freeMemory() {
    free(columns);
    free(freeFieldsCount);
    for (int i = 0; i < colCount+1; i++) {
        free(attackBitMap[i]);
    }
    free(attackBitMap);
    for (int i = 0; i < colCount+1; i++) {
        free(freeColumns[i]);
    }
    free(freeColumns);
}

void constraintQueens(int n) {
    
    colCount = n;
    init();
    intervall = 10;
    clock_gettime(CLOCK_MONOTONIC, &intervall_start);
    constraintBacktracking(0); 
    printf("No more solutions found\n");

    if(doTests) {
        initialize();
        printf("Teste setColum und freeColumn\n");
        printf("Bitmask(1): ");
        printBitmask(1);

        printFreeColumns(freeColumns[1]);
        fflush(stdout); // Erzwingt die Ausgabe des Puffers

        setColumn(1, 5, freeColumns[1]);
        setNextFreeColumns(1, freeColumns[1], freeColumns[2]);
        printf("nach setColumn(1,5, ...):\n");
        for(int i=1; i<=freeColumns[2][0]; i++) {
            cb(freeColumns[2], i, __LINE__);
            printf("getBitmask(%d): ", freeColumns[2][i]);
            printBitmask(freeColumns[2][i]);
        }
        fflush(stdout); // Erzwingt die Ausgabe des Puffers

        setColumn(2, 3, freeColumns[2]);
        printf("nach setColumn(2,3, ...):\n");       
        setNextFreeColumns(2, freeColumns[2], freeColumns[3]);
        for(int i=1; i<=freeColumns[3][0]; i++) {
            cb(freeColumns[3], i, __LINE__);
            printf("getBitmask(%d): ", freeColumns[3][i]);
            printBitmask(freeColumns[3][i]);

        }
        clearColumn(2, freeColumns[2]);
        printf("nach freeColumn(2, ...):\n");
        for(int i=1; i<=freeColumns[2][0]; i++) {
            cb(freeColumns[2], i, __LINE__);
            printf("getBitmask(%d): ", freeColumns[2][i]);
            printBitmask(freeColumns[2][i]);
        }
        clearColumn(1, freeColumns[1]);
        printf("nach freeColumn(1, ...):\n");
        for(int i=1; i<=freeColumns[1][0]; i++) {
            cb(freeColumns[1], i, __LINE__);
            printf("getBitmask(%d): ", freeColumns[1][i]);
            printBitmask(freeColumns[1][i]);

        }
        printf("----------------\n");
        fflush(stdout); // Erzwingt die Ausgabe des Puffers

        initialize();

        printf("Bitmap Test\n");
        freeFieldsCount[1] = 8;
        setBitmask(1, 1);
        setBitmask(1, 2);
        setBitmask(1, 7);
        printf("getBitmask(1): ");
        printBitmask(1);
        printf("freeFieldsCount[1]: %d\n", freeFieldsCount[1]);
        fflush(stdout); // Erzwingt die Ausgabe des Puffers

        testInsert();
    }
    freeMemory();
}

int main(int argc, char *argv[]) {
    clock_t start, end;
    double cpu_time_used;

    srand(time(NULL));

    printf("main: Constraint Backtracking Queens\n");
    fflush(stdout); // Erzwingt die Ausgabe des Puffers

    start = clock();
 
    if(argc < 2) {
        printf("Usage: %s <n>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    if(argc > 2) {
        solCount = atoi(argv[2]);
    }
    if(argc > 3) {
        doTests = (strcmp(argv[3],"--test") == 0);
        printf("doTests: %d\n", doTests);
    }

    constraintQueens(n);
    end = clock();
    
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Laufzeit: %f Sekunden\n", cpu_time_used);    

    return 0;
}