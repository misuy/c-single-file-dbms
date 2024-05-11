#include "cell.h"

LINKED_LIST(Cell_LinkedList)

char* column_type_get_name(ColumnType type);

// sizes
Maybe_uint16_t cell_get_size(Cell *cell)
{
    if (cell == NULL) EXCEPTION(uint16_t, "null ptr");
    switch (cell->type)
    {
        case PRIMARY_PTR:
            RESULT(uint16_t, sizeof(PrimaryPtrCell));
        case SECONDARY_PTR:
            RESULT(uint16_t, sizeof(SecondaryPtrCell));
        case BOOL:
            RESULT(uint16_t, sizeof(BoolCell));
        case UINT_8:
            RESULT(uint16_t, sizeof(Uint8Cell));
        case UINT_16:
            RESULT(uint16_t, sizeof(Uint16Cell));
        case UINT_32:
            RESULT(uint16_t, sizeof(Uint32Cell));
        case INT_32:
            RESULT(uint16_t, sizeof(Int32Cell));
        case FLOAT:
            RESULT(uint16_t, sizeof(FloatCell));
        case ROW_SLICE:
        case STRING:
        case TABLE_SCHEMA:
            RESULT(uint16_t, sizeof(uint16_t) + cell->var_size.size);
        default:
            EXCEPTION(uint16_t, "unsupported cell type");
    }
}

// 0 for variable size columns
uint16_t column_type_get_size(ColumnType type)
{
    switch (type)
    {
        case PRIMARY_PTR:
            return sizeof(PrimaryPtrCell);
        case SECONDARY_PTR:
            return sizeof(SecondaryPtrCell);
        case BOOL:
            return sizeof(BoolCell);
        case UINT_8:
            return sizeof(Uint8Cell);
        case UINT_16:
            return sizeof(Uint16Cell);
        case UINT_32:
            return sizeof(Uint32Cell);
        case INT_32:
            return sizeof(Int32Cell);
        case FLOAT:
            return sizeof(FloatCell);
        case ROW_SLICE:
        case STRING:
        case TABLE_SCHEMA:
        default:
            return 0;
    }
}

uint8_t column_type_is_integer(ColumnType type)
{
    switch (type)
    {
        case BOOL:
        case UINT_8:
        case UINT_16:
        case UINT_32:
        case INT_32:
            return 1;
        default:
            return 0;
    }
}

char* column_type_get_name(ColumnType type)
{
    switch (type)
    {
        case BOOL:
            return "bool";
        case UINT_8:
            return "uint_8";
        case UINT_16:
            return "uint_16";
        case UINT_32:
            return "uint_32";
        case INT_32:
            return "int_32";
        case FLOAT:
            return "float";
        case PRIMARY_PTR:
            return "primary_ptr";
        case SECONDARY_PTR:
            return "secondary_ptr";
        case ROW_SLICE:
            return "row_slice";
        case STRING:
            return "string";
        case TABLE_SCHEMA:
            return "table_schema";
        default:
            return "unsupported";
    }
}



// variable size cells
Maybe_VariableSizeCell variable_size_cell_new(uint16_t size)
{
    uint8_t *ptr = malloc(size);
    if (ptr == NULL)
        EXCEPTION(VariableSizeCell, "not enough memory for a cell");
    RESULT(VariableSizeCell, ((VariableSizeCell) { .size = size, .payload = ptr }));
}

void variable_size_cell_free(VariableSizeCell *cell)
{
    if (cell->payload != NULL)
        free(cell->payload);
}

uint8_t cell_is_variable_size(Cell *cell)
{
    switch (cell->type)
    {
        case PRIMARY_PTR:
        case SECONDARY_PTR:
        case BOOL:
        case UINT_8:
        case UINT_16:
        case UINT_32:
        case INT_32:
        case FLOAT:
            return 0;
        default:
            return 1;
    }
}



// schemas
ColumnSchema column_schema_new(char *name, ColumnType type)
{
    return (ColumnSchema) { .name = name, .type = type };
}

uint8_t column_schema_is_variable_size(ColumnSchema_LinkedList *schema)
{
    ColumnSchema_LinkedListNode *it = schema->head;
    while (it != 0)
    {
        if (column_type_get_size(it->value.type) == 0) return 1;
        it = it->next;
    }
    return 0;
}

