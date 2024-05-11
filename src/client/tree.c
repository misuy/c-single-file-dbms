#include "tree.h"

#define SPACES_PER_LEVEL 3

void print_spaces(FILE *file, uint32_t n);
char * unary_operator_get_name(UnaryOperator operator);
char * binary_operator_get_name(BinaryOperator operator);
char * type_get_name(DataType type);
char * node_get_type_name(Node *node);



void print_spaces(FILE *file, uint32_t n)
{
    for (uint32_t i=0; i<n; i++)
    {
        fprintf(file, " ");
    }
}

char * unary_operator_get_name(UnaryOperator operator)
{
    switch (operator)
    {
        case OP_NOT:
            return "!(not)";
        default:
            return "unknown";
    }
}

char * binary_operator_get_name(BinaryOperator operator)
{
    switch (operator)
    {
        case OP_AND:
            return "&(and)";
        case OP_OR:
            return "|(or)";
        case OP_EQUAL:
            return "==(equal)";
        case OP_NOT_EQUAL:
            return "!=(not equal)";
        case OP_LESS:
            return "<(less)";
        case OP_NOT_GREATER:
            return "<=(not greater)";
        case OP_GREATER:
            return ">(greater)";
        case OP_NOT_LESS:
            return ">=(not less)";
        case OP_SUBSTR:
            return "substr(substring)";
        default:
            return "unknown";
    }
}

char * type_get_name(DataType type)
{
    switch (type)
    {
        case TYPE_BOOL:
            return "bool";
        case TYPE_UINT_8:
            return "uint_8";
        case TYPE_UINT_16:
            return "uint_16";
        case TYPE_UINT_32:
            return "uint_32";
        case TYPE_INT_32:
            return "int_32";
        case TYPE_FLOAT:
            return "float";
        case TYPE_STRING:
            return "string";
        default:
            return "unknown";
    }
}

char * node_get_type_name(Node *node)
{
    if (node == 0)
        return "null";
    
    switch (node->type)
    {
        case NODE_ERROR:
            return "error";
        case NODE_BOOL_CONSTANT:
            return "bool_constant";
        case NODE_INTEGER_CONSTANT:
            return "integer_constant";
        case NODE_FLOAT_CONSTANT:
            return "float_constant";
        case NODE_STRING_CONSTANT:
            return "string_constant";
        case NODE_CONSTANTS_LIST:
            return "constants_list";
        case NODE_TYPE:
            return "type";
        case NODE_COLUMN_DEF:
            return "column_def";
        case NODE_COLUMNS_DEF_LIST:
            return "columns_def_list";
        case NODE_VARIABLE:
            return "variable";
        case NODE_COLUMN_PTR:
            return "column_ptr";
        case NODE_PROJECTION:
            return "projection";
        case NODE_UNARY_EXPRESSION:
            return "unary_expression";
        case NODE_BINARY_EXPRESSION:
            return "binary_expression";
        case NODE_TABLE_DATA_SOURCE:
            return "table_data_source";
        case NODE_JOIN_DATA_SOURCE:
            return "join_data_source";
        case NODE_FILTER:
            return "filter";
        case NODE_ASSIGNMENT:
            return "assignment";
        case NODE_ASSIGNMENTS_LIST:
            return "assignments_list";
        case NODE_DATA_TUPLES_LIST:
            return "data_tuples_list";
        case NODE_SELECT_QUERY:
            return "select_query";
        case NODE_DELETE_QUERY:
            return "delete_query";
        case NODE_INSERT_QUERY:
            return "insert_query";
        case NODE_UPDATE_QUERY:
            return "update_query";
        case NODE_CREATE_TABLE_QUERY:
            return "create_table_query";
        case NODE_DELETE_TABLE_QUERY:
            return "delete_table_query";
        default:
            return "unknown";
    }
}

