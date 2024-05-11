#include "handler.h"

LINKED_LIST(VariableTableRelation);

Maybe_Nothing parse_column_name(ProtocolColumnPtr *column_ptr, VariableTableRelation_LinkedList rel, char **name);
Maybe_PredicatePtr parse_predicate(ProtocolExpression *expression, VariableTableRelation_LinkedList rel);
Maybe_Projection_LinkedList parse_projection(ProtocolProjection *projection, VariableTableRelation_LinkedList rel);
Maybe_Transformation parse_transformation(ProtocolAssignment *assignment, VariableTableRelation_LinkedList rel);
Maybe_Transformation_LinkedList parse_transformations(GPtrArray *arr, VariableTableRelation_LinkedList rel);
ColumnSchema_LinkedList parse_table_schema(GPtrArray *arr);
Maybe_DataIterator parse_data_iterator(State *state, ProtocolDataSource *source, VariableTableRelation_LinkedList *rel, uint8_t is_table);
Maybe_Row_LinkedList parse_rows(State *state, Table *table, GPtrArray *arr);

void rows_to_data(Row_LinkedList rows, GPtrArray *arr);
void schema_to_defs(ColumnSchema_LinkedList schema, GPtrArray *arr);

Maybe_Nothing parse_column_name(ProtocolColumnPtr *column_ptr, VariableTableRelation_LinkedList rel, char **name)
{
    char *table_name = NULL;
    VariableTableRelation_LinkedListNode *rel_it = rel.head;
    while (rel_it != NULL)
    {
        if (strcmp(column_ptr->table_name->name, rel_it->value.var_name) == 0)
        {
            table_name = rel_it->value.table_name;
            break;
        }
        rel_it = rel_it->next;
    }
    if (table_name == NULL)
        EXCEPTION(Nothing, "unknown variable");

    *name = malloc(strlen(column_ptr->column_name->name) + strlen(table_name) + 2);
    uint32_t offset = 0;
    memcpy(*name, table_name, strlen(table_name));
    offset += strlen(table_name);
    (*name)[offset] = '.';
    offset++;
    memcpy(*name + offset, column_ptr->column_name->name, strlen(column_ptr->column_name->name));
    offset += strlen(column_ptr->column_name->name);
    (*name)[offset] = 0;

    OK(Nothing);
}