uint16_t column_schema_get_size(ColumnSchema_LinkedList *schema)
{
    ColumnSchema_LinkedListNode *it = schema->head;
    uint16_t size = 0;
    while (it != 0)
    {
        uint16_t column_size = column_type_get_size(it->value.type);
        if (column_size == 0) return column_type_get_size(PRIMARY_PTR);
        size += column_size;
        it = it->next;
    }
    return size;
}

void table_schema_free(ColumnSchema_LinkedList *schema)
{
    ColumnSchema_linked_list_free(schema);
}



ColumnSchema_LinkedList root_table_schema(void)
{
    ColumnSchema_LinkedList schema = ColumnSchema_linked_list_new();
    ColumnSchema_linked_list_push_back(&schema, column_schema_new("id", UINT_16));
    ColumnSchema_linked_list_push_back(&schema, column_schema_new("name", STRING));
    ColumnSchema_linked_list_push_back(&schema, column_schema_new("is_system", BOOL));
    ColumnSchema_linked_list_push_back(&schema, column_schema_new("is_data", BOOL));
    ColumnSchema_linked_list_push_back(&schema, column_schema_new("schema", TABLE_SCHEMA));
    ColumnSchema_linked_list_push_back(&schema, column_schema_new("row_size", UINT_16));
    ColumnSchema_linked_list_push_back(&schema, column_schema_new("first_page_id", UINT_32));
    ColumnSchema_linked_list_push_back(&schema, column_schema_new("first_full_page_id", UINT_32));
    return schema;
}

ColumnSchema_LinkedList free_table_schema(void)
{
    ColumnSchema_LinkedList schema = ColumnSchema_linked_list_new();
    return schema;
}

ColumnSchema_LinkedList pool_table_schema(void)
{
    ColumnSchema_LinkedList schema = ColumnSchema_linked_list_new();
    ColumnSchema_linked_list_push_back(&schema, column_schema_new("data", ROW_SLICE));
    ColumnSchema_linked_list_push_back(&schema, column_schema_new("next_slot_ptr", SECONDARY_PTR));
    return schema;
}



Maybe_Cell cell_extract(ColumnType type, RawSlot *slot)
{
    Cell cell = (Cell) { .type = type };
    switch (type)
    {
        case PRIMARY_PTR:
            if (slot->size - slot->offset < (uint16_t) sizeof(PrimaryPtrCell)) EXCEPTION(Cell, "small slot size");
            cell.primary_ptr = *((PrimaryPtrCell *) (slot->value + slot->offset));
            slot->offset += (uint16_t) sizeof(PrimaryPtrCell);
            break;
        case SECONDARY_PTR:
            if (slot->size - slot->offset < (uint16_t) sizeof(SecondaryPtrCell)) EXCEPTION(Cell, "small slot size");
            cell.secondary_ptr = *((SecondaryPtrCell *) (slot->value + slot->offset));
            slot->offset += sizeof(SecondaryPtrCell);
            break;
        case BOOL:
            if (slot->size - slot->offset < (uint16_t) sizeof(BoolCell)) EXCEPTION(Cell, "small slot size");
            cell.bool = *((BoolCell *) (slot->value + slot->offset));
            slot->offset += (uint16_t) sizeof(BoolCell);
            break;
        case UINT_8:
            if (slot->size - slot->offset < (uint16_t) sizeof(Uint8Cell)) EXCEPTION(Cell, "small slot size");
            cell.uint8 = *((Uint8Cell *) (slot->value + slot->offset));
            slot->offset += (uint16_t) sizeof(Uint8Cell);
            break;
        case UINT_16:
            if (slot->size - slot->offset < (uint16_t) sizeof(Uint16Cell)) EXCEPTION(Cell, "small slot size");
            cell.uint16 = *((Uint16Cell *) (slot->value + slot->offset));
            slot->offset += (uint16_t) sizeof(Uint16Cell);
            break;
        case UINT_32:
            if (slot->size - slot->offset < (uint16_t) sizeof(Uint32Cell)) EXCEPTION(Cell, "small slot size");
            cell.uint32 = *((Uint32Cell *) (slot->value + slot->offset));
            slot->offset += (uint16_t) sizeof(Uint32Cell);
            break;
        case INT_32:
            if (slot->size - slot->offset < (uint16_t) sizeof(Int32Cell)) EXCEPTION(Cell, "small slot size");
            cell.int32 = *((Int32Cell *) (slot->value + slot->offset));
            slot->offset += (uint16_t) sizeof(Int32Cell);
            break;
        case FLOAT:
            if (slot->size - slot->offset < (uint16_t) sizeof(FloatCell)) EXCEPTION(Cell, "small slot size");
            cell._float = *((FloatCell *) (slot->value + slot->offset));
            slot->offset += (uint16_t) sizeof(FloatCell);
            break;
        case ROW_SLICE:
        case STRING:
        case TABLE_SCHEMA:
            if (slot->size - slot->offset < (uint16_t) sizeof(uint16_t)) EXCEPTION(Cell, "small slot size");
            cell.var_size = HANDLE(Cell, VariableSizeCell, variable_size_cell_new(*((uint16_t *) (slot->value + slot->offset))));
            slot->offset += (uint16_t) sizeof(uint16_t);
            if (slot->size - slot->offset < cell.var_size.size) EXCEPTION(Cell, "small slot size");
            memcpy(cell.var_size.payload, slot->value + slot->offset, cell.var_size.size);
            slot->offset += cell.var_size.size;
            break;
    }
    RESULT(Cell, cell);
}

