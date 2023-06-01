#include <stdio.h>
#include <stdlib.h>
#include "cell.h"
#include "matrix.h"

typedef struct Sparse_Matrix{
    int numberRows, numberColumns, numberNonNullValues;
    Cell **rows;
    Cell **columns;
} Sparse_Matrix;

/**
 * @brief This function allocates memory for Sparse_Matrix type based on the number of rows and columns in the original matrix.
 *
 * @brief Time Complexity: O(1), because the operation is done in a unique way, regardless of the size of the matrix.
 * @return Sparse_Matrix* 
 * An allocated and empty sparse matrix
 */
Sparse_Matrix *sparse_matrix_create(){
    Sparse_Matrix *matrix = calloc(1, sizeof(Sparse_Matrix));

    matrix->rows = (Cell **)calloc(1, sizeof(Cell *));
    matrix->columns = (Cell **)calloc(1, sizeof(Cell *));

    matrix->numberRows = 1;
    matrix->numberColumns = 1;
    matrix->numberNonNullValues = 0;

    return matrix;
}

/**
 * @brief This function frees the memory allocated for Sparse_Matrix type.
 * 
 * @brief Time Complexity: O(n^2), because the function loops through the rows (depending on its size n) and the columns (which also depend on its size n).
 * @param matrix 
 * The pointer to a matrix that will be deallocated
 */
void sparse_matrix_destroy(Sparse_Matrix *matrix){
    Cell *current;
    Cell *aux;

    for(int i = 0; i < matrix->numberRows; i++){
        current = matrix->rows[i];

        while(current){
            aux = current->nextRow;
            cell_destroy(current);
            current = aux;
        }
    }

    free(matrix->rows);
    free(matrix->columns);
    free(matrix);
}

/**
 * @brief This function checks if the index exists in the Sparsed Matrix, i.e., if it represents some non-null value.
 * 
 * @brief Time Complexity: O(n), because the function goes straight to the line that needs to be evaluated and traverses its size n.
 * 
 * @param matrix 
 * The matrix that will be evaluated
 * @param row 
 * The row wanted
 * @param column 
 * The column wanted
 * @return void*
 * The pointer to the Cell if it exists or NULL if not
 */
void *sparse_matrix_index_exists(Sparse_Matrix *matrix, int row, int column){
    if(!matrix->rows[row] || !matrix->columns[column]){
        return NULL;
    }

    Cell *aux = matrix->rows[row];

    while(aux){
        if(aux->positionRow == row && aux->positionColumn == column){
            return aux;
        }

        aux = aux->nextRow;
    }

    return NULL;
}

/**
 * @brief This function inserts a value in the middle or at the beginning of the row.
 * 
 * @brief Time Complexity: O(1) if the value is inserted at the beginning of the list and O(n) if the value is inserted in the middle of the list, requiring it to be traversed
 * 
 * @param matrix 
 * The matrix that will receive the value
 * @param cell 
 * The pointer of the cell that will be placed at the matrix
 * @param row 
 * The index of the row where the cell will be placed
 */
void _sparse_matrix_push_row(Sparse_Matrix *matrix, void *cell, int row){
    Cell *current = matrix->rows[row];
    Cell *previous = NULL;
    Cell *cell_to_push = cell;

    if(current == NULL){
        matrix->rows[row] = cell_to_push;
        cell_to_push->nextRow = NULL;
        return; 
    }

    while(current){
        if(current->positionRow > row){
            if(previous == NULL){
                cell_to_push->nextRow = current;
                break;
            }
            else{
                previous->nextRow = cell_to_push;
                cell_to_push->nextRow = current;
                break;
            }
        }

        if(current->nextRow == NULL){
            current->nextRow = cell_to_push;
            break;
        }

        previous = current;
        current = current->nextRow;
    }
}

/**
 * @brief This function inserts a value in the middle or at the beginning of the column.
 * 
 * @brief Time Complexity: O(1) if the value is inserted at the beginning of the list and O(n) if the value is inserted in the middle of the list, requiring it to be traversed
 * 
 * @param matrix 
 * The matrix that will receive the value
 * @param cell 
 * The pointer of the cell that will be placed at the matrix
 * @param row 
 * The index of the column where the cell will be placed
 */