Maybe_PredicatePtr parse_predicate(ProtocolExpression *expression, VariableTableRelation_LinkedList rel)
{
    Operand_LinkedList operands = Operand_linked_list_new();
    
    for (uint32_t i = 0; i < expression->list->len; i++)
    {
        ProtocolExpressionNode *node = g_ptr_array_index(expression->list, i);
        switch (node->type)
        {
            case PROTOCOL_EXPRESSION_NODE_TYPE_OPERAND:
                ProtocolExpressionOperand *operand = node->value->operand;
                switch (operand->type)
                {
                    case PROTOCOL_EXPRESSION_OPERAND_TYPE_CONSTANT:
                        CellValue value;
                        switch (operand->operand->constant_operand->type)
                        {
                            case PROTOCOL_CONSTANT_TYPE_BOOL:
                                value = HANDLE(PredicatePtr, CellValue, cell_value_new(INT_32, &operand->operand->constant_operand->value->bool_constant));
                                break;
                            case PROTOCOL_CONSTANT_TYPE_INTEGER:
                                value = HANDLE(PredicatePtr, CellValue, cell_value_new(INT_32, &operand->operand->constant_operand->value->integer_constant));
                                break;
                            case PROTOCOL_CONSTANT_TYPE_FLOAT:
                                float float_val = (float) operand->operand->constant_operand->value->float_constant;
                                value = HANDLE(PredicatePtr, CellValue, cell_value_new(FLOAT, &float_val));
                                break;
                            case PROTOCOL_CONSTANT_TYPE_STRING:
                                value = HANDLE(PredicatePtr, CellValue, cell_value_new(STRING, &operand->operand->constant_operand->value->string_constant));
                                break;
                        }
                        Operand_linked_list_push_front(&operands, operand_new_by_value(value));
                        break;

                    case PROTOCOL_EXPRESSION_OPERAND_TYPE_COLUMN_PTR:
                        char *name;
                        HANDLE_NOTHING(PredicatePtr, parse_column_name(operand->operand->column_ptr_operand, rel, &name));
                        Operand_linked_list_push_front(&operands, operand_new_by_ptr(name));
                        break;
                }
                break;

            case PROTOCOL_EXPRESSION_NODE_TYPE_OPERATOR:
                Predicate *predicate;
                ProtocolExpressionOperator *operator = node->value->operator;
                Maybe_uint8_t (*operatorr) (CellValue, CellValue);
                switch (operator->type)
                {
                    case PROTOCOL_OPERATOR_TYPE_NOT:
                        operatorr = operator_not;
                        break;
                    case PROTOCOL_OPERATOR_TYPE_AND:
                        operatorr = operator_and;
                        break;
                    case PROTOCOL_OPERATOR_TYPE_OR:
                        operatorr = operator_or;
                        break;
                    case PROTOCOL_OPERATOR_TYPE_EQUAL:
                        operatorr = operator_equal;
                        break;
                    case PROTOCOL_OPERATOR_TYPE_NOT_EQUAL:
                        operatorr = operator_not_equal;
                        break;
                    case PROTOCOL_OPERATOR_TYPE_LESS:
                        operatorr = operator_less;
                        break;
                    case PROTOCOL_OPERATOR_TYPE_NOT_LESS:
                        operatorr = operator_not_less;
                        break;
                    case PROTOCOL_OPERATOR_TYPE_GREATER:
                        operatorr = operator_greater;
                        break;
                    case PROTOCOL_OPERATOR_TYPE_NOT_GREATER:
                        operatorr = operator_not_greater;
                        break;
                    case PROTOCOL_OPERATOR_TYPE_SUBSTR:
                        operatorr = operator_substr;
                        break;
                }
                if (operator->type == PROTOCOL_OPERATOR_TYPE_NOT)
                {
                    Operand op = HANDLE(PredicatePtr, Operand, Operand_linked_list_pop_front(&operands));
                    predicate = predicate_new(operatorr, op, op);
                }
                else
                {
                    Operand right_op = HANDLE(PredicatePtr, Operand, Operand_linked_list_pop_front(&operands));
                    Operand left_op = HANDLE(PredicatePtr, Operand, Operand_linked_list_pop_front(&operands));
                    predicate = predicate_new(operatorr, left_op, right_op);
                }
                Operand_linked_list_push_front(&operands, operand_new_by_predicate(predicate));
                break;
        }
    }

    Predicate *predicate;
    Operand op = HANDLE(PredicatePtr, Operand, Operand_linked_list_pop_front(&operands));
    if ((op.type == VALUE) | (op.type == VALUE_PTR))
        predicate = predicate_new(operator_ok, op, op);
    else
        predicate = op.predicate;
    
    RESULT(PredicatePtr, predicate);
}


Maybe_Projection_LinkedList parse_projection(ProtocolProjection *projection, VariableTableRelation_LinkedList rel)
{
    Projection_LinkedList ret = Projection_linked_list_new();
    for (uint32_t i = 0; i < projection->column_ptrs->len; i++)
    {
        char *name;
        HANDLE_NOTHING(Projection_LinkedList, parse_column_name(g_ptr_array_index(projection->column_ptrs, i), rel, &name));
        Projection_linked_list_push_back(&ret, projection_new(name));
    }
    printf("projection end\n");
    RESULT(Projection_LinkedList, ret);
}


