%{
    #include <stdlib.h>
    #include <stdio.h>

    #include "lex.h"
%}

%define api.value.type { struct Node * }
%define api.pure full
%define api.push-pull both

%parse-param { struct FiniteAutomata *automata }
%lex-param { struct FiniteAutomata *automata }

%parse-param { struct Text *text }
%lex-param { struct Text *text }

%parse-param { struct Node **result_node }

%token LBRACE
%token RBRACE
%token DOT
%token COMMA
%token ASTERISK
%token SEMICOLON

%token FROM
%token IN
%token WHERE
%token SELECT
%token INSERT
%token DELETE
%token UPDATE
%left JOIN
%token ON
%token CREATE

%token VARIABLE
%token BOOL_CONSTANT
%token INTEGER_CONSTANT
%token FLOAT_CONSTANT
%token STRING_CONSTANT

%token BOOL_TYPE
%token UINT_8_TYPE
%token UINT_16_TYPE
%token UINT_32_TYPE
%token INT_32_TYPE
%token FLOAT_TYPE
%token STRING_TYPE

%left ASSIGN

%left AND
%left OR

%left EQUAL
%left NOT_EQUAL
%left LESS
%left NOT_LESS
%left GREATER
%left NOT_GREATER
%left SUBSTR

%right NOT



%%
query:
    select_query
    {
        *result_node = $1;
    }
|   delete_query
    {
        *result_node = $1;
    }
|   insert_query
    {
        *result_node = $1;
    }
|   update_query
    {
        *result_node = $1;
    }
|   create_table_query
    {
        *result_node = $1;
    }
|   delete_table_query
    {
        *result_node = $1;
    }
;

column_ptr:
    VARIABLE DOT VARIABLE
    { 
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_COLUMN_PTR;
        $$->column_ptr.table_name = $1;
        $$->column_ptr.column_name = $3;
        *result_node = $$;
    }
;

projection:
    ASTERISK
    {
        $$ = 0;
    }
|   column_ptr
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_PROJECTION;
        $$->projection.column_ptr = $1;
        $$->projection.next = 0;
        *result_node = $$;
    }
|   projection COMMA column_ptr
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_PROJECTION;
        $$->projection.column_ptr = $3;
        $$->projection.next = $1;
        *result_node = $$;
    }
|   LBRACE projection RBRACE
    {
        $$ = $2;
        *result_node = $$;
    }
;

constant:
    BOOL_CONSTANT
    {
        *result_node = $$;
    }
|   INTEGER_CONSTANT
    {
        *result_node = $$;
    }
|   FLOAT_CONSTANT
    {
        *result_node = $$;
    }
|   STRING_CONSTANT
    {
        *result_node = $$;
    }
;

operand:
    column_ptr
    {
        *result_node = $$;
    }
|   constant
    {
        *result_node = $$;
    }
;

expression:
    operand
    {
        *result_node = $$;
    }
|   expression EQUAL expression
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_BINARY_EXPRESSION;
        $$->binary_expression.op = OP_EQUAL;
        $$->binary_expression.left_operand = $1;
        $$->binary_expression.right_operand = $3;
        *result_node = $$;
    }
|   expression NOT_EQUAL expression
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_BINARY_EXPRESSION;
        $$->binary_expression.op = OP_NOT_EQUAL;
        $$->binary_expression.left_operand = $1;
        $$->binary_expression.right_operand = $3;
        *result_node = $$;
    }
|   expression LESS expression
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_BINARY_EXPRESSION;
        $$->binary_expression.op = OP_LESS;
        $$->binary_expression.left_operand = $1;
        $$->binary_expression.right_operand = $3;
        *result_node = $$;
    }
|   expression NOT_LESS expression
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_BINARY_EXPRESSION;
        $$->binary_expression.op = OP_NOT_LESS;
        $$->binary_expression.left_operand = $1;
        $$->binary_expression.right_operand = $3;
        *result_node = $$;
    }
|   expression GREATER expression
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_BINARY_EXPRESSION;
        $$->binary_expression.op = OP_GREATER;
        $$->binary_expression.left_operand = $1;
        $$->binary_expression.right_operand = $3;
        *result_node = $$;
    }
|   expression NOT_GREATER expression
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_BINARY_EXPRESSION;
        $$->binary_expression.op = OP_NOT_GREATER;
        $$->binary_expression.left_operand = $1;
        $$->binary_expression.right_operand = $3;
        *result_node = $$;
    }
|   expression SUBSTR expression
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_BINARY_EXPRESSION;
        $$->binary_expression.op = OP_SUBSTR;
        $$->binary_expression.left_operand = $1;
        $$->binary_expression.right_operand = $3;
        *result_node = $$;
    }
|   NOT expression
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_UNARY_EXPRESSION;
        $$->unary_expression.op = OP_NOT;
        $$->unary_expression.operand = $2;
        *result_node = $$;
    }
|   expression AND expression
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_BINARY_EXPRESSION;
        $$->binary_expression.op = OP_AND;
        $$->binary_expression.left_operand = $1;
        $$->binary_expression.right_operand = $3;
        *result_node = $$;
    }
|   expression OR expression
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_BINARY_EXPRESSION;
        $$->binary_expression.op = OP_OR;
        $$->binary_expression.left_operand = $1;
        $$->binary_expression.right_operand = $3;
        *result_node = $$;
    }
|   LBRACE expression RBRACE
    {
        $$ = $2;
        *result_node = $$;
    }
;

table_data_source:
    VARIABLE IN VARIABLE
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_TABLE_DATA_SOURCE;
        $$->table_data_source.table = $3;
        $$->table_data_source.variable = $1;
        *result_node = $$;
    }