void _sparse_matrix_push_column(Sparse_Matrix *matrix, void *cell, int column){
    Cell *current = matrix->columns[column];
    Cell *previous = NULL;
    Cell *cell_to_push = cell;

    if(current == NULL){
        matrix->columns[column] = cell_to_push;
        cell_to_push->nextColumn = NULL;
        return; 
    }

    while(current){
        if(current->positionColumn > column){
            if(previous == NULL){
                cell_to_push->nextColumn = current;
                break;
            }
            else{
                previous->nextColumn = cell_to_push;
                cell_to_push->nextColumn = current;
                break;
            }
        }

        if(current->nextColumn == NULL){
            current->nextColumn = cell_to_push;
            break;
        }

        previous = current;
        current = current->nextColumn;
    }
}

/**
 * @brief This function frees the memory allocated for a cell when the user puts a 0 in place of a non-null value.
 * 
 * @brief Time Complexity: O(n), because the function goes straight to the row you want to destroy, traversing the size n of the list
 * 
 * @param matrix 
 * The matrix that will be modified
 * @param row 
 * The row of the cell that will be freed
 * @param column 
 * The column of the cell that will be freed
 */
void _sparse_matrix_destroy_cell(Sparse_Matrix *matrix, int row, int column){
    Cell *current = matrix->rows[row];
    Cell *aux = NULL;
    Cell *prev = NULL;

    while(current){
        if(current->positionColumn == column && current->positionRow == row){
            if(column == 0){
                aux = current;
                matrix->rows[row] = aux->nextRow;
                cell_destroy(aux);
                break;
            }

            else{
                aux = current;
                prev->nextRow = current->nextRow;
                cell_destroy(aux);
                break;
            }
        }

        prev = current;
        current = current->nextRow;
    }
}

/**
 * @brief This function creates a cell when the user puts a non-null value in the matrix.
 * 
 * @brief Time Complexity: O(2n), because this function uses the push functions two times (each one has time complexity O(n))
 * 
 * @param matrix 
 * The matrix that will be defined
 * @param data 
 * The value that will be defined
 * @param row 
 * The row wanted
 * @param column 
 * The column wanted
 * @return void* 
 * The return is a void pointer that will be transformed in Cell* in other functions
 */
void *_sparse_matrix_create_cell(Sparse_Matrix *matrix, matrix_value_type data, int row, int column){
    Cell *new_cell = cell_creating(column, row, data, NULL, NULL);

    _sparse_matrix_push_row(matrix, new_cell, row);
    _sparse_matrix_push_column(matrix, new_cell, column);

    return new_cell;
}

/**
 * @brief This function reallocates the memory for a matrix when the user puts a non-null value in a index higher than the current indexes.
 * 
 * @brief Time Complexity: O(2n), because this function relocates the row and column list, moving the components once (depends on size n)
 * @param matrix 
 * The matrix that will be reallocated
 * @param row 
 * The new number of rows
 * @param column 
 * The new number of columns
 */
void _sparse_matrix_realloc(Sparse_Matrix *matrix, int row, int column){
    if(row > matrix->numberRows - 1){
        matrix->rows = (Cell **)realloc(matrix->rows, (row + 1) * sizeof(Cell *));

        for(int i = matrix->numberRows; i < row + 1; i++){
            matrix->rows[i] = NULL;
        }

        matrix->numberRows = row + 1;
    }

    if(column > matrix->numberColumns - 1){
        matrix->columns = (Cell **)realloc(matrix->columns, (column + 1) * sizeof(Cell *));

        for(int i = matrix->numberColumns; i < column + 1; i++){
            matrix->columns[i] = NULL;
        }

        matrix->numberColumns = column + 1;
    }
}