Maybe_Nothing cell_pack(Cell *cell, RawSlot *slot)
{
    switch (cell->type)
    {
        case PRIMARY_PTR:
            if (slot->size - slot->offset < (uint16_t) sizeof(PrimaryPtrCell)) EXCEPTION(Nothing, "small slot size");
            *((PrimaryPtrCell *) (slot->value + slot->offset)) = cell->primary_ptr;
            slot->offset += (uint16_t) sizeof(PrimaryPtrCell);
            break;
        case SECONDARY_PTR:
            if (slot->size - slot->offset < (uint16_t) sizeof(SecondaryPtrCell)) EXCEPTION(Nothing, "small slot size");
            *((SecondaryPtrCell *) (slot->value + slot->offset)) = cell->secondary_ptr;
            slot->offset += (uint16_t) sizeof(SecondaryPtrCell);
            break;
        case BOOL:
            if (slot->size - slot->offset < (uint16_t) sizeof(BoolCell)) EXCEPTION(Nothing, "small slot size");
            *((BoolCell *) (slot->value + slot->offset)) = cell->bool;
            slot->offset += (uint16_t) sizeof(BoolCell);
            break;
        case UINT_8:
            if (slot->size - slot->offset < (uint16_t) sizeof(Uint8Cell)) EXCEPTION(Nothing, "small slot size");
            *((Uint8Cell *) (slot->value + slot->offset)) = cell->uint8;
            slot->offset += (uint16_t) sizeof(Uint8Cell);
            break;
        case UINT_16:
            if (slot->size - slot->offset < (uint16_t) sizeof(Uint16Cell)) EXCEPTION(Nothing, "small slot size");
            *((Uint16Cell *) (slot->value + slot->offset)) = cell->uint16;
            slot->offset += (uint16_t) sizeof(Uint16Cell);
            break;
        case UINT_32:
            if (slot->size - slot->offset < (uint16_t) sizeof(Uint32Cell)) EXCEPTION(Nothing, "small slot size");
            *((Uint32Cell *) (slot->value + slot->offset)) = cell->uint32;
            slot->offset += (uint16_t) sizeof(Uint32Cell);
            break;
        case INT_32:
            if (slot->size - slot->offset < (uint16_t) sizeof(Int32Cell)) EXCEPTION(Nothing, "small slot size");
            *((Int32Cell *) (slot->value + slot->offset)) = cell->int32;
            slot->offset += (uint16_t) sizeof(Int32Cell);
            break;
        case FLOAT:
            if (slot->size - slot->offset < (uint16_t) sizeof(FloatCell)) EXCEPTION(Nothing, "small slot size");
            *((FloatCell *) (slot->value + slot->offset)) = cell->_float;
            slot->offset += (uint16_t) sizeof(FloatCell);
            break;
        case ROW_SLICE:
        case STRING:
        case TABLE_SCHEMA:
            if (slot->size - slot->offset < (uint16_t) sizeof(uint16_t)) EXCEPTION(Nothing, "small slot size");
            memcpy(slot->value + slot->offset, &cell->var_size.size, sizeof(uint16_t));
            slot->offset += (uint16_t) sizeof(uint16_t);
            if (slot->size - slot->offset < cell->var_size.size) EXCEPTION(Nothing, "small slot size");
            memcpy(slot->value + slot->offset, cell->var_size.payload, cell->var_size.size);
            slot->offset += cell->var_size.size;
            break;
    }
    OK(Nothing);
}