void node_print(FILE *file, Node *node, char *name, uint32_t level)
{
    if (node == 0)
    {
        print_spaces(file, (level - 1) * SPACES_PER_LEVEL);
        fprintf(file, "%s: null,\n", name);
        return;
    }
    
    print_spaces(file, (level - 1) * SPACES_PER_LEVEL);
    fprintf(file, "%s: {\n", name);
    print_spaces(file, level * SPACES_PER_LEVEL);
    //printf("%d\n", node->type);
    fprintf(file, "node_type: %s,\n", node_get_type_name(node));

    switch (node->type)
    {
        case NODE_ERROR:
            print_spaces(file, level * SPACES_PER_LEVEL);
            fprintf(file, "error_offset: %"PRIu32"\n", node->error.error_offset);
            print_spaces(file, level * SPACES_PER_LEVEL);
            fprintf(file, "error_message:\n%s,\n", node->error.error_message);
            node_print(file, node->error.tree, "tree", level + 1);
            break;

        case NODE_BOOL_CONSTANT:
            print_spaces(file, level * SPACES_PER_LEVEL);
            fprintf(file, "value: \'%s\',\n", node->bool_constant.value ? "t" : "f");
            break;

        case NODE_INTEGER_CONSTANT:
            print_spaces(file, level * SPACES_PER_LEVEL);
            fprintf(file, "value: %"PRIu32",\n", node->integer_constant.value);
            break;

        case NODE_FLOAT_CONSTANT:
            print_spaces(file, level * SPACES_PER_LEVEL);
            fprintf(file, "value: %f,\n", node->float_constant.value);
            break;

        case NODE_STRING_CONSTANT:
            print_spaces(file, level * SPACES_PER_LEVEL);
            fprintf(file, "value: \'%s\',\n", node->string_constant.value);
            break;

        case NODE_CONSTANTS_LIST:
            node_print(file, node->constants_list.constant, "constant", level + 1);
            node_print(file, node->constants_list.next, "next", level + 1);
            break;
        
        case NODE_TYPE:
            print_spaces(file, level * SPACES_PER_LEVEL);
            fprintf(file, "type: %s,\n", type_get_name(node->typee.type));

        case NODE_COLUMN_DEF:
            node_print(file, node->column_def.type, "type", level + 1);
            node_print(file, node->column_def.name, "column_name", level + 1);
            break;
        
        case NODE_COLUMNS_DEF_LIST:
            node_print(file, node->columns_def_list.def, "def", level + 1);
            node_print(file, node->columns_def_list.next, "next", level + 1);
            break;

        case NODE_VARIABLE:
            print_spaces(file, level * SPACES_PER_LEVEL);
            fprintf(file, "name: \'%s\',\n", node->variable.name);
            break;

        case NODE_COLUMN_PTR:
            node_print(file, node->column_ptr.table_name, "table_name", level + 1);
            node_print(file, node->column_ptr.column_name, "column_name", level + 1);
            break;

        case NODE_PROJECTION:
            node_print(file, node->projection.column_ptr, "column_ptr", level + 1);
            node_print(file, node->projection.next, "next", level + 1);
            break;
        
        case NODE_UNARY_EXPRESSION:
            print_spaces(file, level * SPACES_PER_LEVEL);
            fprintf(file, "operator: %s,\n", unary_operator_get_name(node->unary_expression.op));
            node_print(file, node->unary_expression.operand, "operand", level + 1);
            break;
        
        case NODE_BINARY_EXPRESSION:
            print_spaces(file, level * SPACES_PER_LEVEL);
            fprintf(file, "operator: %s,\n", binary_operator_get_name(node->binary_expression.op));
            node_print(file, node->binary_expression.left_operand, "left_operand", level + 1);
            node_print(file, node->binary_expression.right_operand, "right_operand", level + 1);
            break;
        
        case NODE_TABLE_DATA_SOURCE:
            node_print(file, node->table_data_source.table, "table", level + 1);
            node_print(file, node->table_data_source.variable, "variable", level + 1);
            break;
        
        case NODE_JOIN_DATA_SOURCE:
            node_print(file, node->join_data_source.expression, "expression", level + 1);
            node_print(file, node->join_data_source.left_data_source, "left_data_source", level + 1);
            node_print(file, node->join_data_source.right_data_source, "right_data_source", level + 1);
            break;

        case NODE_FILTER:
            node_print(file, node->filter.expression, "expression", level + 1);
            break;
        
        case NODE_ASSIGNMENT:
            node_print(file, node->assignment.to, "to", level + 1);
            node_print(file, node->assignment.what, "what", level + 1);
            break;
        
        case NODE_ASSIGNMENTS_LIST:
            node_print(file, node->assignments_list.assignment, "assignment", level + 1);
            node_print(file, node->assignments_list.next, "next", level + 1);
            break;

        case NODE_DATA_TUPLES_LIST:
            node_print(file, node->data_tuples_list.data_tuple, "data_tuple", level + 1);
            node_print(file, node->data_tuples_list.next, "next", level + 1);
            break;
        
        case NODE_SELECT_QUERY:
            node_print(file, node->select_query.data_source, "data_source", level + 1);
            node_print(file, node->select_query.filter, "filter", level + 1);
            node_print(file, node->select_query.projection, "projection", level + 1);
            break;

        case NODE_DELETE_QUERY:
            node_print(file, node->delete_query.data_source, "data_source", level + 1);
            node_print(file, node->delete_query.filter, "filter", level + 1);
            break;

        case NODE_INSERT_QUERY:
            node_print(file, node->insert_query.table_name, "table_name", level + 1);
            node_print(file, node->insert_query.data_tuples, "data_tuples", level + 1);
            break;

        case NODE_UPDATE_QUERY:
            node_print(file, node->update_query.data_source, "data_source", level + 1);
            node_print(file, node->update_query.filter, "filter", level + 1);
            node_print(file, node->update_query.assignments, "assignments", level + 1);
            break;
        
        case NODE_CREATE_TABLE_QUERY:
            node_print(file, node->create_table_query.table_name, "table_name", level + 1);
            node_print(file, node->create_table_query.columns_def, "columns_def", level + 1);
            break;
        
        case NODE_DELETE_TABLE_QUERY:
            node_print(file, node->delete_table_query.table_name, "table_name", level + 1);
            break;

        default:
            break;
    }

    print_spaces(file, (level - 1) * 3);
    fprintf(file, "},\n");
}

