#include "util.h"

void thrift_print_data_type(ProtocolDataType type);
void thrift_print_constant_type(ProtocolConstantType type);
void thrift_print_constant(ProtocolConstant *constant);
void thrift_print_variable(ProtocolVariable *variable);
void thrift_print_column_ptr(ProtocolColumnPtr *ptr);
void thrift_print_projection(ProtocolProjection *proj);
void thrift_print_expression(ProtocolExpression *expr);
void thrift_print_data_source(ProtocolDataSource *ds);
void thrift_print_assignment(ProtocolAssignment *assignment);
void thrift_print_filter(ProtocolFilter *filter);
void thrift_print_select_query(ProtocolSelectQuery *query);
void thrift_print_delete_query(ProtocolDeleteQuery *query);
void thrift_print_constants_list(GPtrArray *arr);
void thrift_print_constants_list_list(GPtrArray *arr);
void thrift_print_insert_query(ProtocolInsertQuery *query);
void thrift_print_assignments_list(GPtrArray *arr);
void thrift_print_update_query(ProtocolUpdateQuery *query);
void thrift_print_columns_def_list(GPtrArray *arr);
void thrift_print_create_table_query(ProtocolCreateTableQuery *query);
void thrift_print_delete_table_query(ProtocolDeleteTableQuery *query);

void thrift_print_data_type(ProtocolDataType type)
{
    switch (type)
    {
        case PROTOCOL_DATA_TYPE_BOOL:
            printf("bool");
            break;
        case PROTOCOL_DATA_TYPE_UINT_8:
            printf("uint_8");
            break;
        case PROTOCOL_DATA_TYPE_UINT_16:
            printf("uint_16");
            break;
        case PROTOCOL_DATA_TYPE_UINT_32:
            printf("uint_32");
            break;
        case PROTOCOL_DATA_TYPE_INT_32:
            printf("int_32");
            break;
        case PROTOCOL_DATA_TYPE_FLOAT:
            printf("float");
            break;
        case PROTOCOL_DATA_TYPE_STRING:
            printf("string");
            break;
        default:
            printf("unknown data type");
    }
}

void thrift_print_constant_type(ProtocolConstantType type)
{
    switch (type)
    {
        case PROTOCOL_CONSTANT_TYPE_BOOL:
            printf("bool const");
            break;
        case PROTOCOL_CONSTANT_TYPE_INTEGER:
            printf("integer const");
            break;
        case PROTOCOL_CONSTANT_TYPE_FLOAT:
            printf("float const");
            break;
        case PROTOCOL_CONSTANT_TYPE_STRING:
            printf("string const");
            break;
        default:
            printf("unknown const type\n");
    }
}

void thrift_print_constant(ProtocolConstant *constant)
{
    switch (constant->type)
    {
        case PROTOCOL_CONSTANT_TYPE_BOOL:
            printf("%d", constant->value->bool_constant);
            break;
        case PROTOCOL_CONSTANT_TYPE_INTEGER:
            printf("%d", constant->value->integer_constant);
            break;
        case PROTOCOL_CONSTANT_TYPE_FLOAT:
            printf("%f", constant->value->float_constant);
            break;
        case PROTOCOL_CONSTANT_TYPE_STRING:
            printf("%s", constant->value->string_constant);
            break;
    }
}

void thrift_print_variable(ProtocolVariable *variable)
{
    printf("%s", variable->name);
}

void thrift_print_column_ptr(ProtocolColumnPtr *ptr)
{
    printf("column ptr:\n");
    thrift_print_variable(ptr->table_name);
    thrift_print_variable(ptr->column_name);
}

void thrift_print_projection(ProtocolProjection *proj)
{
    printf("projection\n");
    if (proj == 0)
        return;
    
    uint32_t i = 0;
    while (i < proj->column_ptrs->len)
    {
        thrift_print_column_ptr(g_ptr_array_index(proj->column_ptrs, i));
        i++;
    }
}

