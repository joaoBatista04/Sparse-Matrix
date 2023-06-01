#include <stdio.h>
#include "matrix.h"
#include "cell.h"

#define SZ_LINES 4
#define SZ_COLUMNS 5

#define LINES_KERNEL 3
#define COLUMNS_KERNEL 3

int main(){
    Sparse_Matrix *matrix = sparse_matrix_create();

    sparse_matrix_set_by_index(matrix, 3, 1, 1);
    sparse_matrix_set_by_index(matrix, 2, 0, 1);
    sparse_matrix_set_by_index(matrix, 1, 1, 0);
    sparse_matrix_set_by_index(matrix, 4, 0, 0);
    sparse_matrix_set_by_index(matrix, 7, 2, 2);

    matrix_value_type data = sparse_matrix_get_by_index(matrix, 2, 2);
    printf("Valor encontrado: %.2f\n", data);
    //Esperado: 
    //Valor encontrado: 7.00

    sparse_matrix_show(matrix);
    /*Esperado:
    SHOW SPARSE MATRIX:
    ROWS: 3 COLUMNS: 3

    [0][1] --> 2.00
    [0][0] --> 4.00
    [1][1] --> 3.00
    [1][0] --> 1.00
    [2][2] --> 7.00
    */

    sparse_matrix_show_dense(matrix);
    /*Esperado
    SHOW DENSE MATRIX:
    ROWS: 3 COLUMNS: 3

            [0]     [1]     [2]
    [0]     4.00    2.00    0.00
    [1]     1.00    3.00    0.00
    [2]     0.00    0.00    7.00
    */

   Sparse_Matrix *mult = sparse_matrix_multiply_scalar(matrix, 3.1);
    /*Esperado:
    ----------------------------------------------
    MATRIX FOR SCALAR MULTIPLICATION BY 3.10:
    SHOW DENSE MATRIX:
    ROWS: 3 COLUMNS: 3

            [0]     [1]     [2]
    [0]     4.00    2.00    0.00
    [1]     1.00    3.00    0.00
    [2]     0.00    0.00    7.00

    RESULT OF SCALAR MULTIPLICATION BY 3.10:
    SHOW DENSE MATRIX:
    ROWS: 3 COLUMNS: 3

            [0]     [1]     [2]
    [0]     12.40   6.20    0.00
    [1]     3.10    9.30    0.00
    [2]     0.00    0.00    21.70
    ----------------------------------------------
    */

    Sparse_Matrix *sum = sparse_matrix_sum(matrix, mult);
    /*Esperado:
    ----------------------------------------------
    FIRST MATRIX FOR SUM:
    SHOW DENSE MATRIX:
    ROWS: 3 COLUMNS: 3

            [0]     [1]     [2]
    [0]     4.00    2.00    0.00
    [1]     1.00    3.00    0.00
    [2]     0.00    0.00    7.00

    SECOND MATRIX FOR SUM:
    SHOW DENSE MATRIX:
    ROWS: 3 COLUMNS: 3

            [0]     [1]     [2]
    [0]     12.40   6.20    0.00
    [1]     3.10    9.30    0.00
    [2]     0.00    0.00    21.70

    RESULT OF MATRICES SUM:
    SHOW DENSE MATRIX:
    ROWS: 3 COLUMNS: 3

            [0]     [1]     [2]
    [0]     16.40   8.20    0.00
    [1]     4.10    12.30   0.00
    [2]     0.00    0.00    28.70
    ----------------------------------------------
    */

    Sparse_Matrix *multiplication = sparse_matrix_multiplication(matrix, matrix);
    /*Esperado:
    ----------------------------------------------
    FIRST MATRIX FOR MULTIPLICATION:
    SHOW DENSE MATRIX:
    ROWS: 3 COLUMNS: 3

            [0]     [1]     [2]
    [0]     4.00    2.00    0.00
    [1]     1.00    3.00    0.00
    [2]     0.00    0.00    7.00

    SECOND MATRIX FOR MULTIPLICATION:
    SHOW DENSE MATRIX:
    ROWS: 3 COLUMNS: 3

            [0]     [1]     [2]
    [0]     4.00    2.00    0.00
    [1]     1.00    3.00    0.00
    [2]     0.00    0.00    7.00

    RESULT OF MATRICES MULTIPLICATION:
    SHOW DENSE MATRIX:
    ROWS: 3 COLUMNS: 3

            [0]     [1]     [2]
    [0]     18.00   14.00   0.00
    [1]     7.00    11.00   0.00
    [2]     0.00    0.00    49.00
    ----------------------------------------------
    */

    Sparse_Matrix *point = sparse_matrix_multiply_point(matrix, multiplication);
    /*Esperado:
    ----------------------------------------------
    FIRST MATRIX FOR POINT MULTIPLICATION:
    SHOW DENSE MATRIX:
    ROWS: 3 COLUMNS: 3

            [0]     [1]     [2]
    [0]     4.00    2.00    0.00
    [1]     1.00    3.00    0.00
    [2]     0.00    0.00    7.00

    SECOND MATRIX FOR POINT MULTIPLICATION:
    SHOW DENSE MATRIX:
    ROWS: 3 COLUMNS: 3

            [0]     [1]     [2]
    [0]     18.00   14.00   0.00
    [1]     7.00    11.00   0.00
    [2]     0.00    0.00    49.00

    RESULT OF MATRICES POINT MULTIPLICATION:
    SHOW DENSE MATRIX:
    ROWS: 3 COLUMNS: 3

            [0]     [1]     [2]
    [0]     72.00   28.00   0.00
    [1]     7.00    33.00   0.00
    [2]     0.00    0.00    343.00
    ----------------------------------------------
    */
    
    Sparse_Matrix *transpose = sparse_matrix_transpose(matrix);
    /*Esperado:
    ----------------------------------------------
    MATRIX FOR TRANSPOSE:
    SHOW DENSE MATRIX:
    ROWS: 3 COLUMNS: 3

            [0]     [1]     [2]
    [0]     4.00    2.00    0.00
    [1]     1.00    3.00    0.00
    [2]     0.00    0.00    7.00

    RESULT OF MATRIX TRANSPOSE:
    SHOW DENSE MATRIX:
    ROWS: 3 COLUMNS: 3

            [0]     [1]     [2]
    [0]     4.00    1.00    0.00
    [1]     2.00    3.00    0.00
    [2]     0.00    0.00    7.00
    ----------------------------------------------
    */

    Sparse_Matrix *swap_columns = sparse_matrix_swap_columns(matrix, 0, 1);
    /*Esperado:
    ----------------------------------------------
    MATRIX FOR COLUMNS SWAP - COLUMNS [0] and [1]:
    SHOW DENSE MATRIX:
    ROWS: 3 COLUMNS: 3

            [0]     [1]     [2]
    [0]     4.00    2.00    0.00
    [1]     1.00    3.00    0.00
    [2]     0.00    0.00    7.00

    RESULT OF COLUMNS SWAP:
    SHOW DENSE MATRIX:
    ROWS: 3 COLUMNS: 3

            [0]     [1]     [2]
    [0]     2.00    4.00    0.00
    [1]     3.00    1.00    0.00
    [2]     0.00    0.00    7.00
    ----------------------------------------------
    */

    Sparse_Matrix *swap_rows = sparse_matrix_swap_rows(matrix, 0, 1);
    /*Esperado:
    ----------------------------------------------
    MATRIX FOR ROWS SWAP - ROWS [0] and [1]:
    SHOW DENSE MATRIX:
    ROWS: 3 COLUMNS: 3

            [0]     [1]     [2]
    [0]     4.00    2.00    0.00
    [1]     1.00    3.00    0.00
    [2]     0.00    0.00    7.00

    RESULT OF ROWS SWAP:
    SHOW DENSE MATRIX:
    ROWS: 3 COLUMNS: 3

            [0]     [1]     [2]
    [0]     1.00    3.00    0.00
    [1]     4.00    2.00    0.00
    [2]     0.00    0.00    7.00
    ----------------------------------------------
    */

    Sparse_Matrix *slice = sparse_matrix_slice(matrix, 0, 0, 1, 1);
    /*Esperado:
    ----------------------------------------------
    MATRIX FOR SLICE - ROW 1: [0] [0] AND ROW 2: [1] [1]
    SHOW DENSE MATRIX:
    ROWS: 3 COLUMNS: 3

            [0]     [1]     [2]
    [0]     4.00    2.00    0.00
    [1]     1.00    3.00    0.00
    [2]     0.00    0.00    7.00

    RESULT OF MATRIX SLICE:
    SHOW DENSE MATRIX:
    ROWS: 2 COLUMNS: 2

            [0]     [1]
    [0]     4.00    2.00
    [1]     1.00    3.00
    ----------------------------------------------
    */

    sparse_matrix_binary_save(matrix);
    //Esperado: arquivo matrix.bin

    Sparse_Matrix *binary_read = sparse_matrix_binary_read("./matrix.bin");
    sparse_matrix_show(binary_read);
    /*Esperado:
    ----------------------------------------------
    SHOW SPARSE MATRIX:
    ROWS: 3 COLUMNS: 3

    [0][1] --> 2.00
    [0][0] --> 4.00
    [1][1] --> 3.00
    [1][0] --> 1.00
    [2][2] --> 7.00
    */

   //CONVOLUTION - COMENTAR ESSA PARTE PARA AVERIGUAR OS RESULTADOS DAS OPERACOES ACIMA
   /*Sparse_Matrix *m = sparse_matrix_create();

    for (int i = 0; i < SZ_LINES; i++)
    {
        for (int j = 0; j < SZ_COLUMNS; j++)
            sparse_matrix_set_by_index(m, i + j, i, j);
    }

    Sparse_Matrix *k = sparse_matrix_create();

    for (int i = 0; i < LINES_KERNEL; i++)
    {
        for (int j = 0; j < COLUMNS_KERNEL; j++)
            sparse_matrix_set_by_index(k, 6, i, j);
    }

    Sparse_Matrix *conv1 = sparse_matrix_convolution(m, k);
    sparse_matrix_destroy(conv1);
    sparse_matrix_destroy(k);
    sparse_matrix_destroy(m);*/

    /*Esperado:
    ----------------------------------------------
    MATRIX FOR CONVOLUTION:
    SHOW DENSE MATRIX:
    ROWS: 4 COLUMNS: 5

            [0]     [1]     [2]     [3]     [4]
    [0]     0.00    1.00    2.00    3.00    4.00
    [1]     1.00    2.00    3.00    4.00    5.00
    [2]     2.00    3.00    4.00    5.00    6.00
    [3]     3.00    4.00    5.00    6.00    7.00

    KERNEL FOR MATRIX CONVOLUTION:
    SHOW DENSE MATRIX:
    ROWS: 3 COLUMNS: 3

            [0]     [1]     [2]
    [0]     6.00    6.00    6.00
    [1]     6.00    6.00    6.00
    [2]     6.00    6.00    6.00

    RESULT OF MATRIX CONVOLUTION:
    SHOW DENSE MATRIX:
    ROWS: 4 COLUMNS: 5

            [0]     [1]     [2]     [3]     [4]
    [0]     24.00   54.00   90.00   126.00  96.00
    [1]     54.00   108.00  162.00  216.00  162.00
    [2]     90.00   162.00  216.00  270.00  198.00
    [3]     72.00   126.00  162.00  198.00  144.00
    ----------------------------------------------
    */

    sparse_matrix_destroy(binary_read);
    sparse_matrix_destroy(slice);
    sparse_matrix_destroy(swap_rows);
    sparse_matrix_destroy(swap_columns);
    sparse_matrix_destroy(transpose);
    sparse_matrix_destroy(point);
    sparse_matrix_destroy(multiplication);
    sparse_matrix_destroy(sum);
    sparse_matrix_destroy(mult);
    sparse_matrix_destroy(matrix);

    return 0;
}