/**
 * @brief This function puts a value in the matrix by the index. If the user tries to put a 0 in the matrix, the memory will be deleted if there is a non-null value. If a non-null value is placed into an empty index, memory will be allocated to it.
 * 
 * @brief Time Complexity: O(2n) if not necessary relocate the list and O(3n) if necessary
 * 
 * @param matrix 
 * The matrix that will be defined
 * @param data
 * The value that will be defined 
 * @param row 
 * The row wanted
 * @param column 
 * The column wanted
 */
void sparse_matrix_set_by_index(Sparse_Matrix *matrix, matrix_value_type data, int row, int column){
    if(row < 0 || column < 0){
        printf("\033[91mError: invalid index was read!\n\033[0m");
        exit(1);
    }
    
    if((row > matrix->numberRows - 1 || column > matrix->numberColumns - 1)){
        _sparse_matrix_realloc(matrix, row, column);
    }

    Cell *aux = sparse_matrix_index_exists(matrix, row, column);

    if(data == 0){
        if(aux != NULL){
            _sparse_matrix_destroy_cell(matrix, row, column);
            matrix->numberNonNullValues--;
        }
    }

    else{
        if(aux == NULL){
            aux = _sparse_matrix_create_cell(matrix, data, row, column);
        }

        aux->value = data;
        matrix->numberNonNullValues++;
    }
}

/**
 * @brief This function returns the value of an index in sparse matrix. If the index doesn't represent a valid cell, 0.0 is returned.
 * 
 * @brief Time Complexity: O(n), because this function loops through the entire list of size n to check if the value exists
 * 
 * @param matrix 
 * The matrix that will be evaluated
 * @param row 
 * The row wanted
 * @param column 
 * The column wanted
 * @return matrix_value_type 
 * The value of that index (null or non-null)
 */
matrix_value_type sparse_matrix_get_by_index(Sparse_Matrix *matrix, int row, int column){
    Cell *aux = sparse_matrix_index_exists(matrix, row, column);

    if(aux == NULL){
        return 0.0;
    }

    else{
        return aux->value;
    }
}

/**
 * @brief This function returns the sum of the values in the matrix.
 * 
 * @brief Time Complexity: O(n^2), because this function traverses the list of size n, n times
 * 
 * @param matrix 
 * The matrix that will be operated
 * @return matrix_value_type 
 * The sum of the values in the matrix
 */
matrix_value_type _sparse_matrix_sum_cells(Sparse_Matrix *matrix){
    matrix_value_type sum = 0;

    for(int i = 0; i < matrix->numberRows; i++){
        for(int j = 0; j < matrix->numberColumns; j++){
            sum += sparse_matrix_get_by_index(matrix, i, j);
        }
    }

    return sum;
}

/**
 * @brief This function multiply the values of a sparse matrix by a scalar k and returns a new sparse matrix.
 * 
 * @brief Time Complexity: O(2n^3), because this function traverses the list of size n, n times, with complexity 2n when accommodating the value
 * 
 * @param matrix 
 * The original matrix
 * @param scalar 
 * The factor by which the values ​​will be multiplied
 * @return Sparse_Matrix* 
 * The new matrix with multiplied values
 */
Sparse_Matrix *sparse_matrix_multiply_scalar(Sparse_Matrix *matrix, matrix_value_type scalar){
    Cell *current;

    Sparse_Matrix *new_matrix = sparse_matrix_create();

    for(int i = 0; i < matrix->numberRows; i++){
        current = matrix->rows[i];

        while(current){
            sparse_matrix_set_by_index(new_matrix, current->value * scalar, current->positionRow, current->positionColumn);
            current = current->nextRow;
        }
    }

    printf("\033[92m----------------------------------------------\nMATRIX FOR SCALAR MULTIPLICATION BY %.2f:\n\033[0m", scalar);
    sparse_matrix_show_dense(matrix);
    printf("\033[92m\nRESULT OF SCALAR MULTIPLICATION BY %.2f:\n\033[0m", scalar);
    sparse_matrix_show_dense(new_matrix);
    printf("\033[92m----------------------------------------------\n\033[0m");

    return new_matrix;
}

