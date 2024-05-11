#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "table.h"
#include "predicate.h"
#include "transformation.h"
#include "query.h"

Maybe_TablePtr table_open(State *, char *);
Maybe_TablePtr table_create(State *, char *, ColumnSchema_LinkedList *);
Maybe_Nothing table_delete(State *state, Table *table);
void table_close(State *, Table *);

Maybe_State storage_connect(char *name);
Maybe_Nothing storage_disconnect(State *state);

#endif