void node_free(Node *node)
{
    if (node == 0)
        return;
        
    switch (node->type)
    {
        case NODE_ERROR:
            free(node->error.error_message);
            node_free(node->error.tree);
            break;

        case NODE_BOOL_CONSTANT:
        case NODE_INTEGER_CONSTANT:
        case NODE_FLOAT_CONSTANT:
            break;
        case NODE_STRING_CONSTANT:
            free(node->string_constant.value);
            break;
        case NODE_CONSTANTS_LIST:
            node_free(node->constants_list.constant);
            node_free(node->constants_list.next);
            break;

        case NODE_TYPE:
            break;

        case NODE_COLUMN_DEF:
            node_free(node->column_def.name);
            break;
        case NODE_COLUMNS_DEF_LIST:
            node_free(node->columns_def_list.def);
            node_free(node->columns_def_list.next);
            break;

        case NODE_VARIABLE:
            free(node->variable.name);
            break;
        
        case NODE_COLUMN_PTR:
            node_free(node->column_ptr.table_name);
            node_free(node->column_ptr.column_name);
            break;
        
        case NODE_PROJECTION:
            node_free(node->projection.column_ptr);
            node_free(node->projection.next);
            break;

        case NODE_UNARY_EXPRESSION:
            node_free(node->unary_expression.operand);
            break;
        case NODE_BINARY_EXPRESSION:
            node_free(node->binary_expression.left_operand);
            node_free(node->binary_expression.right_operand);
            break;

        case NODE_TABLE_DATA_SOURCE:
            node_free(node->table_data_source.table);
            node_free(node->table_data_source.variable);
            break;
        case NODE_JOIN_DATA_SOURCE:
            node_free(node->join_data_source.left_data_source);
            node_free(node->join_data_source.right_data_source);
            node_free(node->join_data_source.expression);
            break;
        
        case NODE_FILTER:
            node_free(node->filter.expression);
            break;

        case NODE_ASSIGNMENT:
            node_free(node->assignment.to);
            node_free(node->assignment.what);
            break;
        case NODE_ASSIGNMENTS_LIST:
            node_free(node->assignments_list.assignment);
            node_free(node->assignments_list.next);
            break;
        
        case NODE_DATA_TUPLES_LIST:
            node_free(node->data_tuples_list.data_tuple);
            node_free(node->data_tuples_list.next);
            break;

        case NODE_SELECT_QUERY:
            node_free(node->select_query.data_source);
            node_free(node->select_query.filter);
            node_free(node->select_query.projection);
            break;

        case NODE_DELETE_QUERY:
            node_free(node->delete_query.data_source);
            node_free(node->delete_query.filter);
            break;

        case NODE_INSERT_QUERY:
            node_free(node->insert_query.data_tuples);
            node_free(node->insert_query.table_name);
            break;
        
        case NODE_UPDATE_QUERY:
            node_free(node->update_query.assignments);
            node_free(node->update_query.data_source);
            node_free(node->update_query.filter);
            break;
        
        case NODE_CREATE_TABLE_QUERY:
            node_free(node->create_table_query.table_name);
            node_free(node->create_table_query.columns_def);
            break;

        case NODE_DELETE_TABLE_QUERY:
            node_free(node->delete_table_query.table_name);
            break;

        default:
            break;
    }
    free(node);
}



// transformation to thrift format

ProtocolDataType thrift_parse_data_type(DataType type);
ProtocolDataType thrift_parse_data_type_node(Node *node);
ProtocolColumnDef * thrift_parse_column_def(Node *node);
void thrift_parse_columns_def_list_impl(Node *node, GPtrArray *arr);
GPtrArray * thrift_parse_columns_def_list(Node *node);
ProtocolConstantType thrift_parse_constant_type(Node *node);
ProtocolConstantUnion * thrift_parse_constant_union(Node *node);
ProtocolConstant * thrift_parse_constant(Node *node);
void thrift_parse_constants_list_impl(Node *node, GPtrArray *arr);
GPtrArray * thrift_parse_constants_list(Node *node);
void thrift_parse_constants_list_list_impl(Node *node, GPtrArray *arr);
GPtrArray * thrift_parse_constants_list_list(Node *node);
ProtocolVariable * thrift_parse_variable(Node *node);
ProtocolColumnPtr * thrift_parse_column_ptr(Node *node);
void thrift_parse_projection_impl(Node *node, GPtrArray *arr);
ProtocolProjection * thrift_parse_projection(Node *node);
void thrift_parse_expression_impl(Node *node, GPtrArray *arr);
ProtocolExpression * thrift_parse_expression(Node *node);
void thrift_parse_data_source_impl(Node *node, GPtrArray *arr);
ProtocolDataSource * thrift_parse_data_source(Node *node);
ProtocolAssignment * thrift_parse_assignment(Node *node);
void thrift_parse_assignments_list_impl(Node *node, GPtrArray *arr);
GPtrArray * thrift_parse_assignments_list(Node *node);
ProtocolFilter * thrift_parse_filter(Node *node);
ProtocolSelectQuery * thrift_parse_select_query(Node *node);
ProtocolDeleteQuery * thrift_parse_delete_query(Node *node);
ProtocolInsertQuery * thrift_parse_insert_query(Node *node);
ProtocolUpdateQuery * thrift_parse_update_query(Node *node);
ProtocolCreateTableQuery * thrift_parse_create_table_query(Node *node);
ProtocolDeleteTableQuery * thrift_parse_delete_table_query(Node *node);
ProtocolQueryType thrift_parse_query_type(Node *node);
ProtocolQueryUnion * thrift_parse_query_union(Node *node);
ProtocolQuery * thrift_parse_query(Node *node);

ProtocolDataType thrift_parse_data_type(DataType type)
{
    switch (type)
    {
        case TYPE_BOOL:
            return PROTOCOL_DATA_TYPE_BOOL;
        case TYPE_UINT_8:
            return PROTOCOL_DATA_TYPE_UINT_8;
        case TYPE_UINT_16:
            return PROTOCOL_DATA_TYPE_UINT_16;
        case TYPE_UINT_32:
            return PROTOCOL_DATA_TYPE_UINT_32;
        case TYPE_INT_32:
            return PROTOCOL_DATA_TYPE_INT_32;
        case TYPE_FLOAT:
            return PROTOCOL_DATA_TYPE_FLOAT;
        case TYPE_STRING:
            return PROTOCOL_DATA_TYPE_STRING;
        default:
            return 0;
    }
}

