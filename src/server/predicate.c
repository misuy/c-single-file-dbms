#include "predicate.h"

void operand_free(Operand *operand);
Maybe_Nothing operand_load(Row *row, Operand *operand);


LINKED_LIST(Operand)


Operand operand_new_by_value(CellValue value)
{
    return (Operand) { .type = VALUE, .loaded = 1, .value = value };
}

Operand operand_new_by_ptr(char *name)
{
    return (Operand) { .type = VALUE_PTR, .loaded = 0, .value_ptr = (CellValuePtr) { .name = name } };
}

Operand operand_new_by_predicate(Predicate *predicate)
{
    return (Operand) { .type = PREDICATE, .loaded = 0, .predicate = predicate };
}

void operand_free(Operand *operand)
{
    if (!operand)
        return;
    if (operand->type == VALUE)
        cell_value_free(&operand->value);
    if (operand->type == PREDICATE)
        predicate_free(operand->predicate);
}


Predicate * predicate_new(Maybe_uint8_t (*operator) (CellValue, CellValue), Operand left, Operand right)
{
    Predicate *predicate = malloc(sizeof(Predicate));
    *predicate = (Predicate) { .operator = operator, .left_operand = left, .right_operand = right };
    return predicate;
}

void predicate_free(Predicate *predicate)
{
    if (predicate == 0)
        return;
    operand_free(&predicate->left_operand);
    operand_free(&predicate->right_operand);
    free(predicate);
}


Maybe_Nothing operand_load(Row *row, Operand *operand)
{
    if (operand->loaded)
        OK(Nothing);
    if (operand->type == VALUE)
    {
        operand->loaded = 1;
    }
    if (operand->type == VALUE_PTR)
    {
        Cell cell = HANDLE(Nothing, Cell, row_get_cell_by_name(row, operand->value_ptr.name));
        operand->value = HANDLE(Nothing, CellValue, cell_get_value(&cell));
        operand->loaded = 1;
    }
    if (operand->type == PREDICATE)
    {
        uint8_t result = HANDLE(Nothing, uint8_t, predicate_apply(row, operand->predicate));
        operand->value = HANDLE(Nothing, CellValue, cell_value_new(UINT_8, &result));
        operand->loaded = 1;
    }
    OK(Nothing);
}


Maybe_uint8_t predicate_apply(Row *row, Predicate *predicate)
{
    if (predicate == 0)
        RESULT(uint8_t, 1);

    HANDLE_NOTHING(uint8_t, operand_load(row, &predicate->left_operand))
    HANDLE_NOTHING(uint8_t, operand_load(row, &predicate->right_operand))

    uint8_t res = HANDLE(uint8_t, uint8_t, predicate->operator(predicate->left_operand.value, predicate->right_operand.value));
    predicate->left_operand.loaded = 0;
    predicate->right_operand.loaded = 0;
    RESULT(uint8_t, res);
}



// default operators
Maybe_uint8_t operator_ok(CellValue left_value, CellValue right_value)
{
    if (!column_type_is_integer(left_value.type))
        RESULT(uint8_t, 0);
    
    if (cell_value_to_int32_t(&left_value))
        RESULT(uint8_t, 1);
    
    RESULT(uint8_t, 0);
}

Maybe_uint8_t operator_not(CellValue left_value, CellValue right_value)
{
    switch (left_value.type)
    {
        case BOOL:
            RESULT(uint8_t, !left_value.bool);

        case UINT_8:
            RESULT(uint8_t, !left_value.uint8);
        
        case UINT_16:
            RESULT(uint8_t, !left_value.uint16);

        case UINT_32:
            RESULT(uint8_t, !left_value.uint32);

        case INT_32:
            RESULT(uint8_t, !left_value.int32);
        
        default:
            EXCEPTION(uint8_t, "illegal operand type");
    }
}

Maybe_uint8_t operator_and(CellValue left_value, CellValue right_value)
{
    if (left_value.type != right_value.type)
        if (!(column_type_is_integer(left_value.type) & column_type_is_integer(right_value.type)))
            EXCEPTION(uint8_t, "different operand types");

    switch (left_value.type)
    {
        case BOOL:
        case UINT_8:
        case UINT_16:
        case UINT_32:
        case INT_32:
            RESULT(uint8_t, cell_value_to_int32_t(&left_value) & cell_value_to_int32_t(&right_value));
        
        default:
            EXCEPTION(uint8_t, "illegal operand type");
    }
}