Maybe_Cell cell_clone(Cell *cell)
{
    CellValue cell_value = HANDLE(Cell, CellValue, cell_get_value(cell));
    Cell new_cell = HANDLE(Cell, Cell, cell_from_value(&cell_value));
    cell_value_free(&cell_value);
    RESULT(Cell, new_cell);
}


Maybe_CellValue cell_get_value(Cell *cell)
{
    CellValue value;
    value.type = cell->type;
    switch (cell->type)
    {
        case BOOL:
            value.bool = cell->bool.payload;
            break;
        case UINT_8:
            value.uint8 = cell->uint8.payload;
            break;
        case UINT_16:
            value.uint16 = cell->uint16.payload;
            break;
        case UINT_32:
            value.uint32 = cell->uint32.payload;
            break;
        case INT_32:
            value.int32 = cell->int32.payload;
            break;
        case FLOAT:
            value._float = cell->_float.payload;
            break;
        case STRING:
            if (cell->var_size.payload == NULL) EXCEPTION(CellValue, "null ptr");
            value.string = malloc(cell->var_size.size);
            memcpy(value.string, cell->var_size.payload, cell->var_size.size);
            break;
        case TABLE_SCHEMA:
            if (cell->var_size.payload == NULL) EXCEPTION(CellValue, "null ptr");
            value.table_schema = ColumnSchema_linked_list_new();
            uint8_t shift = 0;
            while (shift < cell->var_size.size)
            {
                ColumnSchema column_schema;
                uint16_t column_schema_size = *((uint16_t *) (cell->var_size.payload + shift));
                shift += sizeof(uint16_t);
                column_schema.name = malloc(column_schema_size - sizeof(ColumnType));
                memcpy(column_schema.name, cell->var_size.payload + shift, column_schema_size - sizeof(ColumnType));
                shift += column_schema_size - sizeof(ColumnType);
                column_schema.type = *((ColumnType *) (cell->var_size.payload + shift));
                shift += sizeof(ColumnType);
                ColumnSchema_linked_list_push_back(&value.table_schema, column_schema);
            }
            break;
        default:
            EXCEPTION(CellValue, "unsupported value type");
    }
    RESULT(CellValue, value);
}




Maybe_Cell cell_from_value(CellValue *value)
{
    Cell cell;
    cell.type = value->type;
    switch (value->type)
    {
        case BOOL:
            cell.bool.payload = value->bool;
            break;
        case UINT_8:
            cell.uint8.payload = value->uint8;
            break;
        case UINT_16:
            cell.uint16.payload = value->uint16;
            break;
        case UINT_32:
            cell.uint32.payload = value->uint32;
            break;
        case INT_32:
            cell.int32.payload = value->int32;
            break;
        case FLOAT:
            cell._float.payload = value->_float;
            break;
        case STRING:
            if (value->string == NULL) EXCEPTION(Cell, "null ptr");
            cell.var_size = HANDLE(Cell, VariableSizeCell, variable_size_cell_new(strlen(value->string) + 1));
            memcpy(cell.var_size.payload, value->string, strlen(value->string) + 1);
            break;
        case TABLE_SCHEMA:
        {
            uint16_t table_schema_size = 0;
            ColumnSchema_LinkedListNode *it = value->table_schema.head;
            while (it != NULL)
            {
                table_schema_size += sizeof(uint16_t) + strlen(it->value.name) + 1 + sizeof(ColumnType);
                it = it->next;
            }
            cell.var_size = HANDLE(Cell, VariableSizeCell, variable_size_cell_new(table_schema_size));

            uint16_t shift = 0;
            it = value->table_schema.head;
            while (it != NULL)
            {
                *((uint16_t *) (cell.var_size.payload + shift)) = strlen(it->value.name) + 1 + sizeof(ColumnType);
                shift += sizeof(uint16_t);
                memcpy(cell.var_size.payload + shift, it->value.name, strlen(it->value.name) + 1);
                shift += strlen(it->value.name) + 1;
                *((ColumnType *) (cell.var_size.payload + shift)) = it->value.type;
                shift += sizeof(ColumnType);
                it = it->next;
            }
            break;
        }
        default:
            EXCEPTION(Cell, "unsupported value type");
    }
    RESULT(Cell, cell);
}