ProtocolDataType thrift_parse_data_type_node(Node *node)
{
    if (node == 0)
        return 0;
    if (node->type != NODE_TYPE)
        return 0;
    return thrift_parse_data_type(node->typee.type);
}

ProtocolColumnDef * thrift_parse_column_def(Node *node)
{
    if (node == 0)
        return 0;
    if (node->type != NODE_COLUMN_DEF)
        return 0;
    
    ProtocolVariable *name = thrift_parse_variable(node->column_def.name);
    if (!name)
        return 0;
    ProtocolDataType type = thrift_parse_data_type_node(node->column_def.type);
    return g_object_new
    (
        TYPE_PROTOCOL_COLUMN_DEF,
        "type", type,
        "name", name,
        NULL
    );
}

void thrift_parse_columns_def_list_impl(Node *node, GPtrArray *arr)
{
    if (node == 0)
        return;
    if (node->type != NODE_COLUMNS_DEF_LIST)
        return;

    thrift_parse_columns_def_list_impl(node->columns_def_list.next, arr);
    
    ProtocolColumnDef *def = thrift_parse_column_def(node->columns_def_list.def);
    if (!def)
        return;
    
    g_ptr_array_add(arr, def);
}

GPtrArray * thrift_parse_columns_def_list(Node *node)
{
    if (node == 0)
        return 0;
    if (node->type != NODE_COLUMNS_DEF_LIST)
        return 0;

    GPtrArray *arr = g_ptr_array_new();
    thrift_parse_columns_def_list_impl(node, arr);

    return arr;
}

ProtocolConstantType thrift_parse_constant_type(Node *node)
{
    if (node == 0)
        return 0;
    
    switch (node->type)
    {
        case NODE_BOOL_CONSTANT:
            return PROTOCOL_CONSTANT_TYPE_BOOL;
        case NODE_INTEGER_CONSTANT:
            return PROTOCOL_CONSTANT_TYPE_INTEGER;
        case NODE_FLOAT_CONSTANT:
            return PROTOCOL_CONSTANT_TYPE_FLOAT;
        case NODE_STRING_CONSTANT:
            return PROTOCOL_CONSTANT_TYPE_STRING;
        
        default:
            return 0;
    }
}

ProtocolConstantUnion * thrift_parse_constant_union(Node *node)
{
    if (node == 0)
        return 0;

    ProtocolConstantUnion *ret = g_object_new(TYPE_PROTOCOL_CONSTANT_UNION, 0);
    
    switch (node->type)
    {
        case NODE_BOOL_CONSTANT:
            g_object_set
            (
                ret,
                "bool_constant", node->bool_constant.value,
                NULL
            );
            break;
        
        case NODE_INTEGER_CONSTANT:
            g_object_set
            (
                ret,
                "integer_constant", node->integer_constant.value,
                NULL
            );
            break;

        case NODE_FLOAT_CONSTANT:
            g_object_set
            (
                ret,
                "float_constant", node->float_constant.value,
                NULL
            );
            break;
        
        case NODE_STRING_CONSTANT:
            g_object_set
            (
                ret,
                "string_constant", g_strdup(node->string_constant.value),
                NULL
            );
            break;
        
        default:
            g_object_unref(ret);
            ret = 0;
    }

    return ret;
}

ProtocolConstant * thrift_parse_constant(Node *node)
{
    ProtocolConstantType type = thrift_parse_constant_type(node);
    if (!type)
        return 0;
    
    ProtocolConstantUnion *value = thrift_parse_constant_union(node);
    if (!value)
        return 0;
    
    ProtocolConstant *ret = g_object_new(TYPE_PROTOCOL_CONSTANT, 0);
    g_object_set
    (
        ret,
        "type", type,
        "value", value,
        NULL
    );
    return ret;
}

void thrift_parse_constants_list_impl(Node *node, GPtrArray *arr)
{
    if (node == 0)
        return;
    if (node->type != NODE_CONSTANTS_LIST)
        return;

    thrift_parse_constants_list_impl(node->constants_list.next, arr);

    ProtocolConstant *constant = thrift_parse_constant(node->constants_list.constant);
    if (!constant)
        return;
    
    g_ptr_array_add(arr, constant);
}

GPtrArray * thrift_parse_constants_list(Node *node)
{
    if (node == 0)
        return 0;
    if (node->type != NODE_CONSTANTS_LIST)
        return 0;
    
    GPtrArray *arr = g_ptr_array_new();
    thrift_parse_constants_list_impl(node, arr);

    return arr;
}

void thrift_parse_constants_list_list_impl(Node *node, GPtrArray *arr)
{
    if (node == 0)
        return;
    if (node->type != NODE_DATA_TUPLES_LIST)
        return;
    
    thrift_parse_constants_list_list_impl(node->data_tuples_list.next, arr);

    GPtrArray *constants_list = thrift_parse_constants_list(node->data_tuples_list.data_tuple);
    if (!constants_list)
        return;
    
    g_ptr_array_add(arr, constants_list);
}

GPtrArray * thrift_parse_constants_list_list(Node *node)
{
    if (node == 0)
        return 0;
    if (node->type != NODE_DATA_TUPLES_LIST)
        return 0;

    GPtrArray *arr = g_ptr_array_new();
    thrift_parse_constants_list_list_impl(node, arr);
    
    return arr;
}

