#include "row.h"

#include <glib-object.h>

void row_free_schema(Row *row);
void row_free_cells(Row *row);

LINKED_LIST(Projection)
LINKED_LIST(Row)

Row row_new(ColumnSchema_LinkedList schema)
{
    ColumnSchema_LinkedList new_schema = ColumnSchema_linked_list_new();
    ColumnSchema_LinkedListNode *it = schema.head;
    while (it != NULL)
    {
        char *name_copy = malloc(strlen(it->value.name) + 1);
        memcpy(name_copy, it->value.name, strlen(it->value.name));
        name_copy[strlen(it->value.name)] = 0;
        ColumnSchema_linked_list_push_back(&new_schema, column_schema_new(name_copy, it->value.type));
        it = it->next;
    }

    return (Row) { .schema = new_schema };
}

void row_free_schema(Row *row)
{
    ColumnSchema_LinkedListNode *it = row->schema.head;
    while (it != NULL)
    {
        free(it->value.name);
        it = it->next;
    }
    ColumnSchema_linked_list_free(&row->schema);
}

void row_free_cells(Row *row)
{
    Cell_LinkedListNode *it = row->cells.head;
    while (it != NULL)
    {
        if (cell_is_variable_size(&it->value))
            variable_size_cell_free(&it->value.var_size);
        it = it->next;
    }
    Cell_linked_list_free(&row->cells);
}

void row_free(Row *row)
{
    row_free_cells(row);
    row_free_schema(row);
}

Maybe_Nothing row_set_cells(Row *row, Cell_LinkedList cells)
{
    row_free_cells(row);

    Cell_LinkedListNode *cells_it = cells.head;
    ColumnSchema_LinkedListNode *schema_it = row->schema.head;
    while (cells_it != NULL)
    {
        if (schema_it == NULL) EXCEPTION(Nothing, "cells does not match schema");
        if (cells_it->value.type != schema_it->value.type) EXCEPTION(Nothing, "cells does not match schema");
        cells_it = cells_it->next;
        schema_it = schema_it->next;
    }
    if (schema_it != NULL) EXCEPTION(Nothing, "cells does not match schema");
    row->cells = cells;
    OK(Nothing);
}

Maybe_Nothing row_join_row(Row *row, Row *to_join, char *join_table_name)
{
    Cell_LinkedListNode *cells_it = to_join->cells.head;
    while (cells_it != NULL)
    {
        Cell_linked_list_push_back(&row->cells, HANDLE(Nothing, Cell, cell_clone(&cells_it->value)));

        cells_it = cells_it->next;
    }

    ColumnSchema_LinkedListNode *schema_it = to_join->schema.head;
    while (schema_it != NULL)
    {
        char *new_name;
        if (join_table_name == 0)
        {
            new_name = malloc(strlen(schema_it->value.name) + 1);
            memcpy(new_name, schema_it->value.name, strlen(schema_it->value.name));
            new_name[strlen(schema_it->value.name)] = 0;
        }
        else
        {
            new_name = malloc(strlen(join_table_name) + strlen(schema_it->value.name) + 2);
            memcpy(new_name, join_table_name, strlen(join_table_name));
            new_name[strlen(join_table_name)] = '.';
            memcpy(new_name + strlen(join_table_name) + 1, schema_it->value.name, strlen(schema_it->value.name));
            new_name[strlen(join_table_name) + strlen(schema_it->value.name) + 1] = 0;
        }

        ColumnSchema_linked_list_push_back(&row->schema, column_schema_new(new_name, schema_it->value.type));

        schema_it = schema_it->next;
    }

    OK(Nothing);
}

Projection projection_new(char *column_name)
{
    return (Projection) { .column_name = column_name };
}

