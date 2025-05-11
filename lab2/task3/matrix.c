#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct {
    int rows;
    int cols;
    int** data;
} Matrix;

typedef struct {
    const Matrix* a;
    const Matrix* b;
    Matrix* c;
    int row;
    int col;
} ThreadData;

Matrix read_matrix(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    Matrix mat;
    char label[10];
    fscanf(file, "%[^=]=%d %[^=]=%d", label, &mat.rows, label, &mat.cols);

    mat.data = (int**)malloc(mat.rows * sizeof(int*));
    for (int i = 0; i < mat.rows; i++) {
        mat.data[i] = (int*)malloc(mat.cols * sizeof(int));
        for (int j = 0; j < mat.cols; j++) {
            fscanf(file, "%d", &mat.data[i][j]);
        }
    }

    fclose(file);
    return mat;
}

void write_matrix(const char* filename, const Matrix* mat) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fprintf(file, "row=%d col=%d\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            fprintf(file, "%d ", mat->data[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void* compute_element(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int sum = 0;
    for (int k = 0; k < data->a->cols; k++) {
        sum += data->a->data[data->row][k] * data->b->data[k][data->col];
    }
    data->c->data[data->row][data->col] = sum;
    free(data);
    return NULL;
}

Matrix multiply_matrices_per_element(const Matrix* a, const Matrix* b) {
    if (a->cols != b->rows) {
        fprintf(stderr, "Matrix dimensions incompatible for multiplication\n");
        exit(EXIT_FAILURE);
    }

    Matrix result;
    result.rows = a->rows;
    result.cols = b->cols;

    result.data = (int**)malloc(result.rows * sizeof(int*));
    for (int i = 0; i < result.rows; i++) {
        result.data[i] = (int*)calloc(result.cols, sizeof(int));
    }

    pthread_t* threads = (pthread_t*)malloc(result.rows * result.cols * sizeof(pthread_t));
    int thread_count = 0;

    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            ThreadData* data = (ThreadData*)malloc(sizeof(ThreadData));
            data->a = a;
            data->b = b;
            data->c = &result;
            data->row = i;
            data->col = j;
            pthread_create(&threads[thread_count++], NULL, compute_element, data);
        }
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    return result;
}

void free_matrix(Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        free(mat->data[i]);
    }
    free(mat->data);
}

int main(int argc, char* argv[]) {
    const char* mat1_filename = (argc > 1) ? argv[1] : "a";
    const char* mat2_filename = (argc > 2) ? argv[2] : "b";
    const char* output_prefix = (argc > 3) ? argv[3] : "c";

    char mat1_path[100], mat2_path[100], output_path[100];
    snprintf(mat1_path, sizeof(mat1_path), "%s.txt", mat1_filename);
    snprintf(mat2_path, sizeof(mat2_path), "%s.txt", mat2_filename);
    snprintf(output_path, sizeof(output_path), "%s_per_element.txt", output_prefix);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    Matrix a = read_matrix(mat1_path);
    Matrix b = read_matrix(mat2_path);
    Matrix c = multiply_matrices_per_element(&a, &b);
    write_matrix(output_path, &c);

    gettimeofday(&end, NULL);
    long seconds = end.tv_sec - start.tv_sec;
    long micros = end.tv_usec - start.tv_usec;
    if (micros < 0) {
        micros += 1000000;
        seconds--;
    }

    printf("Method: Per Element\n");
    printf("Threads created: %d\n", c.rows * c.cols);
    printf("Time taken: %.6f seconds (%ld microseconds)\n", seconds + micros / 1e6, seconds * 1000000 + micros);

    free_matrix(&a);
    free_matrix(&b);
    free_matrix(&c);
    return 0;
}
