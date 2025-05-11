#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>

typedef struct {
    int rows;
    int cols;
    int** data;
} Matrix;

typedef struct {
    Matrix* a;
    Matrix* b;
    Matrix* c;
    int row;
} RowThreadArgs;

Matrix read_matrix(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    Matrix mat;
    if (fscanf(file, "row=%d col=%d", &mat.rows, &mat.cols) != 2) {
        fprintf(stderr, "Invalid file format\n");
        exit(EXIT_FAILURE);
    }

    mat.data = (int**)malloc(mat.rows * sizeof(int*));
    for (int i = 0; i < mat.rows; i++) {
        mat.data[i] = (int*)malloc(mat.cols * sizeof(int));
        for (int j = 0; j < mat.cols; j++) {
            if (fscanf(file, "%d", &mat.data[i][j]) != 1) {
                fprintf(stderr, "Invalid matrix data\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(file);
    return mat;
}

void write_matrix(const char* filename, Matrix mat) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "row=%d col=%d\n", mat.rows, mat.cols);
    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            fprintf(file, "%d ", mat.data[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void free_matrix(Matrix mat) {
    for (int i = 0; i < mat.rows; i++) {
        free(mat.data[i]);
    }
    free(mat.data);
}

unsigned __stdcall compute_row(void* args) {
    RowThreadArgs* rta = (RowThreadArgs*)args;
    int row = rta->row;

    for (int j = 0; j < rta->b->cols; j++) {
        rta->c->data[row][j] = 0;
        for (int k = 0; k < rta->a->cols; k++) {
            rta->c->data[row][j] += rta->a->data[row][k] * rta->b->data[k][j];
        }
    }

    free(rta);
    return 0;
}

Matrix multiply_matrices_per_row(Matrix a, Matrix b) {
    if (a.cols != b.rows) {
        fprintf(stderr, "Matrix dimensions incompatible for multiplication\n");
        exit(EXIT_FAILURE);
    }

    Matrix result;
    result.rows = a.rows;
    result.cols = b.cols;
    result.data = (int**)malloc(result.rows * sizeof(int*));
    for (int i = 0; i < result.rows; i++) {
        result.data[i] = (int*)malloc(result.cols * sizeof(int));
    }

    HANDLE* threads = (HANDLE*)malloc(result.rows * sizeof(HANDLE));

    for (int i = 0; i < result.rows; i++) {
        RowThreadArgs* args = malloc(sizeof(RowThreadArgs));
        args->a = &a;
        args->b = &b;
        args->c = &result;
        args->row = i;

        threads[i] = (HANDLE)_beginthreadex(NULL, 0, compute_row, args, 0, NULL);
    }

    for (int i = 0; i < result.rows; i++) {
        WaitForSingleObject(threads[i], INFINITE);
        CloseHandle(threads[i]);
    }

    free(threads);
    return result;
}

int main(int argc, char* argv[]) {
    const char* mat1_filename = (argc > 1) ? argv[1] : "a";
    const char* mat2_filename = (argc > 2) ? argv[2] : "b";
    const char* output_prefix = (argc > 3) ? argv[3] : "c";

    char mat1_path[100], mat2_path[100], output_path[100];
    sprintf(mat1_path, "%s.txt", mat1_filename);
    sprintf(mat2_path, "%s.txt", mat2_filename);
    sprintf(output_path, "%s_per_row.txt", output_prefix);

    LARGE_INTEGER start, end, freq;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    Matrix a = read_matrix(mat1_path);
    Matrix b = read_matrix(mat2_path);
    Matrix c = multiply_matrices_per_row(a, b);
    write_matrix(output_path, c);

    QueryPerformanceCounter(&end);

    double time_taken = (double)(end.QuadPart - start.QuadPart) * 1000000 / freq.QuadPart;

    printf("Method: Per Row\n");
    printf("Threads created: %d\n", c.rows);
    printf("Time taken: %.2f microseconds\n", time_taken);

    free_matrix(a);
    free_matrix(b);
    free_matrix(c);

    return 0;
}
