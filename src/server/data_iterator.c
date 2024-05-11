#include "data_iterator.h"


LINKED_LIST(DataIterator);



Maybe_JoinIterator join_iterator_new(State *state, DataIterator *it, uint16_t table_id, Predicate *predicate);
Maybe_Nothing join_iterator_next(State *state, JoinIterator *it);
void join_iterator_free(JoinIterator *it);



Maybe_Nothing join_iterator_next(State *state, JoinIterator *it)
{
    join_iterator_next_begin:
    if (it->is_null)
        OK(Nothing);

    HANDLE_NOTHING(Nothing, row_iterator_next(state, &it->table_it));

    if (it->table_it.is_null)
    {
        row_iterator_free(&it->table_it);
        HANDLE_NOTHING(Nothing, data_iterator_next(state, it->it));
        if (!data_iterator_is_null(it->it))
        {
            it->table_it = HANDLE(Nothing, RowIterator, row_iterator_new(state, it->table_id));
        }
        else
            it->is_null = 1;
    }

    if (!it->table_it.is_null)
    {
        row_free(&it->row);
        it->row = row_new(ColumnSchema_linked_list_new());
        Row row = data_iterator_get_row(it->it);
        HANDLE_NOTHING(Nothing, row_join_row(&it->row, &row, 0));
        HANDLE_NOTHING(Nothing, row_join_row(&it->row, &it->table_it.row, 0));
    }
    else
        it->is_null = 1;
    
    if (!it->is_null)
    {
        if (!HANDLE(Nothing, uint8_t, predicate_apply(&it->row, it->predicate)))
            goto join_iterator_next_begin;
    }
    
    OK(Nothing);
}

Maybe_JoinIterator join_iterator_new(State *state, DataIterator *it, uint16_t table_id, Predicate *predicate)
{
    JoinIterator join_it = (JoinIterator) { .it = it, .is_null = 0, .table_id = table_id, .predicate = predicate };
    
    join_it.table_it = HANDLE(JoinIterator, RowIterator, row_iterator_new(state, join_it.table_id));

    if ((!join_it.table_it.is_null) & (!data_iterator_is_null(join_it.it)))
    {
        Row row = data_iterator_get_row(join_it.it);
        join_it.row = row_new(ColumnSchema_linked_list_new());
        HANDLE_NOTHING(JoinIterator, row_join_row(&join_it.row, &row, 0));
        HANDLE_NOTHING(JoinIterator, row_join_row(&join_it.row, &join_it.table_it.row, 0));
    }
    else
        join_it.is_null = 1;

    if (!join_it.is_null)
    {
        if (!HANDLE(JoinIterator, uint8_t, predicate_apply(&join_it.row, join_it.predicate)))
            join_iterator_next(state, &join_it);
    }
    
    RESULT(JoinIterator, join_it);
}

void join_iterator_free(JoinIterator *it)
{
    if (it != NULL)
    {
        if (it->it != NULL)
        {
            data_iterator_free(it->it);
        }
        if (!it->is_null)
        {
            row_iterator_free(&it->table_it);
            row_free(&it->row);
        }
        it->is_null = 1;
    }
}



Maybe_Nothing data_iterator_next(State *state, DataIterator *it)
{
    switch (it->type)
    {
        case TABLE:
            printf("row it next\n");
            HANDLE_NOTHING(Nothing, row_iterator_next(state, &it->table));
            break;
        case JOIN:
            printf("join it next\n");
            HANDLE_NOTHING(Nothing, join_iterator_next(state, &it->join));
            break;
    }
    OK(Nothing);
}

uint8_t data_iterator_is_null(DataIterator *it)
{
    switch (it->type)
    {
        case TABLE:
            return it->table.is_null;
        case JOIN:
            return it->join.is_null;
        default:
            return 1;
    }
}

Row data_iterator_get_row(DataIterator *it)
{
    switch (it->type)
    {
        case TABLE:
            return it->table.row;
        case JOIN:
            return it->join.row;
        default:
            return row_new(ColumnSchema_linked_list_new());
    }
}

void data_iterator_free(DataIterator *it)
{
    switch (it->type)
    {
        case TABLE:
            row_iterator_free(&it->table);
            break;
        case JOIN:
            join_iterator_free(&it->join);
            break;
    }
}

DataIterator data_iterator_table_new(State *state, uint16_t table_id)
{
    DataIterator it = (DataIterator) { .type = TABLE, .table = TRY(RowIterator, row_iterator_new(state, table_id)) };
    return it;
}

DataIterator data_iterator_join_new(State *state, DataIterator *it, uint16_t table_id, Predicate *predicate)
{
    DataIterator data_it = (DataIterator) { .type = JOIN, .join = TRY(JoinIterator, join_iterator_new(state, it, table_id, predicate)) };
    return data_it;
}
