#ifndef CELL_H_
#define CELL_H_

#include "inttypes.h"
#include "memory.h"

#include "util.h"
#include "slot.h"

typedef enum ColumnType
{
    PRIMARY_PTR,
    SECONDARY_PTR,
    ROW_SLICE,
    BOOL,
    UINT_8,
    UINT_16,
    UINT_32,
    INT_32,
    FLOAT,
    STRING,
    TABLE_SCHEMA,
} ColumnType;


typedef struct VariableSizeCell
{
    uint16_t size;
    uint8_t* payload;
} VariableSizeCell;

MAYBE(VariableSizeCell, VariableSizeCell)

typedef struct PrimaryPtrCell
{
    uint16_t size;
    SlotPtr payload;
} PrimaryPtrCell;

typedef struct SecondaryPtrCell
{
    SlotPtr payload;
} SecondaryPtrCell;

typedef struct BoolCell
{
    uint8_t payload;
} BoolCell;

typedef struct Uint8Cell
{
    uint8_t payload;
} Uint8Cell;

typedef struct Uint16Cell
{
    uint16_t payload;
} Uint16Cell;

typedef struct Uint32Cell
{
    uint32_t payload;
} Uint32Cell;

typedef struct Int32Cell
{
    int32_t payload;
} Int32Cell;

typedef struct FloatCell
{
    float payload;
} FloatCell;

typedef struct Cell
{
    ColumnType type;
    union
    {
        BoolCell bool;
        Uint8Cell uint8;
        Uint16Cell uint16;
        Uint32Cell uint32;
        Int32Cell int32;
        FloatCell _float;
        PrimaryPtrCell primary_ptr;
        SecondaryPtrCell secondary_ptr;
        VariableSizeCell var_size;
    };
} Cell;

LINKED_LIST_HEADER(Cell)
LINKED_LIST_HEADER(Cell_LinkedList)
MAYBE(Cell_LinkedList_LinkedList, Cell_LinkedList_LinkedList)


typedef struct ColumnSchema
{
    char *name;
    ColumnType type;
} ColumnSchema;

LINKED_LIST_HEADER(ColumnSchema)
MAYBE(ColumnSchema_LinkedList, ColumnSchema_LinkedList)


typedef struct CellValue
{
    ColumnType type;
    union
    {
        uint8_t bool;
        uint8_t uint8;
        uint16_t uint16;
        uint32_t uint32;
        int32_t int32;
        float _float;
        char *string;
        ColumnSchema_LinkedList table_schema;
    };
} CellValue;

LINKED_LIST_HEADER(CellValue)
MAYBE(CellValue_LinkedList, CellValue_LinkedList)

uint8_t column_type_is_integer(ColumnType type);

Maybe_uint16_t cell_get_size(Cell *);
uint16_t column_type_get_size(ColumnType type);

Maybe_VariableSizeCell variable_size_cell_new(uint16_t);
void variable_size_cell_free(VariableSizeCell *);
uint8_t cell_is_variable_size(Cell *);

Maybe_Cell cell_extract(ColumnType, RawSlot *);
Maybe_Nothing cell_pack(Cell *, RawSlot *);

ColumnSchema column_schema_new(char *, ColumnType);
uint8_t column_schema_is_variable_size(ColumnSchema_LinkedList *);
uint16_t column_schema_get_size(ColumnSchema_LinkedList *);

void table_schema_free(ColumnSchema_LinkedList *);

ColumnSchema_LinkedList root_table_schema(void);
ColumnSchema_LinkedList free_table_schema(void);
ColumnSchema_LinkedList pool_table_schema(void);

Maybe_Cell cell_clone(Cell *cell);
Maybe_CellValue cell_get_value(Cell *);
Maybe_Cell cell_from_value(CellValue *);

Maybe_CellValue cell_value_new(ColumnType, void *);
int32_t cell_value_to_int32_t(CellValue *value);
Maybe_Nothing cell_value_print(CellValue *);
void cell_value_free(CellValue *);

char * full_column_name_new(char *table_name, char *column_name);
void full_column_name_free(char *full_name);

#endif