Maybe_Transformation parse_transformation(ProtocolAssignment *assignment, VariableTableRelation_LinkedList rel)
{
    char *name;
    HANDLE_NOTHING(Transformation, parse_column_name(assignment->to, rel, &name));

    TransformationOperand operand;
    switch (assignment->what->type)
    {
        case PROTOCOL_EXPRESSION_OPERAND_TYPE_CONSTANT:
            switch (assignment->what->operand->constant_operand->type)
            {
                case PROTOCOL_CONSTANT_TYPE_BOOL:
                    operand = transformation_operand_new_by_value(HANDLE(Transformation, CellValue, cell_value_new(INT_32, &assignment->what->operand->constant_operand->value->bool_constant)));
                    break;
                case PROTOCOL_CONSTANT_TYPE_INTEGER:
                    operand = transformation_operand_new_by_value(HANDLE(Transformation, CellValue, cell_value_new(INT_32, &assignment->what->operand->constant_operand->value->integer_constant)));
                    break;
                case PROTOCOL_CONSTANT_TYPE_FLOAT:
                    operand = transformation_operand_new_by_value(HANDLE(Transformation, CellValue, cell_value_new(FLOAT, &assignment->what->operand->constant_operand->value->float_constant)));
                    break;
                case PROTOCOL_CONSTANT_TYPE_STRING:
                    operand = transformation_operand_new_by_value(HANDLE(Transformation, CellValue, cell_value_new(STRING, &assignment->what->operand->constant_operand->value->string_constant)));
                    break;
            }
            break;
        
        case PROTOCOL_EXPRESSION_OPERAND_TYPE_COLUMN_PTR:
            char *op_name;
            HANDLE_NOTHING(Transformation, parse_column_name(assignment->what->operand->column_ptr_operand, rel, &op_name));
            operand = transformation_operand_new_by_ptr(op_name);
            break;
    }

    RESULT(Transformation, transformation_new(arg_ptr_new(name), operand, transformation_ok));
}


Maybe_Transformation_LinkedList parse_transformations(GPtrArray *arr, VariableTableRelation_LinkedList rel)
{
    Transformation_LinkedList ret = Transformation_linked_list_new();
    for (uint32_t i = 0; i < arr->len; i++)
    {
        Transformation transformation = HANDLE(Transformation_LinkedList, Transformation, parse_transformation(g_ptr_array_index(arr, i), rel));
        Transformation_linked_list_push_back(&ret, transformation);
    }
    RESULT(Transformation_LinkedList, ret);
}


ColumnSchema_LinkedList parse_table_schema(GPtrArray *arr)
{
    ColumnSchema_LinkedList ret = ColumnSchema_linked_list_new();
    for (uint32_t i = 0; i < arr->len; i++)
    {
        ProtocolColumnDef *def = g_ptr_array_index(arr, i);
        ColumnType type;
        switch (def->type)
        {
            case PROTOCOL_DATA_TYPE_BOOL:
                type = BOOL;
                break;
            case PROTOCOL_DATA_TYPE_UINT_8:
                type = UINT_8;
                break;
            case PROTOCOL_DATA_TYPE_UINT_16:
                type = UINT_16;
                break;
            case PROTOCOL_DATA_TYPE_UINT_32:
                type = UINT_32;
                break;
            case PROTOCOL_DATA_TYPE_INT_32:
                type = INT_32;
                break;
            case PROTOCOL_DATA_TYPE_FLOAT:
                type = FLOAT;
                break;
            case PROTOCOL_DATA_TYPE_STRING:
                type = STRING;
                break;
        }
        ColumnSchema_linked_list_push_back(&ret, column_schema_new(def->name->name, type));
    }
    return ret;
}