Maybe_uint8_t operator_or(CellValue left_value, CellValue right_value)
{
    if (left_value.type != right_value.type)
        if (!(column_type_is_integer(left_value.type) & column_type_is_integer(right_value.type)))
            EXCEPTION(uint8_t, "different operand types");

    switch (left_value.type)
    {
        case BOOL:
        case UINT_8:
        case UINT_16:
        case UINT_32:
        case INT_32:
            RESULT(uint8_t, cell_value_to_int32_t(&left_value) | cell_value_to_int32_t(&right_value));
        
        default:
            EXCEPTION(uint8_t, "illegal operand type");
    }

}

Maybe_uint8_t operator_equal(CellValue left_value, CellValue right_value)
{
    if (left_value.type != right_value.type)
        if (!(column_type_is_integer(left_value.type) & column_type_is_integer(right_value.type)))
            EXCEPTION(uint8_t, "different operand types");

    switch (left_value.type)
    {
        case BOOL:
        case UINT_8:
        case UINT_16:
        case UINT_32:
        case INT_32:
            RESULT(uint8_t, cell_value_to_int32_t(&left_value) == cell_value_to_int32_t(&right_value));
        
        case FLOAT:
            RESULT(uint8_t, left_value._float == right_value._float);
        
        case STRING:
            int res = strcmp(left_value.string, right_value.string);
            RESULT(uint8_t, res == 0);

        default:
            EXCEPTION(uint8_t, "illegal operand type");
    }
}

Maybe_uint8_t operator_not_equal(CellValue left_value, CellValue right_value)
{
    uint8_t equal = HANDLE(uint8_t, uint8_t, operator_equal(left_value, right_value));
    RESULT(uint8_t, !equal);
}

Maybe_uint8_t operator_less(CellValue left_value, CellValue right_value)
{
    if (left_value.type != right_value.type)
        if (!(column_type_is_integer(left_value.type) & column_type_is_integer(right_value.type)))
            EXCEPTION(uint8_t, "different operand types");

    switch (left_value.type)
    {
        case BOOL:
        case UINT_8:
        case UINT_16:
        case UINT_32:
        case INT_32:
            RESULT(uint8_t, cell_value_to_int32_t(&left_value) < cell_value_to_int32_t(&right_value));
        
        case FLOAT:
            RESULT(uint8_t, left_value._float < right_value._float);
        
        case STRING:
            int res = strcmp(left_value.string, right_value.string);
            RESULT(uint8_t, res < 0);

        default:
            EXCEPTION(uint8_t, "illegal operand type");
    }
}

Maybe_uint8_t operator_not_less(CellValue left_value, CellValue right_value)
{
    uint8_t less = HANDLE(uint8_t, uint8_t, operator_less(left_value, right_value));
    RESULT(uint8_t, !less);
}

Maybe_uint8_t operator_greater(CellValue left_value, CellValue right_value)
{
    if (left_value.type != right_value.type)
        if (!(column_type_is_integer(left_value.type) & column_type_is_integer(right_value.type)))
            EXCEPTION(uint8_t, "different operand types");

    switch (left_value.type)
    {
        case BOOL:
        case UINT_8:
        case UINT_16:
        case UINT_32:
        case INT_32:
            RESULT(uint8_t, cell_value_to_int32_t(&left_value) > cell_value_to_int32_t(&right_value));
        
        case FLOAT:
            RESULT(uint8_t, left_value._float > right_value._float);
        
        case STRING:
            int res = strcmp(left_value.string, right_value.string);
            RESULT(uint8_t, res > 0);

        default:
            EXCEPTION(uint8_t, "illegal operand type");
    }
}

Maybe_uint8_t operator_not_greater(CellValue left_value, CellValue right_value)
{
    uint8_t greater = HANDLE(uint8_t, uint8_t, operator_greater(left_value, right_value));
    RESULT(uint8_t, !greater);
}

Maybe_uint8_t operator_substr(CellValue left_value, CellValue right_value)
{
    if (left_value.type != right_value.type)
        EXCEPTION(uint8_t, "different operand types");
    if (left_value.type != STRING)
        EXCEPTION(uint8_t, "illegal operand type");
    
    if (strlen(left_value.string) > strlen(right_value.string))
        RESULT(uint8_t, 0);
    
    for (uint32_t i = 0; i <= strlen(right_value.string) - strlen(left_value.string); i++)
    {
        uint8_t equal = 1;
        for (uint32_t j = 0; j < strlen(left_value.string); j++)
        {
            if (right_value.string[i + j] != left_value.string[i + j])
            {
                equal = 0;
                break;
            }
        }
        if (equal)
            RESULT(uint8_t, 1);
    }
    RESULT(uint8_t, 0);
}