ProtocolVariable * thrift_parse_variable(Node *node)
{
    if (node == 0)
        return 0;
    if (node->type != NODE_VARIABLE)
        return 0;

    ProtocolVariable *ret = g_object_new(TYPE_PROTOCOL_VARIABLE, 0);
    g_object_set
    (
        ret,
        "name", g_strdup(node->variable.name),
        NULL
    );
    return ret;
}

ProtocolColumnPtr * thrift_parse_column_ptr(Node *node)
{
    if (node == 0)
        return 0;
    if (node->type != NODE_COLUMN_PTR)
        return 0;
    ProtocolVariable *table_name = thrift_parse_variable(node->column_ptr.table_name);
    if (!table_name)
        return 0;
    ProtocolVariable *column_name = thrift_parse_variable(node->column_ptr.column_name);
    if (!column_name)
        return 0;
    
    ProtocolColumnPtr *ret = g_object_new(TYPE_PROTOCOL_COLUMN_PTR, 0);
    g_object_set
    (
        ret,
        "table_name", table_name,
        "column_name", column_name,
        NULL
    );
    return ret;
}

void thrift_parse_projection_impl(Node *node, GPtrArray *arr)
{
    if (node == 0)
        return;
    if (node->type != NODE_PROJECTION)
        return;

    thrift_parse_projection_impl(node->projection.next, arr);

    ProtocolColumnPtr *column_ptr = thrift_parse_column_ptr(node->projection.column_ptr);
    g_ptr_array_add(arr, column_ptr);
}

ProtocolProjection * thrift_parse_projection(Node *node)
{
    GPtrArray *arr = g_ptr_array_new();
    ProtocolProjection *ret = g_object_new(TYPE_PROTOCOL_PROJECTION, 0);
    if (node != 0)
    {
        if (node->type != NODE_PROJECTION)
            return 0;

        thrift_parse_projection_impl(node, arr);
    }
    g_object_set
    (
        ret,
        "column_ptrs", arr,
        NULL
    );
    return ret;
}



void thrift_parse_expression_impl(Node *node, GPtrArray *arr)
{
    if (node == 0)
        return;
    
    ProtocolExpressionNode *add = 0;
    ProtocolConstant *constant = 0;
    ProtocolExpressionOperandUnion *operand_union = 0;
    ProtocolExpressionOperand *operand = 0;
    ProtocolExpressionNodeUnion *node_union = 0;
    ProtocolColumnPtr *column_ptr = 0;
    ProtocolOperatorType op_type = 0;
    ProtocolExpressionOperator *operator = 0;

    switch (node->type)
    {
        case NODE_BOOL_CONSTANT:
        case NODE_INTEGER_CONSTANT:
        case NODE_FLOAT_CONSTANT:
        case NODE_STRING_CONSTANT:
            constant = thrift_parse_constant(node);
            if (!constant)
                return;
            operand_union = g_object_new
            (
                TYPE_PROTOCOL_EXPRESSION_OPERAND_UNION,
                "constant_operand", constant,
                NULL
            );
            operand = g_object_new
            (
                TYPE_PROTOCOL_EXPRESSION_OPERAND,
                "type", PROTOCOL_EXPRESSION_OPERAND_TYPE_CONSTANT,
                "operand", operand_union,
                NULL
            );
            node_union = g_object_new
            (
                TYPE_PROTOCOL_EXPRESSION_NODE_UNION,
                "operand", operand,
                NULL
            );
            add = g_object_new
            (
                TYPE_PROTOCOL_EXPRESSION_NODE,
                "type", PROTOCOL_EXPRESSION_NODE_TYPE_OPERAND,
                "value", node_union,
                NULL
            );
            g_ptr_array_add(arr, add);
            return;
        
        case NODE_COLUMN_PTR:
            column_ptr = thrift_parse_column_ptr(node);
            if (!column_ptr)
                return;
            operand_union = g_object_new
            (
                TYPE_PROTOCOL_EXPRESSION_OPERAND_UNION,
                "column_ptr_operand", column_ptr,
                NULL
            );
            operand = g_object_new
            (
                TYPE_PROTOCOL_EXPRESSION_OPERAND,
                "type", PROTOCOL_EXPRESSION_OPERAND_TYPE_COLUMN_PTR,
                "operand", operand_union,
                NULL
            );
            node_union = g_object_new
            (
                TYPE_PROTOCOL_EXPRESSION_NODE_UNION,
                "operand", operand,
                NULL
            );
            add = g_object_new
            (
                TYPE_PROTOCOL_EXPRESSION_NODE,
                "type", PROTOCOL_EXPRESSION_NODE_TYPE_OPERAND,
                "value", node_union,
                NULL
            );
            g_ptr_array_add(arr, add);
            return;

        case NODE_UNARY_EXPRESSION:
            thrift_parse_expression_impl(node->unary_expression.operand, arr);
            op_type = 0;
            switch (node->unary_expression.op)
            {
                case OP_NOT:
                    op_type = PROTOCOL_OPERATOR_TYPE_NOT;
                    break;
                default:
                    break;
            }
            operator = g_object_new
            (
                TYPE_PROTOCOL_EXPRESSION_OPERATOR,
                "type", op_type,
                NULL
            );
            node_union = g_object_new
            (
                TYPE_PROTOCOL_EXPRESSION_NODE_UNION,
                "operator", operator,
                NULL
            );
            add = g_object_new
            (
                TYPE_PROTOCOL_EXPRESSION_NODE,
                "type", PROTOCOL_EXPRESSION_NODE_TYPE_OPERATOR,
                "value", node_union,
                NULL
            );
            g_ptr_array_add(arr, add);
            return;
        
        case NODE_BINARY_EXPRESSION:
            thrift_parse_expression_impl(node->binary_expression.left_operand, arr);
            thrift_parse_expression_impl(node->binary_expression.right_operand, arr);
            op_type = 0;
            switch (node->binary_expression.op)
            {
                case OP_AND:
                    op_type = PROTOCOL_OPERATOR_TYPE_AND;
                    break;
                case OP_OR:
                    op_type = PROTOCOL_OPERATOR_TYPE_OR;
                    break;
                case OP_EQUAL:
                    op_type = PROTOCOL_OPERATOR_TYPE_EQUAL;
                    break;
                case OP_NOT_EQUAL:
                    op_type = PROTOCOL_OPERATOR_TYPE_NOT_EQUAL;
                case OP_LESS:
                    op_type = PROTOCOL_OPERATOR_TYPE_LESS;
                    break;
                case OP_NOT_LESS:
                    op_type = PROTOCOL_OPERATOR_TYPE_NOT_LESS;
                    break;
                case OP_GREATER:
                    op_type = PROTOCOL_OPERATOR_TYPE_GREATER;
                    break;
                case OP_NOT_GREATER:
                    op_type = PROTOCOL_OPERATOR_TYPE_NOT_GREATER;
                    break;
                case OP_SUBSTR:
                    op_type = PROTOCOL_OPERATOR_TYPE_SUBSTR;
                    break;
                default:
                    break;
            }
            operator = g_object_new
            (
                TYPE_PROTOCOL_EXPRESSION_OPERATOR,
                "type", op_type,
                NULL
            );
            node_union = g_object_new
            (
                TYPE_PROTOCOL_EXPRESSION_NODE_UNION,
                "operator", operator,
                NULL
            );
            add = g_object_new
            (
                TYPE_PROTOCOL_EXPRESSION_NODE,
                "type", PROTOCOL_EXPRESSION_NODE_TYPE_OPERATOR,
                "value", node_union,
                NULL
            );
            g_ptr_array_add(arr, add);
            return;
        
        default:
            return;
    }
}