Maybe_DataIterator parse_data_iterator(State *state, ProtocolDataSource *source, VariableTableRelation_LinkedList *rel, uint8_t is_table)
{
    if (is_table)
        if (source->list->len != 1)
            EXCEPTION(DataIterator, "not a table iterator");

    DataIterator_LinkedList list = DataIterator_linked_list_new();
    for (uint32_t i = 0; i < source->list->len; i++)
    {
        ProtocolDataSourceNode *node = g_ptr_array_index(source->list, i);
        DataIterator it;
        switch (node->type)
        {
            case PROTOCOL_DATA_SOURCE_NODE_TYPE_TABLE:
                Table *table = HANDLE(DataIterator, TablePtr, table_open(state, node->value->table->table_name->name));
                if (table == 0)
                    EXCEPTION(DataIterator, "table is not exists");
                it = data_iterator_table_new(state, table->id);
                VariableTableRelation_linked_list_push_back(rel, (VariableTableRelation) { .table_name = node->value->table->table_name->name, .var_name = node->value->table->variable_name->name });
                break;
            
            case PROTOCOL_DATA_SOURCE_NODE_TYPE_EXPRESSION:
                Predicate *predicate = HANDLE(DataIterator, PredicatePtr, parse_predicate(node->value->expression, *rel));
                DataIterator right_it = HANDLE(DataIterator, DataIterator, DataIterator_linked_list_pop_front(&list));
                if (right_it.type != TABLE)
                    EXCEPTION(DataIterator, "not a table iterator");
                DataIterator left_it = HANDLE(DataIterator, DataIterator, DataIterator_linked_list_pop_front(&list));
                DataIterator *new_it = malloc(sizeof(DataIterator));
                *new_it = left_it;
                Table *right_table = HANDLE(DataIterator, TablePtr, table_open(state, right_it.table.table_name));
                if (right_table == 0)
                    EXCEPTION(DataIterator, "table is not exists");
                it = data_iterator_join_new(state, new_it, right_table->id, predicate);
                data_iterator_free(&right_it);
                break;
        }
        DataIterator_linked_list_push_front(&list, it);
    }
    RESULT(DataIterator, HANDLE(DataIterator, DataIterator, DataIterator_linked_list_pop_front(&list)));
}


Maybe_Row_LinkedList parse_rows(State *state, Table *table, GPtrArray *arr)
{
    printf("parse rows\n");
    Row_LinkedList ret = Row_linked_list_new();
    for (uint32_t i = 0; i < arr->len; i++)
    {
        GPtrArray *arr2 = g_ptr_array_index(arr, i);
        void **row = malloc(sizeof(void *) * arr2->len);
        for (uint32_t j = 0; j < arr2->len; j++)
        {
            ProtocolConstant *constant = g_ptr_array_index(arr2, j);
            switch (constant->type)
            {
                case PROTOCOL_CONSTANT_TYPE_BOOL:
                    row[j] = malloc(sizeof(uint8_t));
                    *((uint8_t *) row[j]) = constant->value->bool_constant;
                    break;
                case PROTOCOL_CONSTANT_TYPE_INTEGER:
                    row[j] = malloc(sizeof(int32_t));
                    *((int32_t *) row[j]) = constant->value->integer_constant;
                    break;
                case PROTOCOL_CONSTANT_TYPE_FLOAT:
                    row[j] = malloc(sizeof(float));
                    *((float *) row[j]) = constant->value->float_constant;
                    break;
                case PROTOCOL_CONSTANT_TYPE_STRING:
                    char *str = malloc(sizeof(char) * strlen(constant->value->string_constant) + 1);
                    memcpy(str, constant->value->string_constant, strlen(constant->value->string_constant));
                    str[strlen(constant->value->string_constant)] = 0;
                    row[j] = malloc(sizeof(char *));
                    *((char **) row[j]) = str;
                    break;
            }
        }
        Row add = HANDLE(Row_LinkedList, Row, row_new_by_values(&table->schema, row));
        Row_linked_list_push_back(&ret, add);
    }
    RESULT(Row_LinkedList, ret);
}


