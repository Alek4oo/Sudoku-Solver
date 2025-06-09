#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int board[9][9];

int read_ss_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return 0;
    }

    char line[100];
    int row = 0;
    while (fgets(line, sizeof(line), file) && row < 9) {
        for (int col = 0; col < 9; col++) {
            if (isdigit(line[col]))
                board[row][col] = line[col] - '0';
            else
                board[row][col] = 0;
        }
        row++;
    }

    fclose(file);
    return 1;
}

int write_ss_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error writing to file: %s\n", filename);
        return 0;
    }

    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++)
            fprintf(file, "%d", board[row][col]);
        fprintf(file, "\n");
    }

    fclose(file);
    return 1;
}

int is_valid(int row, int col, int num) {
    for (int i = 0; i < 9; i++) {
        if (board[row][i] == num || board[i][col] == num)
            return 0;
    }

    int boxRow = (row / 3) * 3;
    int boxCol = (col / 3) * 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[boxRow + i][boxCol + j] == num)
                return 0;

    return 1;
}

int solve() {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (board[row][col] == 0) {
                for (int num = 1; num <= 9; num++) {
                    if (is_valid(row, col, num)) {
                        board[row][col] = num;
                        if (solve())
                            return 1;
                        board[row][col] = 0;
                    }
                }
                return 0;
            }
        }
    }
    return 1;
}

const char *get_extension(const char *filename) {
    int len = strlen(filename);
    for (int i = len - 1; i >= 0; i--) {
        if (filename[i] == '.') {
            return &filename[i + 1];
        }
    }
    return "";
}

void build_output_filename(const char *input, char *output, const char *ext) {
    int i = 0;
    while (input[i] != '\0' && input[i] != '.') {
        output[i] = input[i];
        i++;
    }
    output[i] = '\0';
    strcat(output, "_solved.");
    strcat(output, ext);
}

int main() {
    char input_file[100];
    printf("Enter the path to the Sudoku file (.ss): ");
    scanf("%s", input_file);

    const char *ext = get_extension(input_file);
    int success = 0;

    if (strcmp(ext, "ss") == 0) {
        success = read_ss_file(input_file);
    } else {
        printf("Only .ss format is supported for now.\n");
        return 1;
    }

    if (!success) {
        printf("Failed to load the input file.\n");
        return 1;
    }

    if (solve()) {
        printf("Sudoku solved successfully!\n");

        char output_file[120];
        build_output_filename(input_file, output_file, ext);

        if (strcmp(ext, "ss") == 0)
            write_ss_file(output_file);

        printf("Solution saved to: %s\n", output_file);
    } else {
        printf("No solution exists for this Sudoku.\n");
    }

    return 0;
}
