#include "interface.h"


Maybe_TablePtr table_open(State *state, char *name)
{
    Table *table = HANDLE(TablePtr, TablePtr, root_table_get_table_by_name(state, name));
    RESULT(TablePtr, table);
}

Maybe_TablePtr table_create(State *state, char *name, ColumnSchema_LinkedList *schema)
{
    uint16_t id = HANDLE(TablePtr, uint16_t, get_id_for_new_table(state));
    Table* table = table_new(id, name, 0, !column_schema_is_variable_size(schema), *schema, column_schema_get_size(schema), 0, 0);
    state->tables[id] = table;
    HANDLE_NOTHING(TablePtr, table_update(state, id));
    HANDLE_NOTHING(TablePtr, state_sync_tables(state));
    RESULT(TablePtr, table);
}

Maybe_Nothing table_delete(State *state, Table *table)
{
    DeleteQuery delete_query = delete_query_new(table, 0);
    HANDLE(Nothing, uint32_t, delete_query_execute(state, &delete_query));
    delete_query_free(&delete_query);

    HANDLE_NOTHING(Nothing, root_table_delete_table(state, table));
    table_close(state, table);
    HANDLE_NOTHING(Nothing, state_sync_tables(state));
    OK(Nothing);
}

void table_close(State *state, Table *table)
{
    table_free(state, table->id);
}


Maybe_State storage_connect(char *name)
{
    State state = HANDLE(State, State, state_init(name));
    RESULT(State, state);
}

Maybe_Nothing storage_disconnect(State *state)
{
    HANDLE_NOTHING(Nothing, state_sync_tables(state));
    HANDLE_NOTHING(Nothing, file_close(state->file));
    for (uint16_t i=0; i<MAX_TABLES_COUNT; i++)
    {
        if (state->tables[i] != 0)
            free(state->tables[i]);
    }
    free(state->file);
    OK(Nothing);
}