/**
 * @brief This function adds the values of two matrices and returns a new sparse matrix.
 * 
 * @brief Time Complexity: O(2n^4), because this function loops through the list of size n, n times. And in each repetition, it goes through the list to get the value and it takes 2n to accommodate the value in the new matrix
 * 
 * @param matrix1 
 * The first matrix
 * @param matrix2
 * The second matrix
 * @return Sparse_Matrix* 
 * The new sparse matrix that contains the result of the sum
 */
Sparse_Matrix *sparse_matrix_sum(Sparse_Matrix *matrix1, Sparse_Matrix *matrix2){
    if(matrix1->numberRows != matrix2->numberRows || matrix1->numberColumns != matrix2->numberColumns){
        printf("\033[91mError: the number of columns and rows is not equal in both matrices!\n\033[0m");
        exit(1);
    }

    Sparse_Matrix *new_matrix = sparse_matrix_create();
    Cell *current;

    for(int i = 0; i < matrix1->numberRows; i++){
        current = matrix1->rows[i];

        while(current){
            matrix_value_type data = sparse_matrix_get_by_index(matrix2, i, current->positionColumn) + current->value;
            sparse_matrix_set_by_index(new_matrix, data, i, current->positionColumn);
            current = current->nextRow;
        }
    }

    printf("\033[92m----------------------------------------------\nFIRST MATRIX FOR SUM:\n\033[0m");
    sparse_matrix_show_dense(matrix1);
    printf("\033[92m\nSECOND MATRIX FOR SUM:\n\033[0m");
    sparse_matrix_show_dense(matrix2);
    printf("\033[92m\nRESULT OF MATRICES SUM:\n\033[0m");
    sparse_matrix_show_dense(new_matrix);
    printf("\033[92m----------------------------------------------\n\033[0m");

    return new_matrix;
}

/**
 * @brief This function multiplies two matrices.
 * 
 * @brief Time Complexity: O(4n^4), because this function loops through a list of size n, n times. And each repetition takes a process O(2n) and two processes O(n)
 * 
 * @param matrix1 
 * The first matrix to multiply
 * @param matrix2 
 * The second matrix to multiply
 * @return Sparse_Matrix* 
 * The new matrix resulting from the multiplication
 */
Sparse_Matrix *sparse_matrix_multiplication(Sparse_Matrix *matrix1, Sparse_Matrix *matrix2){
    if(matrix1->numberColumns != matrix2->numberRows){
        printf("\033[91mError: the number of columns and rows is not equal in both matrices!\n\033[0m");
        exit(1);
    }

    Sparse_Matrix *new_matrix = sparse_matrix_create();

    for(int i = 0; i < matrix1->numberRows; i++){
        float sum = 0;

        for(int j = 0; j < matrix2->numberColumns; j++){
            for(int k = 0; k < matrix1->numberColumns; k++){
                sum += (sparse_matrix_get_by_index(matrix1, i, k) * sparse_matrix_get_by_index(matrix2, k, j));
            }

            sparse_matrix_set_by_index(new_matrix, sum, i, j);
            sum = 0;
        }
    }

    printf("\033[92m----------------------------------------------\nFIRST MATRIX FOR MULTIPLICATION:\n\033[0m");
    sparse_matrix_show_dense(matrix1);
    printf("\033[92m\nSECOND MATRIX FOR MULTIPLICATION:\n\033[0m");
    sparse_matrix_show_dense(matrix2);
    printf("\033[92m\nRESULT OF MATRICES MULTIPLICATION:\n\033[0m");
    sparse_matrix_show_dense(new_matrix);
    printf("\033[92m----------------------------------------------\n\033[0m");

    return new_matrix;
}

/**
 * @brief This function multiply two matrices by points. This means that the point M1(i, j) will be multiplied by the point M2(i, j)
 * 
 * @brief Time Complexity: O(2n^3) or O(3n^3), because this function loops through a list of size n, n times. And each repetition takes a process O(2n) or O(3n)
 * 
 * @param matrix1
 * The first matrix to multiply 
 * @param matrix2 
 * The second matrix to multiply
 * @return Sparse_Matrix* 
 * The return is the new matrix created
 */
