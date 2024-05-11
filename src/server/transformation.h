#ifndef TRANSFORMATION_H_
#define TRANSFORMATION_H_

#include "predicate.h"

typedef enum TransformationOperandType
{
    TRANSFORMATION_VALUE,
    TRANSFORMATION_VALUE_PTR,
} TransformationOperandType;

typedef struct TransformationOperand
{
    TransformationOperandType type;
    union
    {
        CellValue value;
        CellValuePtr value_ptr;
    };
} TransformationOperand;

typedef struct Transformation
{
    CellValuePtr to;
    TransformationOperand what;
    Maybe_CellValue (*transform) (CellValue);
} Transformation;

LINKED_LIST_HEADER(Transformation);
MAYBE(Transformation_LinkedList, Transformation_LinkedList)


CellValuePtr arg_ptr_new(char *name);

TransformationOperand transformation_operand_new_by_value(CellValue value);
TransformationOperand transformation_operand_new_by_ptr(char *name);

Transformation transformation_new(CellValuePtr to, TransformationOperand what, Maybe_CellValue (*transform) (CellValue));
Transformation_LinkedList transformations_new(void);
void transformations_add_transformation(Transformation_LinkedList *transformations, Transformation transformation);
void transformations_free(Transformation_LinkedList *transformations);
Maybe_Nothing transformation_apply(Transformation *transformation, Row *row);
Maybe_Nothing transformations_apply(Transformation_LinkedList *transformations, Row *row);

Maybe_CellValue transformation_ok(CellValue value);

#endif