void rows_to_data(Row_LinkedList rows, GPtrArray *arr)
{
    Row_LinkedListNode *it = rows.head;
    while (it != NULL)
    {
        GPtrArray *arr1 = g_ptr_array_new();
        Cell_LinkedListNode *it1 = it->value.cells.head;
        while (it1 != NULL)
        {
            ProtocolConstant *constant;
            CellValue value = TRY(CellValue, cell_get_value(&it1->value));
            switch (value.type)
            {
                case BOOL:
                    constant = g_object_new
                    (
                        TYPE_PROTOCOL_CONSTANT,
                        "type", PROTOCOL_CONSTANT_TYPE_BOOL,
                        "value", g_object_new
                        (
                            TYPE_PROTOCOL_CONSTANT_UNION,
                            "bool_constant", value.bool,
                            NULL
                        ),
                        NULL
                    );
                    break;

                case UINT_8:
                case UINT_16:
                case UINT_32:
                case INT_32:
                    constant = g_object_new
                    (
                        TYPE_PROTOCOL_CONSTANT,
                        "type", PROTOCOL_CONSTANT_TYPE_INTEGER,
                        "value", g_object_new
                        (
                            TYPE_PROTOCOL_CONSTANT_UNION,
                            "integer_constant", cell_value_to_int32_t(&value),
                            NULL
                        ),
                        NULL
                    );
                    break;
                
                case FLOAT:
                    constant = g_object_new
                    (
                        TYPE_PROTOCOL_CONSTANT,
                        "type", PROTOCOL_CONSTANT_TYPE_FLOAT,
                        "value", g_object_new
                        (
                            TYPE_PROTOCOL_CONSTANT_UNION,
                            "float_constant", value._float,
                            NULL
                        ),
                        NULL
                    );
                    break;

                case STRING:
                    constant = g_object_new
                    (
                        TYPE_PROTOCOL_CONSTANT,
                        "type", PROTOCOL_CONSTANT_TYPE_STRING,
                        "value", g_object_new
                        (
                            TYPE_PROTOCOL_CONSTANT_UNION,
                            "string_constant", g_strdup(value.string),
                            NULL
                        ),
                        NULL
                    );
                    break;
                default:
                    return;
            }
            g_ptr_array_add(arr1, constant);
            it1 = it1->next;
        }
        g_ptr_array_add(arr, arr1);
        it = it->next;
    }
}


void schema_to_defs(ColumnSchema_LinkedList schema, GPtrArray *arr)
{
    ColumnSchema_LinkedListNode *it = schema.head;
    while (it != NULL)
    {
        ProtocolDataType type;
        switch (it->value.type)
        {
            case BOOL:
                type = PROTOCOL_DATA_TYPE_BOOL;
                break;
            case UINT_8:
                type = PROTOCOL_DATA_TYPE_UINT_8;
                break;
            case UINT_16:
                type = PROTOCOL_DATA_TYPE_UINT_16;
                break;
            case UINT_32:
                type = PROTOCOL_DATA_TYPE_UINT_32;
                break;
            case INT_32:
                type = PROTOCOL_DATA_TYPE_INT_32;
                break;
            case FLOAT:
                type = PROTOCOL_DATA_TYPE_FLOAT;
                break;
            case STRING:
                type = PROTOCOL_DATA_TYPE_STRING;
                break;
            default:
                return;
        }
        ProtocolColumnDef *def = g_object_new
        (
            TYPE_PROTOCOL_COLUMN_DEF,
            "type", type,
            "name", g_object_new
            (
                TYPE_PROTOCOL_VARIABLE,
                "name", g_strdup(it->value.name),
                NULL
            ),
            NULL
        );
        g_ptr_array_add(arr, def);
        it = it->next;
    }
}


Maybe_Nothing handle_select_query(State *state, ProtocolQueryResult **result, ProtocolSelectQuery *query);
Maybe_Nothing handle_insert_query(State *state, ProtocolQueryResult **result, ProtocolInsertQuery *query);
Maybe_Nothing handle_delete_query(State *state, ProtocolQueryResult **result, ProtocolDeleteQuery *query);
Maybe_Nothing handle_update_query(State *state, ProtocolQueryResult **result, ProtocolUpdateQuery *query);
Maybe_Nothing handle_create_table_query(State *state, ProtocolQueryResult **result, ProtocolCreateTableQuery *query);
Maybe_Nothing handle_delete_table_query(State *state, ProtocolQueryResult **result, ProtocolDeleteTableQuery *query);


