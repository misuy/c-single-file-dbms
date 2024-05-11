enum ProtocolDataType
{
    BOOL = 1,
    UINT_8,
    UINT_16,
    UINT_32,
    INT_32,
    FLOAT,
    STRING,
}



enum ProtocolConstantType
{
    BOOL = 1,
    INTEGER,
    FLOAT,
    STRING,
}

union ProtocolConstantUnion
{
    1: bool bool_constant,
    2: i32 integer_constant,
    3: double float_constant,
    4: string string_constant,
}

struct ProtocolConstant
{
    1: ProtocolConstantType type,
    2: ProtocolConstantUnion value,
}



struct ProtocolVariable
{
    1: string name,
}



struct ProtocolColumnPtr
{
    1: ProtocolVariable table_name,
    2: ProtocolVariable column_name,
}



struct ProtocolProjection
{
    1: list<ProtocolColumnPtr> column_ptrs,
}



enum ProtocolExpressionNodeType
{
    OPERAND = 1,
    OPERATOR,
}

enum ProtocolExpressionOperandType
{
    CONSTANT = 1,
    COLUMN_PTR,
}

union ProtocolExpressionOperandUnion
{
    1: ProtocolConstant constant_operand,
    2: ProtocolColumnPtr column_ptr_operand,
}

struct ProtocolExpressionOperand
{
    1: ProtocolExpressionOperandType type,
    2: ProtocolExpressionOperandUnion operand,
}

enum ProtocolOperatorType
{
    NOT = 1,
    AND,
    OR,
    EQUAL,
    NOT_EQUAL,
    LESS,
    NOT_LESS,
    GREATER,
    NOT_GREATER,
    SUBSTR,
}

struct ProtocolExpressionOperator
{
    1: ProtocolOperatorType type,
}

union ProtocolExpressionNodeUnion
{
    1: ProtocolExpressionOperand operand,
    2: ProtocolExpressionOperator operator,
}

struct ProtocolExpressionNode
{
    1: ProtocolExpressionNodeType type,
    2: ProtocolExpressionNodeUnion value,
}

struct ProtocolExpression
{
    1: list<ProtocolExpressionNode> list,
}



enum ProtocolDataSourceNodeType
{
    TABLE = 1,
    EXPRESSION,
}

struct ProtocolDataSourceTable
{
    1: ProtocolVariable table_name,
    2: ProtocolVariable variable_name,
}

union ProtocolDataSourceUnion
{
    1: ProtocolDataSourceTable table,
    2: ProtocolExpression expression,
}

struct ProtocolDataSourceNode
{
    1: ProtocolDataSourceNodeType type,
    2: ProtocolDataSourceUnion value,
}

struct ProtocolDataSource
{
    1: list<ProtocolDataSourceNode> list,
}



struct ProtocolAssignment
{
    1: ProtocolColumnPtr to;
    2: ProtocolExpressionOperand what;
}



struct ProtocolFilter
{
    1: ProtocolExpression expression,
}



struct ProtocolColumnDef
{
    1: ProtocolDataType type;
    2: ProtocolVariable name;
}



struct ProtocolSelectQuery
{
    1: ProtocolDataSource data_source,
    2: optional ProtocolFilter filter,
    3: optional ProtocolProjection projection,
}



struct ProtocolDeleteQuery
{
    1: ProtocolDataSource data_source,
    2: optional ProtocolFilter filter,
}



struct ProtocolInsertQuery
{
    1: ProtocolVariable table_name,
    2: list<list<ProtocolConstant>> data,
}



struct ProtocolUpdateQuery
{
    1: ProtocolDataSource data_source,
    2: optional ProtocolFilter filter,
    3: list<ProtocolAssignment> assignments,
}



struct ProtocolCreateTableQuery
{
    1: ProtocolVariable table_name,
    2: list<ProtocolColumnDef> defs,
}



struct ProtocolDeleteTableQuery
{
    1: ProtocolVariable table_name,
}



enum ProtocolQueryType
{
    SELECT = 1,
    DELETE,
    INSERT,
    UPDATE,
    CREATE_TABLE,
    DELETE_TABLE,
}



union ProtocolQueryUnion
{
    1: ProtocolSelectQuery select,
    2: ProtocolDeleteQuery delete,
    3: ProtocolInsertQuery insert,
    4: ProtocolUpdateQuery update,
    5: ProtocolCreateTableQuery create_table,
    6: ProtocolDeleteTableQuery delete_table,
}



struct ProtocolQuery
{
    1: ProtocolQueryType type,
    2: ProtocolQueryUnion query,
}





enum ProtocolQueryResultCode
{
    OK = 1,
    ERROR,
}

struct SelectQueryResult
{
    1: list<ProtocolColumnDef> defs,
    2: i32 count,
    3: list<list<ProtocolConstant>> data,
}

struct InsertQueryResult
{
    1: i32 count,
}

struct DeleteQueryResult
{
    1: i32 count,
}

struct UpdateQueryResult
{
    1: i32 count,
}

struct CreateTableQueryResult
{
    1: list<ProtocolColumnDef> defs,
}

struct DeleteTableQueryResult
{
}

struct ProtocolQueryResult
{
    1: ProtocolQueryResultCode code,
    2: ProtocolQueryType type,
    3: SelectQueryResult select,
    4: InsertQueryResult insert,
    5: DeleteQueryResult delete,
    6: UpdateQueryResult update,
    7: CreateTableQueryResult create_table,
    8: DeleteTableQueryResult delete_table,
    9: string error_message,
}

service QueryExecutor
{
    ProtocolQueryResult execute(1: ProtocolQuery query),
}
