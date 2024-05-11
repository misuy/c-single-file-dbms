#ifndef DATA_ITERATOR_H_
#define DATA_ITERATOR_H_

#include "table.h"

typedef enum DataIteratorType
{
    TABLE,
    JOIN,
} DataIteratorType;

typedef struct JoinIterator
{
    uint8_t is_null;
    Row row;
    struct DataIterator *it;
    uint16_t table_id;
    RowIterator table_it;
    Predicate *predicate;
} JoinIterator;

MAYBE(JoinIterator, JoinIterator)

typedef struct DataIterator
{
    DataIteratorType type;
    union
    {
        RowIterator table;
        JoinIterator join;
    };    
} DataIterator;

LINKED_LIST_HEADER(DataIterator);



DataIterator data_iterator_table_new(State *state, uint16_t table_id);
DataIterator data_iterator_join_new(State *state, DataIterator *it, uint16_t table_id, Predicate *predicate);
Maybe_Nothing data_iterator_next(State *state, DataIterator *it);
void data_iterator_free(DataIterator *it);
uint8_t data_iterator_is_null(DataIterator *it);
Row data_iterator_get_row(DataIterator *it);


#endif