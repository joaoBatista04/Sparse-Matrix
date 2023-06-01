#include <stdio.h>
#include <stdlib.h>
#include "cell.h"

/**
 * @brief This function creates a new pointer to a cell.
 * 
 * @brief Time Complexity: O(1), because the process does not depend on the size of anything
 * 
 * @param column 
 * The column of the cell
 * @param row 
 * The row of the cell
 * @param value 
 * The value of the cell
 * @param nextRow 
 * The pointer to the next cell in the row
 * @param nextColumn 
 * The pointer to the next cell in the column
 * @return Cell* 
 * The new pointer of Cell created
 */
Cell *cell_creating(int column, int row, matrix_value_type value, Cell *nextRow, Cell *nextColumn){
    Cell *cell = (Cell *)malloc(sizeof(Cell));

    cell->positionColumn = column;
    cell->positionRow = row;
    cell->value = value;

    cell->nextRow = nextRow;
    cell->nextColumn = nextColumn;

    return cell;
}

/**
 * @brief This function destroys a cell, freeing the memory allocated to it.
 * 
 * @brief Time Complexity: O(1), because the process does not depend on the size of anything
 * 
 * @param cell 
 * The pointer of the cell that will be destroyed.
 */
void cell_destroy(Cell *cell){
    cell->nextColumn = NULL;
    cell->nextRow = NULL;

    free(cell);
}