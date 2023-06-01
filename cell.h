#ifndef CELL_H
#define CELL_H

typedef struct Cell{
    int positionColumn;
    int positionRow;
    float value;
    struct Cell *nextRow;
    struct Cell *nextColumn;
} Cell;

typedef float matrix_value_type;

//Allocation functions
Cell *cell_creating(int column, int row, matrix_value_type value, Cell *nextRow, Cell *nextColumn);
void cell_destroy(Cell *cell);

#endif