Maybe_CellValue cell_value_new(ColumnType type, void *value)
{
    if (value == NULL) EXCEPTION(CellValue, "null ptr");
    CellValue cell_value;
    cell_value.type = type;
    switch (type)
    {
        case BOOL:
            cell_value.bool = *((uint8_t*) value);
            break;
        case UINT_8:
            cell_value.uint8 = *((uint8_t*) value);
            break;
        case UINT_16:
            cell_value.uint16 = *((uint16_t*) value);
            break;
        case UINT_32:
            cell_value.uint32 = *((uint32_t*) value);
            break;
        case INT_32:
            cell_value.int32 = *((int32_t*) value);
            break;
        case FLOAT:
            cell_value._float = *((float*) value);
            break;
        case STRING:
            cell_value.string = *((char**) value);
            break;
        case TABLE_SCHEMA:
            cell_value.table_schema = *((ColumnSchema_LinkedList*) value);
            break;
        default:
            EXCEPTION(CellValue, "unsupported value type");
    }
    RESULT(CellValue, cell_value);
}

int32_t cell_value_to_int32_t(CellValue *value)
{
    switch (value->type)
    {
        case BOOL:
            return value->bool;
        case UINT_8:
            return value->uint8;
        case UINT_16:
            return value->uint16;
        case UINT_32:
            return value->uint32;
        case INT_32:
            return value->int32;
        default:
            return 0;
    }
}

Maybe_Nothing cell_value_print(CellValue *value)
{
    if (value == NULL) EXCEPTION(Nothing, "null ptr");
    switch (value->type)
    {
        case BOOL:
            if (value->bool) printf("true");
            else printf("false");
            break;
        case UINT_8:
            printf("%"PRIu8, value->uint8);
            break;
        case UINT_16:
            printf("%"PRIu16, value->uint16);
            break;
        case UINT_32:
            printf("%"PRIu32, value->uint32);
            break;
        case INT_32:
            printf("%"PRIi32, value->int32);
            break;
        case FLOAT:
            printf("%f", value->_float);
            break;
        case STRING:
            printf("%s", value->string);
            break;
        case TABLE_SCHEMA:
        {
            ColumnSchema_LinkedListNode *it = value->table_schema.head;
            while (it != NULL)
            {
                printf("%s (%s)", it->value.name, column_type_get_name(it->value.type));
                if (it->next != NULL) printf("->");
                it=it->next;
            }
            break;
        }
        default:
            EXCEPTION(Nothing, "unsupported value type");
    }
    OK(Nothing);
}

void cell_value_free(CellValue *value)
{
    switch (value->type)
    {
        case STRING:
            free(value->string);
            break;
        case TABLE_SCHEMA:
        {
            ColumnSchema_LinkedListNode *it = value->table_schema.head;
            while (it != NULL)
            {
                free(it->value.name);
                it = it->next;
            }
            table_schema_free(&value->table_schema);
            break;
        }
        default:
            break;
    }
}

char * full_column_name_new(char *table_name, char *column_name)
{
    char *ret = malloc(strlen(table_name) + strlen(column_name) + 2);
    uint32_t off = 0;
    memcpy(ret + off, table_name, strlen(table_name));
    off += strlen(table_name);
    ret[off] = '.';
    off++;
    memcpy(ret + off, column_name, strlen(column_name));
    off += strlen(column_name);
    ret[off] = 0;
    off++;
    return ret;
}

void full_column_name_free(char *full_name)
{
    if (full_name == 0)
        return;
    free(full_name);
}