void thrift_print_expression(ProtocolExpression *expr)
{
    printf("expression\n");
    if (expr == 0)
        return;
    
    uint32_t i = 0;
    while (i < expr->list->len)
    {
        ProtocolExpressionNode *node = g_ptr_array_index(expr->list, i);
        switch (node->type)
        {
            case PROTOCOL_EXPRESSION_NODE_TYPE_OPERATOR:
                switch (node->value->operator->type)
                {
                    case PROTOCOL_OPERATOR_TYPE_NOT:
                        printf("not\n");
                        break;
                    case PROTOCOL_OPERATOR_TYPE_AND:
                        printf("and\n");
                        break;
                    case PROTOCOL_OPERATOR_TYPE_OR:
                        printf("or\n");
                        break;
                    case PROTOCOL_OPERATOR_TYPE_EQUAL:
                        printf("equal\n");
                        break;
                    case PROTOCOL_OPERATOR_TYPE_NOT_EQUAL:
                        printf("not equal\n");
                        break;
                    case PROTOCOL_OPERATOR_TYPE_LESS:
                        printf("less\n");
                        break;
                    case PROTOCOL_OPERATOR_TYPE_NOT_LESS:
                        printf("not less\n");
                        break;
                    case PROTOCOL_OPERATOR_TYPE_GREATER:
                        printf("greater\n");
                        break;
                    case PROTOCOL_OPERATOR_TYPE_NOT_GREATER:
                        printf("not greater\n");
                        break;
                    case PROTOCOL_OPERATOR_TYPE_SUBSTR:
                        printf("substr\n");
                        break;
                    default:
                        break;
                }
                break;
            case PROTOCOL_EXPRESSION_NODE_TYPE_OPERAND:
                switch (node->value->operand->type)
                {
                    case PROTOCOL_EXPRESSION_OPERAND_TYPE_CONSTANT:
                        thrift_print_constant(node->value->operand->operand->constant_operand);
                        break;
                    case PROTOCOL_EXPRESSION_OPERAND_TYPE_COLUMN_PTR:
                        thrift_print_column_ptr(node->value->operand->operand->column_ptr_operand);
                        break;
                }
        }
        i++;
    }
}

void thrift_print_data_source(ProtocolDataSource *ds)
{
    printf("data source\n");
    if (ds == 0)
        return;
    
    uint32_t i = 0;
    while (i < ds->list->len)
    {
        ProtocolDataSourceNode *node = g_ptr_array_index(ds->list, i);
        switch (node->type)
        {
            case PROTOCOL_DATA_SOURCE_NODE_TYPE_TABLE:
                thrift_print_variable(node->value->table->table_name);
                thrift_print_variable(node->value->table->variable_name);
                break;
            case PROTOCOL_DATA_SOURCE_NODE_TYPE_EXPRESSION:
                thrift_print_expression(node->value->expression);
                break;
        }
        i++;
    }
}

void thrift_print_assignment(ProtocolAssignment *assignment)
{
    printf("assignment\n");
    thrift_print_column_ptr(assignment->to);
    switch (assignment->what->type)
    {
        case PROTOCOL_EXPRESSION_OPERAND_TYPE_CONSTANT:
            thrift_print_constant(assignment->what->operand->constant_operand);
            break;
        case PROTOCOL_EXPRESSION_OPERAND_TYPE_COLUMN_PTR:
            thrift_print_column_ptr(assignment->what->operand->column_ptr_operand);
            break;
    }
}

void thrift_print_filter(ProtocolFilter *filter)
{
    printf("filter\n");
    thrift_print_expression(filter->expression);
}

void thrift_print_select_query(ProtocolSelectQuery *query)
{
    printf("select query\n");
    thrift_print_data_source(query->data_source);
    if (query->__isset_filter)
        thrift_print_filter(query->filter);
    if (query->__isset_projection)
        thrift_print_projection(query->projection);
}

void thrift_print_delete_query(ProtocolDeleteQuery *query)
{
    printf("delete query\n");
    thrift_print_data_source(query->data_source);
    if (query->__isset_filter)
        thrift_print_filter(query->filter);
}

void thrift_print_constants_list(GPtrArray *arr)
{
    uint32_t i = 0;
    while (i < arr->len)
    {
        thrift_print_constant(g_ptr_array_index(arr, i));
        printf(" | ");
        i++;
    }
}

void thrift_print_constants_list_list(GPtrArray *arr)
{
    uint32_t i = 0;
    while (i < arr->len)
    {
        thrift_print_constants_list(g_ptr_array_index(arr, i));
        printf("\n");
        i++;
    }
}

void thrift_print_insert_query(ProtocolInsertQuery *query)
{
    printf("insert query\n");
    thrift_print_variable(query->table_name);
    thrift_print_constants_list_list(query->data);
}

void thrift_print_assignments_list(GPtrArray *arr)
{
    printf("assignments list\n");
    uint32_t i = 0;
    while (i < arr->len)
    {
        thrift_print_assignment(g_ptr_array_index(arr, i));
        i++;
    }
}

void thrift_print_update_query(ProtocolUpdateQuery *query)
{
    printf("update query\n");
    thrift_print_data_source(query->data_source);
    if (query->__isset_filter)
        thrift_print_filter(query->filter);
    thrift_print_assignments_list(query->assignments);
}

