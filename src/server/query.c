#include "query.h"



Maybe_Row_LinkedList select_query_execute(State *state, SelectQuery *query)
{
    Row row, proj_row;
    Row_LinkedList ret = Row_linked_list_new();
    while (!data_iterator_is_null(query->it))
    {
        printf("select iter\n");
        row = data_iterator_get_row(query->it);
        if (HANDLE(Row_LinkedList, uint8_t, predicate_apply(&row, query->predicate)))
        {
            proj_row = HANDLE(Row_LinkedList, Row, row_apply_projection(&row, query->projection));
            Row_linked_list_push_back(&ret, proj_row);
        }
        HANDLE_NOTHING(Row_LinkedList, data_iterator_next(state, query->it));
    }
    printf("select iter end\n");
    HANDLE_NOTHING(Row_LinkedList, state_sync_tables(state));
    RESULT(Row_LinkedList, ret);
}

Maybe_uint32_t insert_query_execute(State *state, InsertQuery *query)
{
    uint32_t inserted_count = 0;
    Row_LinkedListNode *it = query->rows.head;
    while (it != NULL)
    {
        //printf("insert\n");
        SlotPtr free_slot_ptr = HANDLE(uint32_t, SlotPtr, table_find_free_slot(state, query->table->id));
        HANDLE(uint32_t, Nothing, table_write_row(state, &it->value, free_slot_ptr));
        inserted_count++;
        it = it->next;
    }
    HANDLE_NOTHING(uint32_t, state_sync_tables(state));
    RESULT(uint32_t, inserted_count);
}

Maybe_uint32_t delete_query_execute(State *state, DeleteQuery *query)
{
    SlotPtr_LinkedList to_delete = SlotPtr_linked_list_new();
    RowIterator it = HANDLE(uint32_t, RowIterator, row_iterator_new(state, query->table->id));
    while (!it.is_null)
    {
        if (HANDLE(uint32_t, uint8_t, predicate_apply(&it.row, query->predicate)))
            SlotPtr_linked_list_push_back(&to_delete, it.slot_iterator.ptr);
        HANDLE(uint32_t, Nothing, row_iterator_next(state, &it));
    }
    row_iterator_free(&it);

    uint32_t deleted_count = 0;
    SlotPtr_LinkedListNode *to_delete_it = to_delete.head;
    while (to_delete_it != NULL)
    {
        HANDLE(uint32_t, Nothing, table_delete_row(state, to_delete_it->value));
        deleted_count++;
        to_delete_it = to_delete_it->next;
    }
    SlotPtr_linked_list_free(&to_delete);
    HANDLE_NOTHING(uint32_t, state_sync_tables(state));
    RESULT(uint32_t, deleted_count);
}

Maybe_uint32_t update_query_execute(State *state, UpdateQuery *query)
{
    SlotPtr_LinkedList to_update = SlotPtr_linked_list_new();
    RowIterator it = HANDLE(uint32_t, RowIterator, row_iterator_new(state, query->table->id));
    while (!it.is_null)
    {
        if (HANDLE(uint32_t, uint8_t, predicate_apply(&it.row, query->predicate)))
        {
            SlotPtr_linked_list_push_back(&to_update, it.slot_iterator.ptr);
        }
        HANDLE(uint32_t, Nothing, row_iterator_next(state, &it));
    }
    row_iterator_free(&it);

    uint32_t updated_count = 0;
    SlotPtr_LinkedListNode *to_update_it = to_update.head;
    while (to_update_it != NULL)
    {
        Row row = HANDLE(uint32_t, Row, table_read_row(state, to_update_it->value));
        HANDLE(uint32_t, Nothing, table_delete_row(state, to_update_it->value));
        HANDLE(uint32_t, Nothing, transformations_apply(&query->transformations, &row));
        SlotPtr free_ptr = HANDLE(uint32_t, SlotPtr, table_find_free_slot(state, query->table->id));
        HANDLE(uint32_t, Nothing, table_write_row(state, &row, free_ptr));
        row_free(&row);
        updated_count++;
        to_update_it = to_update_it->next;
    }
    SlotPtr_linked_list_free(&to_update);
    HANDLE_NOTHING(uint32_t, state_sync_tables(state));
    RESULT(uint32_t, updated_count);
}



SelectQuery select_query_new(DataIterator *it, Predicate *predicate, Projection_LinkedList projection)
{
    return (SelectQuery) { .it = it, .predicate = predicate, .projection = projection };
}

void select_query_free(SelectQuery *query)
{
    data_iterator_free(query->it);
    predicate_free(query->predicate);
}



InsertQuery insert_query_new(Table *table, Row_LinkedList rows)
{
    return (InsertQuery) { .table = table, .rows = rows };
}

void insert_query_free(InsertQuery *query)
{
    rows_free(&query->rows);
}



DeleteQuery delete_query_new(Table *table, Predicate *predicate)
{
    return (DeleteQuery) { .table = table, .predicate = predicate };
}

void delete_query_free(DeleteQuery *query)
{
    predicate_free(query->predicate);
}



UpdateQuery update_query_new(Table *table, Predicate *predicate, Transformation_LinkedList transformations)
{
    return (UpdateQuery) { .table = table, .predicate = predicate, .transformations = transformations };
}

void update_query_free(UpdateQuery *query)
{
    predicate_free(query->predicate);
    transformations_free(&query->transformations);
}
