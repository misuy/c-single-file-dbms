#ifndef TREE_H_
#define TREE_H_

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <glib-object.h>
#include "../protocol/protocol_types.h"
#include "../protocol/util.h"



typedef struct ErrorNode
{
    uint32_t error_offset;
    char *error_message;
    struct Node *tree;
} ErrorNode;



typedef struct BoolConstantNode
{
    uint8_t value;
} BoolConstantNode;

typedef struct IntegerConstantNode
{
    int32_t value;
} IntegerConstantNode;

typedef struct FloatConstantNode
{
    float value;
} FloatConstantNode;

typedef struct StringConstantNode
{
    char *value;
} StringConstantNode;

typedef struct ConstantsListNode
{
    struct Node *constant;
    struct Node *next;
} ConstantsListNode;



typedef struct VariableNode
{
    char *name;
} VariableNode;



typedef struct ColumnPtrNode
{
    struct Node *table_name;
    struct Node *column_name;
} ColumnPtrNode;



typedef struct ProjectionNode
{
    struct Node *column_ptr;
    struct Node *next;
} ProjectionNode;



typedef enum UnaryOperator
{
    OP_NOT,
} UnaryOperator;

typedef struct UnaryExpressionNode
{
    UnaryOperator op;
    struct Node *operand;
} UnaryExpressionNode;

typedef enum BinaryOperator
{
    OP_AND,
    OP_OR,
    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_LESS,
    OP_NOT_GREATER,
    OP_GREATER,
    OP_NOT_LESS,
    OP_SUBSTR,
} BinaryOperator;

typedef struct BinaryExpressionNode
{
    BinaryOperator op;
    struct Node *left_operand;
    struct Node *right_operand;
} BinaryExpressionNode;



typedef struct TableDataSourceNode
{
    struct Node *table;
    struct Node *variable;
} TableDataSourceNode;

typedef struct JoinDataSourceNode
{
    struct Node *left_data_source;
    struct Node *right_data_source;
    struct Node *expression;
} JoinDataSourceNode;



typedef struct FilterNode
{
    struct Node *expression;
} FilterNode;



typedef enum DataType
{
    TYPE_BOOL,
    TYPE_UINT_8,
    TYPE_UINT_16,
    TYPE_UINT_32,
    TYPE_INT_32,
    TYPE_FLOAT,
    TYPE_STRING,
} DataType;

typedef struct TypeNode
{
    DataType type;
} TypeNode;

typedef struct ColumnDefNode
{
    struct Node *type;
    struct Node *name;
} ColumnDefNode;

typedef struct ColumnsDefListNode
{
    struct Node *def;
    struct Node *next;
} ColumnsDefListNode;



typedef struct AssignmentNode
{
    struct Node *to;
    struct Node *what;
} AssignmentNode;

typedef struct AssignmentsListNode
{
    struct Node *assignment;
    struct Node *next;
} AssignmentsListNode;



typedef struct DataTuplesListNode
{
    struct Node *data_tuple;
    struct Node *next;
} DataTuplesListNode;



typedef struct SelectQueryNode
{
    struct Node *data_source;
    struct Node *filter;
    struct Node *projection;
} SelectQueryNode;



typedef struct DeleteQueryNode
{
    struct Node *data_source;
    struct Node *filter;
} DeleteQueryNode;



typedef struct InsertQueryNode
{
    struct Node *table_name;
    struct Node *data_tuples;
} InsertQueryNode;



typedef struct UpdateQueryNode
{
    struct Node *data_source;
    struct Node *filter;
    struct Node *assignments;
} UpdateQueryNode;



typedef struct CreateTableQueryNode
{
    struct Node *table_name;
    struct Node *columns_def;
} CreateTableQueryNode;



typedef struct DeleteTableQueryNode
{
    struct Node *table_name;
} DeleteTableQueryNode;



typedef enum NodeType
{
    NODE_ERROR,

    NODE_BOOL_CONSTANT,
    NODE_INTEGER_CONSTANT,
    NODE_FLOAT_CONSTANT,
    NODE_STRING_CONSTANT,
    NODE_CONSTANTS_LIST,

    NODE_TYPE,

    NODE_COLUMN_DEF,
    NODE_COLUMNS_DEF_LIST,

    NODE_VARIABLE,

    NODE_COLUMN_PTR,

    NODE_PROJECTION,

    NODE_UNARY_EXPRESSION,
    NODE_BINARY_EXPRESSION,

    NODE_TABLE_DATA_SOURCE,
    NODE_JOIN_DATA_SOURCE,

    NODE_FILTER,

    NODE_ASSIGNMENT,
    NODE_ASSIGNMENTS_LIST,

    NODE_DATA_TUPLES_LIST,

    NODE_SELECT_QUERY,
    NODE_DELETE_QUERY,
    NODE_INSERT_QUERY,
    NODE_UPDATE_QUERY,
    NODE_CREATE_TABLE_QUERY,
    NODE_DELETE_TABLE_QUERY,
} NodeType;


typedef struct Node
{
    NodeType type;
    union
    {
        ErrorNode error;
        BoolConstantNode bool_constant;
        IntegerConstantNode integer_constant;
        FloatConstantNode float_constant;
        StringConstantNode string_constant;
        ConstantsListNode constants_list;
        TypeNode typee;
        ColumnDefNode column_def;
        ColumnsDefListNode columns_def_list;
        VariableNode variable;
        ColumnPtrNode column_ptr;
        ProjectionNode projection;
        UnaryExpressionNode unary_expression;
        BinaryExpressionNode binary_expression;
        TableDataSourceNode table_data_source;
        JoinDataSourceNode join_data_source;
        FilterNode filter;
        AssignmentNode assignment;
        AssignmentsListNode assignments_list;
        DataTuplesListNode data_tuples_list;
        SelectQueryNode select_query;
        DeleteQueryNode delete_query;
        InsertQueryNode insert_query;
        UpdateQueryNode update_query;
        CreateTableQueryNode create_table_query;
        DeleteTableQueryNode delete_table_query;
    };
} Node;



void node_print(FILE *file, Node *node, char *name, uint32_t level);
void node_free(Node *node);
ProtocolQuery * thrift_parse_query(Node *node);

#endif