Maybe_Nothing handle_select_query(State *state, ProtocolQueryResult **result, ProtocolSelectQuery *query)
{
    printf("parse data source\n");
    VariableTableRelation_LinkedList rel = VariableTableRelation_linked_list_new();
    DataIterator data_it = HANDLE(Nothing, DataIterator, parse_data_iterator(state, query->data_source, &rel, 0));
    
    printf("parse predicate\n");
    Predicate *predicate = 0;
    if (query->__isset_filter)
        predicate = HANDLE(Nothing, PredicatePtr, parse_predicate(query->filter->expression, rel));

    printf("parse projection\n");
    Projection_LinkedList projection;
    if (query->__isset_projection)
        projection = HANDLE(Nothing, Projection_LinkedList, parse_projection(query->projection, rel));
    else
        projection = Projection_linked_list_new();
    
    printf("query execute\n");
    SelectQuery select_query = select_query_new(&data_it, predicate, projection);
    Row_LinkedList res = HANDLE(Nothing, Row_LinkedList, select_query_execute(state, &select_query));
    printf("select end\n");
    select_query_free(&select_query);
    printf("is list empty: %d\n", Row_linked_list_is_empty(&res));

    printf("set smth\n");
    if (res.head != 0)
        schema_to_defs(res.head->value.schema, (*result)->select->defs);
    rows_to_data(res, (*result)->select->data);

    g_object_set
    (
        (*result)->select,
        "count", (*result)->select->data->len,
        NULL
    );

    printf("selected: %d\n", (*result)->select->data->len);

    g_object_set
    (
        *result,
        "type", PROTOCOL_QUERY_TYPE_SELECT,
        "code", PROTOCOL_QUERY_RESULT_CODE_OK,
        NULL
    );

    OK(Nothing);
}


Maybe_Nothing handle_insert_query(State *state, ProtocolQueryResult **result, ProtocolInsertQuery *query)
{
    printf("insert query 1 %s\n", query->table_name->name);
    Table *table = HANDLE(Nothing, TablePtr, table_open(state, query->table_name->name));
    printf("table pointer %lu\n", (long unsigned int) table);
    if (table == 0)
        EXCEPTION(Nothing, "table is not exists");
    Row_LinkedList rows = HANDLE(Nothing, Row_LinkedList, parse_rows(state, table, query->data));
    InsertQuery insert_query = insert_query_new(table, rows);
    uint32_t count = HANDLE(Nothing, uint32_t, insert_query_execute(state, &insert_query));
    printf("inserted: %"PRIu32"\n", count);
    insert_query_free(&insert_query);
    printf("insert query 2\n");

    g_object_set
    (
        (*result)->insert,
        "count", count,
        NULL
    );

    printf("insert query 3\n");

    g_object_set
    (
        *result,
        "type", PROTOCOL_QUERY_TYPE_INSERT,
        "code", PROTOCOL_QUERY_RESULT_CODE_OK,
        NULL
    );

    OK(Nothing);
}


Maybe_Nothing handle_delete_query(State *state, ProtocolQueryResult **result, ProtocolDeleteQuery *query)
{
    (*result)->type = PROTOCOL_QUERY_TYPE_DELETE;

    VariableTableRelation_LinkedList rel = VariableTableRelation_linked_list_new();
    DataIterator data_it = HANDLE(Nothing, DataIterator, parse_data_iterator(state, query->data_source, &rel, 0));
    if (data_it.type != TABLE)
        EXCEPTION(Nothing, "not a table iterator");
    Table *table = HANDLE(Nothing, TablePtr, table_open(state, data_it.table.table_name));
    if (table == 0)
        EXCEPTION(Nothing, "table is not exists");

    Predicate *predicate = 0;
    if (query->__isset_filter)
        predicate = HANDLE(Nothing, PredicatePtr, parse_predicate(query->filter->expression, rel));

    DeleteQuery delete_query = delete_query_new(table, predicate);
    uint32_t count = HANDLE(Nothing, uint32_t, delete_query_execute(state, &delete_query));
    printf("deleted: %"PRIu32"\n", count);
    delete_query_free(&delete_query);

    g_object_set
    (
        (*result)->delete,
        "count", count,
        NULL
    );

    g_object_set
    (
        *result,
        "type", PROTOCOL_QUERY_TYPE_DELETE,
        "code", PROTOCOL_QUERY_RESULT_CODE_OK,
        NULL
    );

    OK(Nothing);
}