Sparse_Matrix *sparse_matrix_multiply_point(Sparse_Matrix *matrix1, Sparse_Matrix *matrix2){
    if(matrix1->numberRows != matrix2->numberRows || matrix1->numberColumns != matrix2->numberColumns){
        printf("\033[91mError: the number of columns and rows is not equal in both matrices!\n\033[0m");
        exit(1);
    }

    Sparse_Matrix *new_matrix = sparse_matrix_create();
    Cell *current;

    for(int i = 0; i < matrix1->numberRows; i++){
        current = matrix1->rows[i];

        while(current){
            matrix_value_type data = sparse_matrix_get_by_index(matrix2, i, current->positionColumn) * current->value;
            sparse_matrix_set_by_index(new_matrix, data, i, current->positionColumn);
            current = current->nextRow;
        }
    }

    printf("\033[92m----------------------------------------------\nFIRST MATRIX FOR POINT MULTIPLICATION:\n\033[0m");
    sparse_matrix_show_dense(matrix1);
    printf("\033[92m\nSECOND MATRIX FOR POINT MULTIPLICATION:\n\033[0m");
    sparse_matrix_show_dense(matrix2);
    printf("\033[92m\nRESULT OF MATRICES POINT MULTIPLICATION:\n\033[0m");
    sparse_matrix_show_dense(new_matrix);
    printf("\033[92m----------------------------------------------\n\033[0m");

    return new_matrix;
}

/**
 * @brief This function transposes a matrix.
 * 
 * @brief Time Complexity: O(2n^3), because this function loops through a list of size n, n times. And each repetition takes a process O(2n)
 * 
 * @param matrix
 * The matrix that will be transposed 
 * @return Sparse_Matrix* 
 * The return is the new matrix created
 */
Sparse_Matrix *sparse_matrix_transpose(Sparse_Matrix *matrix){
    Sparse_Matrix *new_matrix = sparse_matrix_create();
    Cell *current;

    for(int i = 0; i < matrix->numberRows; i++){
        current = matrix->rows[i];

        while(current){
            sparse_matrix_set_by_index(new_matrix, current->value, current->positionColumn, current->positionRow);
            current = current->nextRow;
        }
    }

    printf("\033[92m----------------------------------------------\nMATRIX FOR TRANSPOSE:\n\033[0m");
    sparse_matrix_show_dense(matrix);
    printf("\033[92m\nRESULT OF MATRIX TRANSPOSE:\n\033[0m");
    sparse_matrix_show_dense(new_matrix);
    printf("\033[92m----------------------------------------------\n\033[0m");

    return new_matrix;
}

/**
 * @brief This function swaps the position of two columns in the matrix.
 * 
 * @brief Time Complexity: O(2n^3), because this function loops through a list of size n, n times. And each repetition takes a process O(2n)
 * 
 * @param matrix 
 * The matrix that will be changed
 * @param columnOne 
 * The index of the first column that will be changed
 * @param columnTwo 
 * The index of the second column that will be changed
 * @return Sparse_Matrix* 
 * The new matrix created
 */
Sparse_Matrix *sparse_matrix_swap_columns(Sparse_Matrix *matrix, int columnOne, int columnTwo){
    if(columnOne < 0 || columnTwo < 0 || columnOne == columnTwo || columnOne > matrix->numberColumns || columnTwo > matrix->numberColumns){
        printf("\033[91mError: couldn't change these indexes!\n\033[0m");
        exit(1);
    }

    Cell *current;

    Sparse_Matrix *new_matrix = sparse_matrix_create();

    for(int i = 0; i < matrix->numberRows; i++){
        current = matrix->rows[i];

        while(current){
            if(current->positionColumn == columnOne){
                sparse_matrix_set_by_index(new_matrix, current->value, current->positionRow, columnTwo);
            }

            else if(current->positionColumn == columnTwo){
                sparse_matrix_set_by_index(new_matrix, current->value, current->positionRow, columnOne);
            }

            else{
                sparse_matrix_set_by_index(new_matrix, current->value, current->positionRow, current->positionColumn);
            }

            current = current->nextRow;
        }
    }

    printf("\033[92m----------------------------------------------\nMATRIX FOR COLUMNS SWAP - COLUMNS [%d] and [%d]:\n\033[0m", columnOne, columnTwo);
    sparse_matrix_show_dense(matrix);
    printf("\033[92m\nRESULT OF COLUMNS SWAP:\n\033[0m");
    sparse_matrix_show_dense(new_matrix);
    printf("\033[92m----------------------------------------------\n\033[0m");

    return new_matrix;
}