ProtocolExpression * thrift_parse_expression(Node *node)
{
    if (node == 0)
        return 0;
    
    GPtrArray *arr = g_ptr_array_new();
    thrift_parse_expression_impl(node, arr);

    return g_object_new
    (
        TYPE_PROTOCOL_EXPRESSION,
        "list", arr,
        NULL
    );
}

void thrift_parse_data_source_impl(Node *node, GPtrArray *arr)
{
    if (node == 0)
        return;
    
    ProtocolDataSourceNode *add = 0;
    ProtocolDataSourceUnion *value = 0;

    switch (node->type)
    {
        case NODE_TABLE_DATA_SOURCE:
            ProtocolVariable *table_name = thrift_parse_variable(node->table_data_source.table);
            ProtocolVariable *variable_name = thrift_parse_variable(node->table_data_source.variable);
            ProtocolDataSourceTable *table = g_object_new
            (
                TYPE_PROTOCOL_DATA_SOURCE_TABLE,
                "table_name", table_name,
                "variable_name", variable_name,
                NULL
            );
            value = g_object_new
            (
                TYPE_PROTOCOL_DATA_SOURCE_UNION,
                "table", table,
                NULL
            );
            add = g_object_new
            (
                TYPE_PROTOCOL_DATA_SOURCE_NODE,
                "type", PROTOCOL_DATA_SOURCE_NODE_TYPE_TABLE,
                "value", value,
                NULL
            );
            g_ptr_array_add(arr, add);
            return;
        
        case NODE_JOIN_DATA_SOURCE:
            thrift_parse_data_source_impl(node->join_data_source.left_data_source, arr);
            thrift_parse_data_source_impl(node->join_data_source.right_data_source, arr);
            ProtocolExpression *expression = thrift_parse_expression(node->join_data_source.expression);
            value = g_object_new
            (
                TYPE_PROTOCOL_DATA_SOURCE_UNION,
                "expression", expression,
                NULL
            );
            add = g_object_new
            (
                TYPE_PROTOCOL_DATA_SOURCE_NODE,
                "type", PROTOCOL_DATA_SOURCE_NODE_TYPE_EXPRESSION,
                "value", value,
                NULL
            );
            g_ptr_array_add(arr, add);
            return;
        
        default:
            return;
    }
}

ProtocolDataSource * thrift_parse_data_source(Node *node)
{
    if (node == 0)
        return 0;
    
    GPtrArray *arr = g_ptr_array_new();
    thrift_parse_data_source_impl(node, arr);

    return g_object_new
    (
        TYPE_PROTOCOL_DATA_SOURCE,
        "list", arr,
        NULL
    );
}


