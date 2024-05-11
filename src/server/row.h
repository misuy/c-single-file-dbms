#ifndef ROW_H_
#define ROW_H_

#include "cell.h"

typedef struct Row
{
    ColumnSchema_LinkedList schema;
    Cell_LinkedList cells;
} Row;

LINKED_LIST_HEADER(Row);
MAYBE(Row_LinkedList, Row_LinkedList)

typedef struct Projection
{
    char *column_name;
} Projection;

LINKED_LIST_HEADER(Projection);
MAYBE(Projection_LinkedList, Projection_LinkedList)


Row row_new(ColumnSchema_LinkedList);
Maybe_Row row_new_by_values(ColumnSchema_LinkedList *, void **values);
Maybe_Nothing row_join_row(Row *row, Row *to_join, char *row_table_name);
void row_free(Row *);
void row_print(Row *row);
void cells_print(Cell_LinkedList cells);

Maybe_Nothing row_set_cells(Row *row, Cell_LinkedList);
Maybe_Cell row_get_cell_by_name(Row *, char *);
Maybe_Nothing row_set_cell_by_name(Row *, Cell, char *);

Row_LinkedList rows_new(void);
void rows_add_row(Row_LinkedList *rows, Row row);
void rows_free(Row_LinkedList *rows);

Projection projection_new(char *column_name);
Maybe_Row row_apply_projection(Row *row, Projection_LinkedList projection);

#endif