/**
 * @brief This function swaps the position of two rows in the matrix.
 * 
 * @brief Time Complexity: O(2n^3), because this function loops through a list of size n, n times. And each repetition takes a process O(2n)
 * 
 * @param matrix 
 * The matrix that will be changed
 * @param rowOne 
 * The index of the first row that will be changed
 * @param rowTwo 
 * The index of the second row that will be changed
 * @return Sparse_Matrix* 
 * The new matrix created
 */
Sparse_Matrix *sparse_matrix_swap_rows(Sparse_Matrix *matrix, int rowOne, int rowTwo){
    if(rowOne < 0 || rowTwo < 0 || rowOne == rowTwo || rowOne > matrix->numberRows || rowTwo > matrix->numberRows){
        printf("\033[91mError: couldn't change these indexes!\n\033[0m");
        exit(1);
    }

    Cell *current;

    Sparse_Matrix *new_matrix = sparse_matrix_create();

    for(int i = 0; i < matrix->numberRows; i++){
        current = matrix->rows[i];

        while(current){
            if(current->positionRow == rowOne){
                sparse_matrix_set_by_index(new_matrix, current->value, rowTwo, current->positionColumn);
            }

            else if(current->positionRow == rowTwo){
                sparse_matrix_set_by_index(new_matrix, current->value, rowOne, current->positionColumn);
            }

            else{
                sparse_matrix_set_by_index(new_matrix, current->value, current->positionRow, current->positionColumn);
            }

            current = current->nextRow;
        }
    }

    printf("\033[92m----------------------------------------------\nMATRIX FOR ROWS SWAP - ROWS [%d] and [%d]:\n\033[0m", rowOne, rowTwo);
    sparse_matrix_show_dense(matrix);
    printf("\033[92m\nRESULT OF ROWS SWAP:\n\033[0m");
    sparse_matrix_show_dense(new_matrix);
    printf("\033[92m----------------------------------------------\n\033[0m");

    return new_matrix;
}

/**
 * @brief This function slices a matrix by two indexes.
 * 
 * @brief Time Complexity: O(2n^4), because this function loops through a list of size n, n times. And each repetition takes a O(2n^2) process
 * 
 * @param matrix 
 * The matrix that will be sliced
 * @param rowOne 
 * The index of the row of the begin
 * @param columnOne 
 * The index of the column of the begin
 * @param rowTwo 
 * The index of the row of the end
 * @param columnTwo 
 * The index of the column of the end
 * @return Sparse_Matrix* 
 * The new matrix sliced
 */