ProtocolAssignment * thrift_parse_assignment(Node *node)
{
    if (node == 0)
        return 0;
    if (node->type != NODE_ASSIGNMENT)
        return 0;

    ProtocolColumnPtr *to = thrift_parse_column_ptr(node->assignment.to);
    if (!to)
        return 0;
    
    ProtocolConstant *constant;
    ProtocolColumnPtr *column_ptr;
    ProtocolExpressionOperandUnion *operand_union;
    ProtocolExpressionOperand *operand;

    switch (node->assignment.what->type)
    {
        case NODE_BOOL_CONSTANT:
        case NODE_INTEGER_CONSTANT:
        case NODE_FLOAT_CONSTANT:
        case NODE_STRING_CONSTANT:
            constant = thrift_parse_constant(node->assignment.what);
            if (!constant)
                return 0;
            operand_union = g_object_new
            (
                TYPE_PROTOCOL_EXPRESSION_OPERAND_UNION,
                "constant_operand", constant,
                NULL
            );
            operand = g_object_new
            (
                TYPE_PROTOCOL_EXPRESSION_OPERAND,
                "type", PROTOCOL_EXPRESSION_OPERAND_TYPE_CONSTANT,
                "operand", operand_union,
                NULL
            );
            break;

        case NODE_COLUMN_PTR:
            column_ptr = thrift_parse_column_ptr(node->assignment.what);
            if (!column_ptr)
                return 0;
            operand_union = g_object_new
            (
                TYPE_PROTOCOL_EXPRESSION_OPERAND_UNION,
                "column_ptr_operand", column_ptr,
                NULL
            );
            operand = g_object_new
            (
                TYPE_PROTOCOL_EXPRESSION_OPERAND,
                "type", PROTOCOL_EXPRESSION_OPERAND_TYPE_COLUMN_PTR,
                "operand", operand_union,
                NULL
            );
            break;
        
        default:
            return 0;
    }
    
    ProtocolAssignment *ret = g_object_new(TYPE_PROTOCOL_ASSIGNMENT, 0);
    g_object_set
    (
        ret,
        "to", to,
        "what", operand,
        NULL
    );
    return ret;
}


void thrift_parse_assignments_list_impl(Node *node, GPtrArray *arr)
{
    if (node == 0)
        return;
    if (node->type != NODE_ASSIGNMENTS_LIST)
        return;
    
    thrift_parse_assignments_list_impl(node->assignments_list.next, arr);

    ProtocolAssignment *assignment = thrift_parse_assignment(node->assignments_list.assignment);
    if (!assignment)
        return;
    g_ptr_array_add(arr, assignment);
}

GPtrArray * thrift_parse_assignments_list(Node *node)
{
    if (node == 0)
        return 0;
    if (node->type != NODE_ASSIGNMENTS_LIST)
        return 0;

    GPtrArray *arr = g_ptr_array_new();
    thrift_parse_assignments_list_impl(node, arr);

    return arr;
}

ProtocolFilter * thrift_parse_filter(Node *node)
{
    if (node == 0)
        return 0;
    if (node->type != NODE_FILTER)
        return 0;
    
    ProtocolExpression *expression = thrift_parse_expression(node->filter.expression);
    if (!expression)
        return 0;

    ProtocolFilter *ret = g_object_new(TYPE_PROTOCOL_FILTER, 0);
    g_object_set
    (
        ret,
        "expression", expression,
        NULL
    );
    return ret;
}

ProtocolSelectQuery * thrift_parse_select_query(Node *node)
{
    if (node == 0)
        return 0;
    if (node->type != NODE_SELECT_QUERY)
        return 0;

    ProtocolDataSource *data_source = thrift_parse_data_source(node->select_query.data_source);
    if (!data_source)
        return 0;

    ProtocolSelectQuery *ret = g_object_new(TYPE_PROTOCOL_SELECT_QUERY, 0);
    g_object_set
    (
        ret,
        "data_source", data_source,
        NULL
    );
    
    ProtocolFilter *filter = thrift_parse_filter(node->select_query.filter);
    if (filter)
        g_object_set
        (
            ret,
            "filter", filter,
            NULL
        );

    ProtocolProjection *projection = thrift_parse_projection(node->select_query.projection);
    if (projection)
        g_object_set
        (
            ret,
            "projection", projection,
            NULL
        );
    
    return ret;
}

ProtocolDeleteQuery * thrift_parse_delete_query(Node *node)
{
    if (node == 0)
        return 0;
    if (node->type != NODE_DELETE_QUERY)
        return 0;

    ProtocolDataSource *data_source = thrift_parse_data_source(node->delete_query.data_source);
    if (!data_source)
        return 0;

    ProtocolDeleteQuery *ret = g_object_new(TYPE_PROTOCOL_DELETE_QUERY, 0);
    g_object_set
    (
        ret,
        "data_source", data_source,
        NULL
    );
    
    ProtocolFilter *filter = thrift_parse_filter(node->delete_query.filter);
    if (filter)
        g_object_set
        (
            ret,
            "filter", filter,
            NULL
        );
    
    return ret;
}

ProtocolInsertQuery * thrift_parse_insert_query(Node *node)
{
    if (node == 0)
        return 0;
    if (node->type != NODE_INSERT_QUERY)
        return 0;

    ProtocolVariable *table_name = thrift_parse_variable(node->insert_query.table_name);
    if (!table_name)
        return 0;

    GPtrArray *data = thrift_parse_constants_list_list(node->insert_query.data_tuples);
    if (!data)
        return 0;

    ProtocolInsertQuery *ret = g_object_new(TYPE_PROTOCOL_INSERT_QUERY, 0);
    g_object_set
    (
        ret,
        "table_name", table_name,
        "data", data,
        NULL
    );

    return ret;
}

ProtocolUpdateQuery * thrift_parse_update_query(Node *node)
{
    if (node == 0)
        return 0;
    if (node->type != NODE_UPDATE_QUERY)
        return 0;
    
    ProtocolDataSource *data_source = thrift_parse_data_source(node->update_query.data_source);
    if (!data_source)
        return 0;

    GPtrArray *assignments = thrift_parse_assignments_list(node->update_query.assignments);
    if (!assignments)
        return 0;

    ProtocolUpdateQuery *ret = g_object_new(TYPE_PROTOCOL_UPDATE_QUERY, 0);
    g_object_set
    (
        ret,
        "data_source", data_source,
        "assignments", assignments,
        NULL
    );
    
    ProtocolFilter *filter = thrift_parse_filter(node->update_query.filter);
    if (filter)
        g_object_set
        (
            ret,
            "filter", filter,
            NULL
        );
    
    return ret;
}

