#ifndef QUERY_H_
#define QUERY_H_

#include "transformation.h"
#include "data_iterator.h"
#include "table.h"

typedef enum QueryType
{
    SELECT,
    INSERT,
    DELETE,
    UPDATE,
} QueryType;


typedef struct SelectQuery
{
    DataIterator *it;
    Predicate *predicate;
    Projection_LinkedList projection;
} SelectQuery;

typedef struct InsertQuery
{
    Table *table;
    Row_LinkedList rows;
} InsertQuery;

typedef struct DeleteQuery
{
    Table *table;
    Predicate *predicate;
} DeleteQuery;

typedef struct UpdateQuery
{
    Table *table;
    Predicate *predicate;
    Transformation_LinkedList transformations;
} UpdateQuery;


Maybe_Row_LinkedList select_query_execute(State *state, SelectQuery *query);
SelectQuery select_query_new(DataIterator *it, Predicate *predicate, Projection_LinkedList projection);
void select_query_free(SelectQuery *query);

Maybe_uint32_t insert_query_execute(State *state, InsertQuery *query);
InsertQuery insert_query_new(Table *table, Row_LinkedList rows);
void insert_query_free(InsertQuery *query);

Maybe_uint32_t delete_query_execute(State *state, DeleteQuery *query);
DeleteQuery delete_query_new(Table *table, Predicate *predicate);
void delete_query_free(DeleteQuery *query);

Maybe_uint32_t update_query_execute(State *state, UpdateQuery *query);
UpdateQuery update_query_new(Table *table, Predicate *predicate, Transformation_LinkedList transformations);
void update_query_free(UpdateQuery *query);


#endif