void thrift_print_columns_def_list(GPtrArray *arr)
{
    uint32_t i = 0;
    while (i < arr->len)
    {
        ProtocolColumnDef *def = g_ptr_array_index(arr, i);
        thrift_print_variable(def->name);
        printf(": ");
        thrift_print_data_type(def->type);
        printf(" | ");
        i++;
    }
    printf("\n");
}

void thrift_print_create_table_query(ProtocolCreateTableQuery *query)
{
    printf("create table query\n");
    thrift_print_variable(query->table_name);
    thrift_print_columns_def_list(query->defs);
}

void thrift_print_delete_table_query(ProtocolDeleteTableQuery *query)
{
    printf("delete table query\n");
    thrift_print_variable(query->table_name);
}

void thrift_print_query(ProtocolQuery *query)
{
    printf("query\n");
    switch (query->type)
    {
        case PROTOCOL_QUERY_TYPE_SELECT:
            thrift_print_select_query(query->query->select);
            break;
        case PROTOCOL_QUERY_TYPE_DELETE:
            thrift_print_delete_query(query->query->delete);
            break;
        case PROTOCOL_QUERY_TYPE_INSERT:
            thrift_print_insert_query(query->query->insert);
            break;
        case PROTOCOL_QUERY_TYPE_UPDATE:
            thrift_print_update_query(query->query->update);
            break;
        case PROTOCOL_QUERY_TYPE_CREATE_TABLE:
            thrift_print_create_table_query(query->query->create_table);
            break;
        case PROTOCOL_QUERY_TYPE_DELETE_TABLE:
            thrift_print_delete_table_query(query->query->delete_table);
            break;
    }
}



// query printing
void thrift_print_select_query_result(SelectQueryResult *result);
void thrift_print_delete_query_result(DeleteQueryResult *result);
void thrift_print_insert_query_result(InsertQueryResult *result);
void thrift_print_update_query_result(UpdateQueryResult *result);
void thrift_print_create_table_query_result(CreateTableQueryResult *result);
void thrift_print_delete_table_query_result(DeleteTableQueryResult *result);

void thrift_print_select_query_result(SelectQueryResult *result)
{
    printf("succesfully selected %"PRIu32" rows\n", result->count);
    printf("\nSCHEMA\n");
    thrift_print_columns_def_list(result->defs);
    printf("\nDATA\n");
    thrift_print_constants_list_list(result->data);
}

void thrift_print_delete_query_result(DeleteQueryResult *result)
{
    printf("succesfully deleted %"PRIu32" rows\n", result->count);
}

void thrift_print_insert_query_result(InsertQueryResult *result)
{
    printf("succesfully inserted %"PRIu32" rows\n", result->count);
}

void thrift_print_update_query_result(UpdateQueryResult *result)
{
    printf("succesfully deleted %"PRIu32" rows\n", result->count);
}

void thrift_print_create_table_query_result(CreateTableQueryResult *result)
{
    printf("table created\n");
    printf("\nSCHEMA:\n");
    thrift_print_columns_def_list(result->defs);
}

void thrift_print_delete_table_query_result(DeleteTableQueryResult *result)
{
    printf("table deleted\n");
}

void thrift_print_protocol_query_result(ProtocolQueryResult *result)
{
    printf("\n==========\n");
    if (result == 0)
    {
        printf("result is null");
        printf("==========\n\n");
        return;
    }
    if (!result->__isset_code)
    {
        printf("code is not set\n");
        printf("==========\n\n");
        return;
    }

    if (result->code == PROTOCOL_QUERY_RESULT_CODE_ERROR)
    {
        printf("error occured\n");
        if (result->__isset_error_message)
            printf("error message: %s\n", result->error_message);
        printf("==========\n\n");
        return;
    }
    switch (result->type)
    {
        case PROTOCOL_QUERY_TYPE_SELECT:
            printf("select query successfully executed\n");
            thrift_print_select_query_result(result->select);
            break;
        case PROTOCOL_QUERY_TYPE_INSERT:
            printf("insert query successfully executed\n");
            thrift_print_insert_query_result(result->insert);
            break;
        case PROTOCOL_QUERY_TYPE_DELETE:
            printf("delete query successfully executed\n");
            thrift_print_delete_query_result(result->delete);
            break;
        case PROTOCOL_QUERY_TYPE_UPDATE:
            printf("update query successfully executed\n");
            thrift_print_update_query_result(result->update);
            break;
        case PROTOCOL_QUERY_TYPE_CREATE_TABLE:
            printf("create table query successfully executed\n");
            thrift_print_create_table_query_result(result->create_table);
            break;
        case PROTOCOL_QUERY_TYPE_DELETE_TABLE:
            printf("delete table query successfully executed\n");
            thrift_print_delete_table_query_result(result->delete_table);
            break;
    }
    printf("==========\n\n");
}