;

data_source:
    LBRACE data_source RBRACE
    {
        $$ = $2;
        *result_node = $$;
    }
|   table_data_source
    {
        *result_node = $$;
    }
|   data_source JOIN table_data_source ON expression
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_JOIN_DATA_SOURCE;
        $$->join_data_source.left_data_source = $1;
        $$->join_data_source.right_data_source = $3;
        $$->join_data_source.expression = $5;
        *result_node = $$;
    }
;

filter:
    WHERE expression
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_FILTER;
        $$->filter.expression = $2;
        *result_node = $$;
    }
;

type:
    BOOL_TYPE
    {
        *result_node = $$;
    }
|   UINT_8_TYPE
    {
        *result_node = $$;
    }
|   UINT_16_TYPE
    {
        *result_node = $$;
    }
|   UINT_32_TYPE
    {
        *result_node = $$;
    }
|   INT_32_TYPE
    {
        *result_node = $$;
    }
|   FLOAT_TYPE
    {
        *result_node = $$;
    }
|   STRING_TYPE
    {
        *result_node = $$;
    }
;

column_def:
    type VARIABLE
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_COLUMN_DEF;
        $$->column_def.type = $1;
        $$->column_def.name = $2;
        *result_node = $$;
    }
;

columns_def_list:
    column_def
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_COLUMNS_DEF_LIST;
        $$->columns_def_list.def = $1;
        $$->columns_def_list.next = 0;
        *result_node = $$;
    }
|   LBRACE columns_def_list RBRACE
    {
        $$ = $2;
        *result_node = $$;
    }
|   columns_def_list COMMA column_def
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_COLUMNS_DEF_LIST;
        $$->columns_def_list.def = $3;
        $$->columns_def_list.next = $1;
        *result_node = $$;
    }
;

assignment:
    column_ptr ASSIGN operand
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_ASSIGNMENT;
        $$->assignment.to = $1;
        $$->assignment.what = $3;
        *result_node = $$;
    }
;

assignments_list:
    assignment
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_ASSIGNMENTS_LIST;
        $$->assignments_list.assignment = $1;
        $$->assignments_list.next = 0;
        *result_node = $$;
    }
|   assignments_list COMMA assignment
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_ASSIGNMENTS_LIST;
        $$->assignments_list.assignment = $3;
        $$->assignments_list.next = $1;
        *result_node = $$;
    }
;

constants_list:
    constant
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_CONSTANTS_LIST;
        $$->constants_list.constant = $1;
        $$->constants_list.next = 0;
        *result_node = $$;
    }
|   constants_list COMMA constant
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_CONSTANTS_LIST;
        $$->constants_list.constant = $3;
        $$->constants_list.next = $1;
        *result_node = $$;
    }
;

data_tuple:
    LBRACE constants_list RBRACE
    {
        $$ = $2;
        *result_node = $$;
    }
;

data_tuples_list:
    data_tuple
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_DATA_TUPLES_LIST;
        $$->data_tuples_list.data_tuple = $1;
        $$->data_tuples_list.next = 0;
        *result_node = $$;
    }
|   LBRACE data_tuples_list RBRACE
    {
        $$ = $2;
        *result_node = $$;
    }
|   data_tuples_list COMMA data_tuple
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_DATA_TUPLES_LIST;
        $$->data_tuples_list.data_tuple = $3;
        $$->data_tuples_list.next = $1;
        *result_node = $$;
    }
;

select_query:
    FROM data_source SELECT projection SEMICOLON
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_SELECT_QUERY;
        $$->select_query.data_source = $2;
        $$->select_query.filter = 0;
        $$->select_query.projection = $4;
        *result_node = $$;
    }
|   FROM data_source filter SELECT projection SEMICOLON
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_SELECT_QUERY;
        $$->select_query.data_source = $2;
        $$->select_query.filter = $3;
        $$->select_query.projection = $5;
        *result_node = $$;
    }
;

delete_query:
    FROM data_source DELETE VARIABLE SEMICOLON
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_DELETE_QUERY;
        $$->delete_query.data_source = $2;
        $$->delete_query.filter = 0;
        *result_node = $$;
    }
|   FROM data_source filter DELETE VARIABLE SEMICOLON
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_DELETE_QUERY;
        $$->delete_query.data_source = $2;
        $$->delete_query.filter = $3;
        *result_node = $$;
    }
;

insert_query:
    IN VARIABLE INSERT data_tuples_list SEMICOLON
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_INSERT_QUERY;
        $$->insert_query.table_name = $2;
        $$->insert_query.data_tuples = $4;
        *result_node = $$;
    }
;

update_query:
    FROM data_source UPDATE assignments_list SEMICOLON
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_UPDATE_QUERY;
        $$->update_query.data_source = $2;
        $$->update_query.filter = 0;
        $$->update_query.assignments = $4;
        *result_node = $$;
    }
|   FROM data_source filter UPDATE assignments_list SEMICOLON
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_UPDATE_QUERY;
        $$->update_query.data_source = $2;
        $$->update_query.filter = $3;
        $$->update_query.assignments = $5;
        *result_node = $$;
    }
;

create_table_query:
    CREATE VARIABLE columns_def_list SEMICOLON
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_CREATE_TABLE_QUERY;
        $$->create_table_query.table_name = $2;
        $$->create_table_query.columns_def = $3;
        *result_node = $$;
    }
;

delete_table_query:
    DELETE VARIABLE SEMICOLON
    {
        $$ = malloc(sizeof(struct Node));
        $$->type = NODE_DELETE_TABLE_QUERY;
        $$->delete_table_query.table_name = $2;
        *result_node = $$;
    }
;