ProtocolCreateTableQuery * thrift_parse_create_table_query(Node *node)
{
    if (node == 0)
        return 0;
    if (node->type != NODE_CREATE_TABLE_QUERY)
        return 0;
    
    ProtocolVariable *table_name = thrift_parse_variable(node->create_table_query.table_name);
    if (!table_name)
        return 0;
    
    GPtrArray *defs = thrift_parse_columns_def_list(node->create_table_query.columns_def);
    if (!defs)
        return 0;
    
    ProtocolCreateTableQuery *ret = g_object_new(TYPE_PROTOCOL_CREATE_TABLE_QUERY, 0);
    g_object_set
    (
        ret,
        "table_name", table_name,
        "defs", defs,
        NULL
    );
    return ret;
}

ProtocolDeleteTableQuery * thrift_parse_delete_table_query(Node *node)
{
    if (node == 0)
        return 0;
    if (node->type != NODE_DELETE_TABLE_QUERY)
        return 0;
    
    ProtocolVariable *table_name = thrift_parse_variable(node->delete_table_query.table_name);
    if (!table_name)
        return 0;
    
    ProtocolDeleteTableQuery *ret = g_object_new(TYPE_PROTOCOL_DELETE_TABLE_QUERY, 0);
    g_object_set
    (
        ret,
        "table_name", table_name,
        NULL
    );
    return ret;
}

ProtocolQueryType thrift_parse_query_type(Node *node)
{
    if (node == 0)
        return 0;
    
    switch (node->type)
    {
        case NODE_SELECT_QUERY:
            return PROTOCOL_QUERY_TYPE_SELECT;
        case NODE_DELETE_QUERY:
            return PROTOCOL_QUERY_TYPE_DELETE;
        case NODE_INSERT_QUERY:
            return PROTOCOL_QUERY_TYPE_INSERT;
        case NODE_UPDATE_QUERY:
            return PROTOCOL_QUERY_TYPE_UPDATE;
        case NODE_CREATE_TABLE_QUERY:
            return PROTOCOL_QUERY_TYPE_CREATE_TABLE;
        case NODE_DELETE_TABLE_QUERY:
            return PROTOCOL_QUERY_TYPE_DELETE_TABLE;
        default:
            return 0;
    }
}

ProtocolQueryUnion * thrift_parse_query_union(Node *node)
{
    if (node == 0)
        return 0;

    ProtocolQueryUnion *ret;

    switch (node->type)
    {
        case NODE_SELECT_QUERY:
            ProtocolSelectQuery *select = thrift_parse_select_query(node);
            if (!select)
                return 0;
            ret = g_object_new(TYPE_PROTOCOL_QUERY_UNION, 0);
            g_object_set
            (
                ret,
                "select", select,
                NULL
            );
            return ret;

        case NODE_DELETE_QUERY:
            ProtocolDeleteQuery *delete = thrift_parse_delete_query(node);
            if (!delete)
                return 0;
            ret = g_object_new(TYPE_PROTOCOL_QUERY_UNION, 0);
            g_object_set
            (
                ret,
                "delete", delete,
                NULL
            );
            return ret;

        case NODE_INSERT_QUERY:
            ProtocolInsertQuery *insert = thrift_parse_insert_query(node);
            if (!insert)
                return 0;
            ret = g_object_new(TYPE_PROTOCOL_QUERY_UNION, 0);
            g_object_set
            (
                ret,
                "insert", insert,
                NULL
            );
            return ret;
        
        case NODE_UPDATE_QUERY:
            ProtocolUpdateQuery *update = thrift_parse_update_query(node);
            if (!update)
                return 0;
            ret = g_object_new(TYPE_PROTOCOL_QUERY_UNION, 0);
            g_object_set
            (
                ret,
                "update", update,
                NULL
            );
            return ret;
        
        case NODE_CREATE_TABLE_QUERY:
            ProtocolCreateTableQuery *create_table = thrift_parse_create_table_query(node);
            if (!create_table)
                return 0;
            ret = g_object_new(TYPE_PROTOCOL_QUERY_UNION, 0);
            g_object_set
            (
                ret,
                "create_table", create_table,
                NULL
            );
            return ret;

        case NODE_DELETE_TABLE_QUERY:
            ProtocolDeleteTableQuery *delete_table = thrift_parse_delete_table_query(node);
            if (!delete_table)
                return 0;
            ProtocolQueryUnion *rett = g_object_new(TYPE_PROTOCOL_QUERY_UNION, 0);
            g_object_set
            (
                rett,
                "delete_table", delete_table,
                NULL
            );
            return rett;
        
        default:
            return 0;
    }
}

ProtocolQuery * thrift_parse_query(Node *node)
{
    if (node == 0)
        return 0;
    
    ProtocolQueryType type = thrift_parse_query_type(node);
    if (!type)
        return 0;
    
    ProtocolQueryUnion *query = thrift_parse_query_union(node);
    if (!query)
        return 0;

    ProtocolQuery *ret = g_object_new(TYPE_PROTOCOL_QUERY, 0);
    g_object_set
    (
        ret,
        "type", type,
        "query", query,
        NULL
    );
    return ret;
}
