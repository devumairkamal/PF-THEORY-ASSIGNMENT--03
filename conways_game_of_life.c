#include <stdio.h>
#include <stdlib.h>

int **createGrid(int n) {
    int **grid = (int **)malloc(n * sizeof(int *));
    if (grid == NULL) return NULL;

    for (int i = 0; i < n; i++) {
        grid[i] = (int *)malloc(n * sizeof(int));
        if (grid[i] == NULL) {
            for (int j = 0; j < i; j++) free(grid[j]);
            free(grid);
            return NULL;
        }
    }
    return grid;
}

void freeGrid(int **grid, int n) {
    if (grid == NULL) return;
    for (int i = 0; i < n; i++) free(grid[i]);
    free(grid);
}

int countLiveNeighbors(int **grid, int n, int row, int col) {
    int count = 0;

    for (int r = row - 1; r <= row + 1; r++) {
        for (int c = col - 1; c <= col + 1; c++) {
            if (r == row && c == col) continue;

            if (r >= 0 && r < n && c >= 0 && c < n) {
                if (grid[r][c] == 1) count++;
            }
        }
    }
    return count;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Error: Please provide the input filename.\n");
        printf("Usage: %s input.txt\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error: Could not open file '%s'.\n", argv[1]);
        printf("Make sure the file exists in the program's working directory.\n");
        return 1;
    }

    int n, generations;
    if (fscanf(file, "%d", &n) != 1 || fscanf(file, "%d", &generations) != 1 || n <= 0 || generations < 0) {
        printf("Error: Invalid N or generation count in input file.\n");
        fclose(file);
        return 1;
    }

    int **curr_grid = createGrid(n);
    int **next_grid = createGrid(n);

    if (curr_grid == NULL || next_grid == NULL) {
        printf("Error: Memory allocation failed.\n");
        freeGrid(curr_grid, n);
        freeGrid(next_grid, n);
        fclose(file);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (fscanf(file, "%d", &curr_grid[i][j]) != 1 ||
                (curr_grid[i][j] != 0 && curr_grid[i][j] != 1)) {
                printf("Error: Invalid or missing grid value at row %d, column %d.\n", i + 1, j + 1);
                freeGrid(curr_grid, n);
                freeGrid(next_grid, n);
                fclose(file);
                return 1;
            }
        }
    }
    fclose(file);

    for (int gen = 0; gen < generations; gen++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int neighbors = countLiveNeighbors(curr_grid, n, i, j);

                if (curr_grid[i][j] == 1) {
                    next_grid[i][j] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
                } else {
                    next_grid[i][j] = (neighbors == 3) ? 1 : 0;
                }
            }
        }

        int **temp = curr_grid;
        curr_grid = next_grid;
        next_grid = temp;
    }

    printf("Final grid after %d generations:\n", generations);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", curr_grid[i][j]);
        }
        printf("\n");
    }

    freeGrid(curr_grid, n);
    freeGrid(next_grid, n);
    return 0;
}