Maybe_Nothing handle_update_query(State *state, ProtocolQueryResult **result, ProtocolUpdateQuery *query)
{
    (*result)->type = PROTOCOL_QUERY_TYPE_UPDATE;

    VariableTableRelation_LinkedList rel = VariableTableRelation_linked_list_new();
    DataIterator data_it = HANDLE(Nothing, DataIterator, parse_data_iterator(state, query->data_source, &rel, 0));
    if (data_it.type != TABLE)
        EXCEPTION(Nothing, "not a table iterator");
    Table *table = HANDLE(Nothing, TablePtr, table_open(state, data_it.table.table_name));
    if (table == 0)
        EXCEPTION(Nothing, "table is not exists");

    Predicate *predicate = 0;
    if (query->__isset_filter)
        predicate = HANDLE(Nothing, PredicatePtr, parse_predicate(query->filter->expression, rel));

    Transformation_LinkedList transformations;
    if (query->__isset_assignments)
        transformations = HANDLE(Nothing, Transformation_LinkedList, parse_transformations(query->assignments, rel));
    else
        transformations = Transformation_linked_list_new();
    
    UpdateQuery update_query = update_query_new(table, predicate, transformations);
    uint32_t count = HANDLE(Nothing, uint32_t, update_query_execute(state, &update_query));
    update_query_free(&update_query);

    g_object_set
    (
        (*result)->update,
        "count", count,
        NULL
    );

    g_object_set
    (
        *result,
        "type", PROTOCOL_QUERY_TYPE_UPDATE,
        "code", PROTOCOL_QUERY_RESULT_CODE_OK,
        NULL
    );

    OK(Nothing);
}


Maybe_Nothing handle_create_table_query(State *state, ProtocolQueryResult **result, ProtocolCreateTableQuery *query)
{
    (*result)->type = PROTOCOL_QUERY_TYPE_CREATE_TABLE;

    ColumnSchema_LinkedList schema = parse_table_schema(query->defs);

    if (HANDLE(Nothing, TablePtr, table_open(state, query->table_name->name)) != NULL)
        EXCEPTION(Nothing, "table with provided name already exists");
    
    Table *table = HANDLE(Nothing, TablePtr, table_create(state, query->table_name->name, &schema));

    schema_to_defs(table->schema, (*result)->create_table->defs);

    g_object_set
    (
        *result,
        "type", PROTOCOL_QUERY_TYPE_CREATE_TABLE,
        "code", PROTOCOL_QUERY_RESULT_CODE_OK,
        NULL
    );

    table_close(state, table);

    OK(Nothing);
}


Maybe_Nothing handle_delete_table_query(State *state, ProtocolQueryResult **result, ProtocolDeleteTableQuery *query)
{
    (*result)->type = PROTOCOL_QUERY_TYPE_DELETE_TABLE;

    Table *table = HANDLE(Nothing, TablePtr, table_open(state, query->table_name->name));
    if (table == 0)
        EXCEPTION(Nothing, "table is not exists");
    HANDLE_NOTHING(Nothing, table_delete(state, table));

    g_object_set
    (
        *result,
        "type", PROTOCOL_QUERY_TYPE_DELETE_TABLE,
        "code", PROTOCOL_QUERY_RESULT_CODE_OK,
        NULL
    );

    OK(Nothing);
}
