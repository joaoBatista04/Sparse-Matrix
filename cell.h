#ifndef CELL_H
#define CELL_H

typedef struct Cell{
    int column;
    int row;
    float value;
    struct Cell *nextRow;
    struct Cell *nextColumn;
} Cell;

#endif