Sparse_Matrix *sparse_matrix_slice(Sparse_Matrix *matrix, int rowOne, int columnOne, int rowTwo, int columnTwo){
    if(rowOne < 0 || rowOne >= matrix->numberRows || columnOne < 0 || columnOne >= matrix->numberColumns || rowTwo < 0 || rowTwo >= matrix->numberRows || columnTwo < 0 || columnTwo >= matrix->numberColumns){
        printf("\033[91mError: couldn't slice the matrix by these indexes!\n\033[0m");
        exit(1);
    }

    if(rowOne == rowTwo && columnOne == columnTwo){
        printf("\033[91mError: couldn't slice the matrix by these indexes!\n\033[0m");
        exit(1);
    }

    if(rowOne == 0 && columnOne == 0 && rowTwo == matrix->numberRows - 1 && columnTwo == matrix->numberColumns - 1){
        printf("\033[91mError: the slicing isn't necessary\n\033[0m");
        return matrix;
    }

    if(rowOne > rowTwo && columnOne > columnTwo){
        int aux1, aux2;

        aux1 = rowOne;
        aux2 = columnOne;

        rowOne = rowTwo;
        columnOne = columnTwo;
        rowTwo = aux1;
        columnTwo = aux2;
    }

    Sparse_Matrix *new_matrix = sparse_matrix_create();

    for(int i = rowOne; i <= rowTwo; i++){
        for(int j = columnOne; j <= columnTwo; j++){
            sparse_matrix_set_by_index(new_matrix, sparse_matrix_get_by_index(matrix, i, j), i - rowOne, j - columnOne);
        }
    }

    printf("\033[92m----------------------------------------------\nMATRIX FOR SLICE - ROW 1: [%d] [%d] AND ROW 2: [%d] [%d]\n\033[0m", rowOne, columnOne, rowTwo, columnTwo);
    sparse_matrix_show_dense(matrix);
    printf("\033[92m\nRESULT OF MATRIX SLICE:\n\033[0m");
    sparse_matrix_show_dense(new_matrix);
    printf("\033[92m----------------------------------------------\n\033[0m");

    return new_matrix;
}

/**
 * @brief This function makes the convolution of a matrix from a kernel.
 * 
 * @brief Time Complexity: O(12n^10), because there are 4 loops with size n and that house processes with different time complexities
 * 
 * @param matrix 
 * The matrix that will be convoluted
 * @param kernel 
 * The kernel of the convolution
 * @return Sparse_Matrix* 
 * The new matrix created
 */
Sparse_Matrix *sparse_matrix_convolution(Sparse_Matrix *matrix, Sparse_Matrix *kernel){
    if(kernel->numberColumns % 2 == 0 && kernel->numberRows % 2 == 0){
        printf("\033[91mError: it's necessary that the kernel has an odd size!\n\033[0m");
        exit(1);
    }

    Sparse_Matrix *result = sparse_matrix_create();

    int half_column = kernel->numberColumns / 2;
    int half_row = kernel->numberRows / 2;

    for(int i = 0; i < matrix->numberRows; i++){
        float sum = 0;

        for(int j = 0; j < matrix->numberColumns; j++){
            Sparse_Matrix *new_matrix = sparse_matrix_create();

            for(int h = i - half_row; h <= i + half_row; h++){
                for(int k = j - half_column; k <= j + half_column; k++){
                    if (k < 0 || k >= matrix->numberColumns || h < 0 || h >= matrix->numberRows){
                        sparse_matrix_set_by_index(new_matrix, 0, h + half_row - i, k + half_column - j);
                    }

                    else{
                        sparse_matrix_set_by_index(new_matrix, sparse_matrix_get_by_index(matrix, h, k), h + half_row - i, k + half_column - j);
                    }
                }
            }

            Sparse_Matrix *mult = sparse_matrix_multiply_point(kernel, new_matrix);
            
            sum = _sparse_matrix_sum_cells(mult);
            sparse_matrix_set_by_index(result, sum, i, j);

            sparse_matrix_destroy(mult);
            sparse_matrix_destroy(new_matrix);
        }
    }

    printf("\033[92m----------------------------------------------\nMATRIX FOR CONVOLUTION:\n\033[0m");
    sparse_matrix_show_dense(matrix);
    printf("\033[92m\nKERNEL FOR MATRIX CONVOLUTION:\n\033[0m");
    sparse_matrix_show_dense(kernel);
    printf("\033[92m\nRESULT OF MATRIX CONVOLUTION:\n\033[0m");
    sparse_matrix_show_dense(result);
    printf("\033[92m----------------------------------------------\n\033[0m");  

    return result;
}

/**
 * @brief This function shows on the screen just the non-null values of a sparse matrix.
 * 
 * @brief Time Complexity: O(n^2), because this function loops through the list of size n, n times
 * 
 * @param matrix
 * The matrix that will be displayed 
 */
