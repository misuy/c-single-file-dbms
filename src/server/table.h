#ifndef TABLE_H_
#define TABLE_H_

#include "inttypes.h"

#include "util.h"
#include "file.h"
#include "page.h"
#include "cell.h"
#include "predicate.h"

#define ROOT_TABLE_NAME "ROOT"
#define FREE_TABLE_NAME "FREE"
#define POOL_TABLE_NAME "POOL"


typedef struct RowIterator
{
    uint8_t is_null;
    uint8_t switched_to_full;
    SlotIterator slot_iterator;
    Row row;
    char *table_name;
} RowIterator;

MAYBE(RowIterator, RowIterator)


typedef struct Table
{
    uint16_t id;
    char* name;
    uint8_t is_system;
    uint8_t is_data;
    ColumnSchema_LinkedList schema;
    uint16_t row_size;
    uint32_t first_page_id;
    uint32_t first_full_page_id;
} Table;

MAYBE(Table, Table)
MAYBE(Table*, TablePtr)


typedef struct State
{
    File* file;
    Table* tables[MAX_TABLES_COUNT];
    uint16_t_LinkedList to_sync;
    uint8_t is_syncing;
} State;
MAYBE(State, State)



Maybe_uint16_t get_id_for_new_table(State *);

Table * table_new(uint16_t, char *, uint8_t, uint8_t, ColumnSchema_LinkedList, uint16_t, uint32_t, uint32_t);
void table_free(State *, uint16_t);

Maybe_State state_init(char *);
Maybe_Nothing state_sync_tables(State *);

Maybe_TablePtr root_table_get_table_by_name(State *, char *);
Maybe_Nothing root_table_delete_table(State *state, Table *table);

Maybe_TablePtr table_open_by_id(State *state, uint16_t id);
Maybe_Nothing table_update(State *state, uint16_t id);

Maybe_SlotPtr table_find_free_slot(State *, uint16_t);

Maybe_Nothing table_write_row(State *, Row *, SlotPtr);
Maybe_Row table_read_row(State *, SlotPtr);
Maybe_Nothing table_delete_row(State *, SlotPtr);

Maybe_RowIterator row_iterator_new(State *, uint16_t);
Maybe_Nothing row_iterator_next(State *, RowIterator *);
void row_iterator_free(RowIterator *);

#endif