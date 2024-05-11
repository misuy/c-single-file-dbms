#ifndef PREDICATE_H_
#define PREDICATE_H_

#include "inttypes.h"

#include "util.h"
#include "cell.h"
#include "row.h"


typedef enum OperandType
{
    VALUE,
    VALUE_PTR,
    PREDICATE,
} OperandType;


typedef struct CellValuePtr
{
    char *name;
} CellValuePtr;


typedef struct Operand
{
    OperandType type;
    uint8_t loaded;
    CellValue value;
    union
    {
        CellValuePtr value_ptr;
        struct Predicate *predicate;
    };
} Operand;

LINKED_LIST_HEADER(Operand)


typedef struct Predicate
{
    Maybe_uint8_t (*operator) (CellValue, CellValue);
    Operand left_operand;
    Operand right_operand;
} Predicate;

MAYBE(Predicate, Predicate)
MAYBE(Predicate *, PredicatePtr)


Operand operand_new_by_value(CellValue);
Operand operand_new_by_ptr(char *);
Operand operand_new_by_predicate(Predicate *predicate);

Predicate * predicate_new(Maybe_uint8_t (*operator) (CellValue, CellValue), Operand left, Operand right);
void predicate_free(Predicate *predicate);

Maybe_uint8_t predicate_apply(Row *, Predicate *);

Maybe_uint8_t operator_ok(CellValue left_value, CellValue right_value);
Maybe_uint8_t operator_not(CellValue left_value, CellValue right_value);
Maybe_uint8_t operator_and(CellValue left_value, CellValue right_value);
Maybe_uint8_t operator_or(CellValue left_value, CellValue right_value);
Maybe_uint8_t operator_equal(CellValue left_value, CellValue right_value);
Maybe_uint8_t operator_not_equal(CellValue left_value, CellValue right_value);
Maybe_uint8_t operator_less(CellValue left_value, CellValue right_value);
Maybe_uint8_t operator_not_less(CellValue left_value, CellValue right_value);
Maybe_uint8_t operator_greater(CellValue left_value, CellValue right_value);
Maybe_uint8_t operator_not_greater(CellValue left_value, CellValue right_value);
Maybe_uint8_t operator_substr(CellValue left_value, CellValue right_value);

#endif