Maybe_Row row_apply_projection(Row *row, Projection_LinkedList projection)
{
    Cell_LinkedList ret = Cell_linked_list_new();
    ColumnSchema_LinkedList schema = ColumnSchema_linked_list_new();
    if (Projection_linked_list_is_empty(&projection))
    {
        Cell_LinkedListNode *it = row->cells.head;
        ColumnSchema_LinkedListNode *schema_it = row->schema.head;
        while (it != NULL)
        {
            Cell cell = it->value;
            Cell_linked_list_push_back(&ret, HANDLE(Row, Cell, cell_clone(&cell)));
            ColumnSchema_linked_list_push_back(&schema, column_schema_new(g_strdup(schema_it->value.name), schema_it->value.type));
            it = it->next;
            schema_it = schema_it->next;
        }
    }
    else
    {
        Projection_LinkedListNode *it = projection.head;
        while (it != NULL)
        {
            Cell cell = HANDLE(Row, Cell, row_get_cell_by_name(row, it->value.column_name));
            Cell_linked_list_push_back(&ret, HANDLE(Row, Cell, cell_clone(&cell)));
            ColumnSchema_linked_list_push_back(&schema, column_schema_new(g_strdup(it->value.column_name), cell.type));
            it = it->next;
        }
    }
    Row ret_row = (Row) { .schema = schema, .cells = ret };
    RESULT(Row, ret_row);
}

Maybe_Cell row_get_cell_by_name(Row *row, char *name)
{
    Cell_LinkedListNode *cells_it = row->cells.head;
    ColumnSchema_LinkedListNode *schema_it = row->schema.head;
    while (schema_it != NULL)
    {
        // printf("schema: %s\nname: %s\n", schema_it->value.name, name);
        if (strcmp(schema_it->value.name, name) == 0) RESULT(Cell, cells_it->value);
        schema_it = schema_it->next;
        cells_it = cells_it->next;
    }
    EXCEPTION(Cell, "get_cell there is no cell with provided name");
}

Maybe_Nothing row_set_cell_by_name(Row *row, Cell cell, char *name)
{
    Cell_LinkedListNode *cells_it = row->cells.head;
    ColumnSchema_LinkedListNode *schema_it = row->schema.head;
    while (schema_it != NULL)
    {
        if (strcmp(schema_it->value.name, name) == 0)
        {
            if (schema_it->value.type != cell.type)
                if (!(column_type_is_integer(schema_it->value.type) & column_type_is_integer(cell.type)))
                    EXCEPTION(Nothing, "illegal cell type");
            if (cell_is_variable_size(&cells_it->value)) variable_size_cell_free(&cells_it->value.var_size);
            cells_it->value = cell;
            OK(Nothing);
        }
        schema_it = schema_it->next;
        cells_it = cells_it->next;
    }
    EXCEPTION(Nothing, "set_cell there is no cell with provided name");
}

Maybe_Row row_new_by_values(ColumnSchema_LinkedList *schema, void **values)
{
    Row row = row_new(*schema);
    Cell_LinkedList cells = Cell_linked_list_new();
    ColumnSchema_LinkedListNode *it = schema->head;
    uint16_t offset = 0;
    while (it != NULL)
    {
        CellValue value = HANDLE(Row, CellValue, cell_value_new(it->value.type, values[offset]));
        Cell cell_value = HANDLE(Row, Cell, cell_from_value(&value));
        Cell_linked_list_push_back(&cells, cell_value);
        it = it->next;
        offset++;
    }
    HANDLE_NOTHING(Row, row_set_cells(&row, cells));
    RESULT(Row, row);
}

Row_LinkedList rows_new(void)
{
    return Row_linked_list_new();
}

void rows_add_row(Row_LinkedList *rows, Row row)
{
    Row_linked_list_push_back(rows, row);
}

void rows_free(Row_LinkedList *rows)
{
    Row_LinkedListNode *it = rows->head;
    while (it != NULL)
    {
        row_free(&it->value);
        it = it->next;
    }
    Row_linked_list_free(rows);
}

void row_print(Row *row)
{
    Cell_LinkedList cells = row->cells;
    Cell_LinkedListNode *it = cells.head;
    while (it != NULL)
    {
        CellValue value = TRY(CellValue, cell_get_value(&it->value));
        TRY(Nothing, cell_value_print(&value));
        printf(" ");
        cell_value_free(&value);
        it = it->next;
    }
}

void cells_print(Cell_LinkedList cells)
{
    Cell_LinkedListNode *it = cells.head;
    while (it != NULL)
    {
        CellValue value = TRY(CellValue, cell_get_value(&it->value));
        TRY(Nothing, cell_value_print(&value));
        printf(" ");
        cell_value_free(&value);
        it = it->next;
    }
}