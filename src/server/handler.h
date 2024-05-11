#ifndef HANDLER_H_
#define HANDLER_H_

#include "../protocol/query_executor.h"
#include "../protocol/protocol_types.h"
#include "query.h"
#include "interface.h"

typedef struct VariableTableRelation
{
    char *var_name;
    char *table_name;
} VariableTableRelation;

LINKED_LIST_HEADER(VariableTableRelation);

Maybe_Nothing handle_select_query(State *state, ProtocolQueryResult **result, ProtocolSelectQuery *query);
Maybe_Nothing handle_insert_query(State *state, ProtocolQueryResult **result, ProtocolInsertQuery *query);
Maybe_Nothing handle_delete_query(State *state, ProtocolQueryResult **result, ProtocolDeleteQuery *query);
Maybe_Nothing handle_update_query(State *state, ProtocolQueryResult **result, ProtocolUpdateQuery *query);
Maybe_Nothing handle_create_table_query(State *state, ProtocolQueryResult **result, ProtocolCreateTableQuery *query);
Maybe_Nothing handle_delete_table_query(State *state, ProtocolQueryResult **result, ProtocolDeleteTableQuery *query);

#endif