void sparse_matrix_show(Sparse_Matrix *matrix){
    Cell *current;

    printf("\033[92mSHOW SPARSE MATRIX:\nROWS: %d COLUMNS: %d\n\n\033[0m", matrix->numberRows, matrix->numberColumns);

    for(int i = 0; i < matrix->numberRows; i++){
        current = matrix->rows[i];

        while(current != NULL){
            printf("\033[95m[%d][%d]\033[0m \033[97m--> \033[0m\033[92m%.2f\033[0m\n", current->positionRow, current->positionColumn, current->value);
            current = current->nextRow;
        }
    }
}

/**
 * @brief This function shows the entire matrix, including the null values (dense matrix).
 * 
 * @brief Time Complexity: O(2n^3), because this function loops through a list of size n, n times. And each repetition takes 2 n processes to work
 * 
 * @param matrix 
 * The matrix that will be displayed
 */
void sparse_matrix_show_dense(Sparse_Matrix *matrix){
    Cell *aux;

    printf("\033[92mSHOW DENSE MATRIX:\nROWS: %d COLUMNS: %d\n\n\033[0m\t", matrix->numberRows, matrix->numberColumns);

    for(int i = 0; i < matrix->numberColumns; i++){
        printf("\033[95m[%d]\t\033[0m", i);
    }
    printf("\n");

    for(int i = 0; i < matrix->numberRows; i++){
        printf("\033[95m[%d]\t\033[0m", i);
        for(int j = 0; j < matrix->numberColumns; j++){
            aux = sparse_matrix_index_exists(matrix, i, j);

            if(aux == NULL){
                printf("0.00\t");
            }

            else{
                printf("\033[94m%.2f\033[0m\t", sparse_matrix_get_by_index(matrix, i, j));
            }
        }

        printf("\n");
    }
}

/**
 * @brief This functions creates a binary file with the parameters and values of a sparse matrix (just the non-null values).
 * 
 * @brief Time Complexity: O(n^2), because this function loops through the list of size n, n times
 * 
 * @param matrix 
 * The matrix that will be saved in the file
 */
void sparse_matrix_binary_save(Sparse_Matrix *matrix){
    FILE *fp = fopen("./matrix.bin", "wb");

    if(!fp){
        printf("\033[91mError: Couldn't create the file!\n\033[0m");
        exit(1);
    }

    fwrite(&matrix->numberNonNullValues, 1, sizeof(int), fp);

    Cell *current;

    for(int i = 0; i < matrix->numberRows; i++){
        current = matrix->rows[i];

        while(current != NULL){
            fwrite(&current->positionRow, 1, sizeof(int), fp);
            fwrite(&current->positionColumn, 1, sizeof(int), fp);
            fwrite(&current->value, 1, sizeof(float), fp);
            current = current->nextRow;
        }
    }

    fclose(fp);
}

/**
 * @brief This function creates a sparse matrix with the informations obtained from a binary file.
 * 
 * @brief Time Complexity: O(2n^2) or O(3n^2), because this function calls a function of complexity O(2n) or O(3n), n times 
 * 
 * @param path_to_file 
 * The path to the binary file
 * @return Sparse_Matrix* 
 * The new matrix created
 */
Sparse_Matrix *sparse_matrix_binary_read(char *path_to_file){
    FILE *fp = fopen(path_to_file, "rb");

    if(!fp){
        printf("\033[91mError: Couldn't open the file!\n\033[0m");
        exit(1);
    }

    int numberNonNullValues, column, row;
    float value;
    Sparse_Matrix *matrix = sparse_matrix_create();

    fread(&numberNonNullValues, 1, sizeof(int), fp);

    for(int i = 0; i < numberNonNullValues; i++){
        fread(&row, 1, sizeof(int), fp);
        fread(&column, 1, sizeof(int), fp);
        fread(&value, 1, sizeof(float), fp);

        sparse_matrix_set_by_index(matrix, value, row, column);
    }

    fclose(fp);

    return matrix;
}