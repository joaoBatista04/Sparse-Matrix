#ifndef MATRIX_H
#define MATRIX_H

typedef struct Sparse_Matrix Sparse_Matrix;
typedef float matrix_value_type;

//Allocation functions

Sparse_Matrix *sparse_matrix_create();
void sparse_matrix_destroy();

//Verification functions

void *sparse_matrix_index_exists(Sparse_Matrix *matrix, int row, int column);

//Setters functions

void _sparse_matrix_push_row(Sparse_Matrix *matrix, void *cell, int row);
void _sparse_matrix_push_column(Sparse_Matrix *matrix, void *cell, int column);
void _sparse_matrix_realloc(Sparse_Matrix *matrix, int row, int column);
void _sparse_matrix_destroy_cell(Sparse_Matrix *matrix, int row, int column);
void sparse_matrix_set_by_index(Sparse_Matrix *matrix, matrix_value_type data, int row, int column);

//Getters functions

matrix_value_type sparse_matrix_get_by_index(Sparse_Matrix *matrix, int row, int column);
matrix_value_type _sparse_matrix_sum_cells(Sparse_Matrix *matrix);

//Operation functions with matrices

Sparse_Matrix *sparse_matrix_multiply_scalar(Sparse_Matrix *matrix, matrix_value_type scalar);
Sparse_Matrix *sparse_matrix_sum(Sparse_Matrix *matrix1, Sparse_Matrix *matrix2);
Sparse_Matrix *sparse_matrix_multiplication(Sparse_Matrix *matrix1, Sparse_Matrix *matrix2);
Sparse_Matrix *sparse_matrix_multiply_point(Sparse_Matrix *matrix1, Sparse_Matrix *matrix2);
Sparse_Matrix *sparse_matrix_transpose(Sparse_Matrix *matrix);
Sparse_Matrix *sparse_matrix_swap_columns(Sparse_Matrix *matrix, int columnOne, int columnTwo);
Sparse_Matrix *sparse_matrix_swap_rows(Sparse_Matrix *matrix, int rowOne, int rowTwo);
Sparse_Matrix *sparse_matrix_slice(Sparse_Matrix *matrix, int rowOne, int columnOne, int rowTwo, int columnTwo);
Sparse_Matrix *sparse_matrix_convolution(Sparse_Matrix *matrix, Sparse_Matrix *kernel);

//Print functions

void sparse_matrix_show(Sparse_Matrix *matrix);
void sparse_matrix_show_dense(Sparse_Matrix *matrix);

//File functions

void sparse_matrix_binary_save(Sparse_Matrix *matrix);
Sparse_Matrix *sparse_matrix_binary_read(char *path_to_file);

#endif