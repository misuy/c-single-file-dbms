#include "transformation.h"

LINKED_LIST(Transformation);


CellValuePtr arg_ptr_new(char *name)
{
    return (CellValuePtr) { .name = name };
}

TransformationOperand transformation_operand_new_by_value(CellValue value)
{
    return (TransformationOperand) { .type = TRANSFORMATION_VALUE, .value = value };
}

TransformationOperand transformation_operand_new_by_ptr(char *name)
{
    return (TransformationOperand) { .type = TRANSFORMATION_VALUE_PTR, .value_ptr = arg_ptr_new(name) };
}

Transformation transformation_new(CellValuePtr to, TransformationOperand what, Maybe_CellValue (*transform) (CellValue))
{
    return (Transformation) { .to = to, .what = what, .transform = transform };
}

Transformation_LinkedList transformations_new(void)
{
    return Transformation_linked_list_new();
}

void transformations_add_transformation(Transformation_LinkedList *transformations, Transformation transformation)
{
    Transformation_linked_list_push_back(transformations, transformation);
}

void transformations_free(Transformation_LinkedList *transformations)
{
    Transformation_linked_list_free(transformations);
}

Maybe_Nothing transformation_apply(Transformation *transformation, Row *row)
{
    if ((transformation == NULL) | (row == NULL))
        EXCEPTION(Nothing, "null ptr");
    CellValue value;
    if (transformation->what.type == TRANSFORMATION_VALUE_PTR)
    {
        Cell cell = HANDLE(Nothing, Cell, row_get_cell_by_name(row, transformation->what.value_ptr.name));
        value = HANDLE(Nothing, CellValue, cell_get_value(&cell));
    }
    else
        value = transformation->what.value;

    
    CellValue transformed_value = HANDLE(Nothing, CellValue, transformation->transform(value));
    Cell transformed_cell = HANDLE(Nothing, Cell, cell_from_value(&transformed_value));
    HANDLE_NOTHING(Nothing, row_set_cell_by_name(row, transformed_cell, transformation->to.name));
    OK(Nothing);
}

Maybe_Nothing transformations_apply(Transformation_LinkedList *transformations, Row *row)
{
    if ((transformations == NULL) | (row == NULL)) EXCEPTION(Nothing, "null ptr");
    Transformation_LinkedListNode *it = transformations->head;
    while (it != NULL)
    {
        HANDLE_NOTHING(Nothing, transformation_apply(&it->value, row));
        it = it->next;
    }
    OK(Nothing);
}



// default transformations
Maybe_CellValue transformation_ok(CellValue value)
{
    CellValue new_value = HANDLE(CellValue, CellValue, cell_value_new(value.type, (void *) &value.uint8));
    RESULT(CellValue, new_value);
}