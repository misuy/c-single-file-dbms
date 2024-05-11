#include "table.h"

void state_remove_table_from_sync(State *state, uint16_t id);
void state_add_table_to_sync(State *state, uint16_t id);
Table * root_table_open(State *state);
Table * free_table_new(State* state);
Table * pool_table_new(State* state, uint16_t id);
Maybe_TablePtr table_from_root_table_row(Row *row);
Maybe_Row table_to_root_table_row(State *state, Table *table);
Maybe_TablePtr root_table_get_table_by_id(State *state, uint16_t table_id);
Maybe_Nothing root_table_update_table(State *state, Table *table);
Maybe_uint16_t select_pool_table(State* state, uint16_t size);
Maybe_Nothing table_join_page(State* state, uint16_t table_id, uint32_t page_id);
Maybe_Nothing table_join_full_page(State* state, uint16_t table_id, uint32_t page_id);
Maybe_Nothing table_borrow_page(State* state, uint16_t table_id, uint32_t page_id);
Maybe_Nothing row_slice_read(State *state, RawSlot *acc, SlotPtr init_ptr);
Maybe_SlotPtr row_slice_write(State* state, RawSlot* acc);
Maybe_Nothing row_slice_delete(State *state, SlotPtr ptr);
Maybe_Nothing row_iterator_switch_to_full_pages(State *state, RowIterator *it);


LINKED_LIST(Cell)
LINKED_LIST(ColumnSchema)
LINKED_LIST(uint16_t)


Maybe_Nothing system_tables_open(State *);


Maybe_uint16_t get_id_for_new_table(State *state)
{
    state->file->header->tables_count++;
    HANDLE_NOTHING(uint16_t, file_write_header(state->file))
    RESULT(uint16_t, state->file->header->tables_count - 1);
}


// state
Maybe_State state_init(char *file_name)
{
    State state;
    state.to_sync = uint16_t_linked_list_new();
    state.is_syncing = 0;
    state.file = malloc(sizeof(File));
    *(state.file) = HANDLE(State, File, file_open(file_name));
    for (uint32_t i=0; i<MAX_TABLES_COUNT; i++)
    {
        state.tables[i] = NULL;
    }
    HANDLE_NOTHING(State, system_tables_open(&state))
    RESULT(State, state);
}

void state_remove_table_from_sync(State *state, uint16_t id)
{
    uint16_t_LinkedListNode *it = state->to_sync.head;
    uint32_t idx = 0;
    while (it != 0)
    {
        if (it->value == id)
        {
            uint16_t_linked_list_remove(&state->to_sync, idx);
            return;
        }
        idx++;
        it = it->next;
    }
}

void state_add_table_to_sync(State *state, uint16_t id)
{
    uint16_t_LinkedListNode *it = state->to_sync.head;
    while (it != 0)
    {
        if (it->value == id) return;
        it = it->next;
    }
    //printf("add to sync %"PRIu16"\n", id);
    uint16_t_linked_list_push_back(&state->to_sync, id);
}

Maybe_Nothing state_sync_tables(State *state)
{
    state->is_syncing = 1;
    while (!uint16_t_linked_list_is_empty(&state->to_sync))
    {
        uint16_t table_id = HANDLE(Nothing, uint16_t, uint16_t_linked_list_pop_front(&state->to_sync));
        Table *table = HANDLE(Nothing, TablePtr, table_open_by_id(state, table_id));
        uint32_t old_first_page_id = table->first_page_id;
        uint32_t old_first_full_page_id = table->first_full_page_id;
        //printf("sync %"PRIu16"\n", table_id);
        HANDLE_NOTHING(Nothing, table_update(state, table_id))
        if ((old_first_page_id == table->first_page_id) & (old_first_full_page_id == table->first_full_page_id)) state_remove_table_from_sync(state, table_id);
    }
    state->is_syncing = 0;
    OK(Nothing);
}

// tables

Table * table_new(uint16_t id, char *name, uint8_t is_system, uint8_t is_data, ColumnSchema_LinkedList schema, uint16_t row_size, uint32_t first_page_id, uint32_t first_full_page_id)
{
    Table *table = malloc(sizeof(Table));
    *table = (Table) { .id = id, .name = name, .is_system = is_system, .is_data = is_data, .schema = schema, .row_size = row_size, .first_page_id = first_page_id, .first_full_page_id = first_full_page_id };
    return table;
}

void table_free(State *state, uint16_t id)
{
    if (state->tables[id] != NULL)
        free(state->tables[id]);
    state->tables[id] = NULL;
}

// root table
Table * root_table_open(State *state)
{
    ColumnSchema_LinkedList schema = root_table_schema();
    return table_new(state->file->header->root_table_id, ROOT_TABLE_NAME, 1, 0, schema, column_schema_get_size(&schema), state->file->header->root_table_first_page_id, state->file->header->root_table_first_full_page_id);
}

// free table
Table * free_table_new(State* state)
{
    return table_new(state->file->header->free_table_id, FREE_TABLE_NAME, 1, 0, free_table_schema(), state->file->header->page_size - sizeof(PageHeader) - 1, 0, 0);
}

// pool table
Table * pool_table_new(State* state, uint16_t id)
{
    return table_new(id, POOL_TABLE_NAME, 1, 1, pool_table_schema(), POOL_TABLE_ROW_SIZE((id - state->file->header->first_pool_table_id)), 0, 0);
}

Maybe_TablePtr table_from_root_table_row(Row *row)
{
    char *full_name;

    full_name = full_column_name_new(ROOT_TABLE_NAME, "id");
    Cell id_cell = HANDLE(TablePtr, Cell, row_get_cell_by_name(row, full_name));
    uint16_t id = HANDLE(TablePtr, CellValue, cell_get_value(&id_cell)).uint16;
    full_column_name_free(full_name);

    full_name = full_column_name_new(ROOT_TABLE_NAME, "name");
    Cell name_cell = HANDLE(TablePtr, Cell, row_get_cell_by_name(row, full_name));
    char *name = HANDLE(TablePtr, CellValue, cell_get_value(&name_cell)).string;
    full_column_name_free(full_name);
    
    full_name = full_column_name_new(ROOT_TABLE_NAME, "is_system");
    Cell is_system_cell = HANDLE(TablePtr, Cell, row_get_cell_by_name(row, full_name));
    uint8_t is_system = HANDLE(TablePtr, CellValue, cell_get_value(&is_system_cell)).bool;
    full_column_name_free(full_name);
    
    full_name = full_column_name_new(ROOT_TABLE_NAME, "is_data");
    Cell is_data_cell = HANDLE(TablePtr, Cell, row_get_cell_by_name(row, full_name));
    uint8_t is_data = HANDLE(TablePtr, CellValue, cell_get_value(&is_data_cell)).bool;
    full_column_name_free(full_name);
    
    full_name = full_column_name_new(ROOT_TABLE_NAME, "schema");
    Cell schema_cell = HANDLE(TablePtr, Cell, row_get_cell_by_name(row, full_name));
    ColumnSchema_LinkedList schema = HANDLE(TablePtr, CellValue, cell_get_value(&schema_cell)).table_schema;
    full_column_name_free(full_name);
    
    full_name = full_column_name_new(ROOT_TABLE_NAME, "row_size");
    Cell row_size_cell = HANDLE(TablePtr, Cell, row_get_cell_by_name(row, full_name));
    uint16_t row_size = HANDLE(TablePtr, CellValue, cell_get_value(&row_size_cell)).uint16;
    full_column_name_free(full_name);
    
    full_name = full_column_name_new(ROOT_TABLE_NAME, "first_page_id");
    Cell first_page_id_cell = HANDLE(TablePtr, Cell, row_get_cell_by_name(row, full_name));
    uint32_t first_page_id = HANDLE(TablePtr, CellValue, cell_get_value(&first_page_id_cell)).uint32;
    full_column_name_free(full_name);
    
    full_name = full_column_name_new(ROOT_TABLE_NAME, "first_full_page_id");
    Cell first_full_page_id_cell = HANDLE(TablePtr, Cell, row_get_cell_by_name(row, full_name));
    uint32_t first_full_page_id = HANDLE(TablePtr, CellValue, cell_get_value(&first_full_page_id_cell)).uint32;
    full_column_name_free(full_name);
    
    RESULT(TablePtr, table_new(id, name, is_system, is_data, schema, row_size, first_page_id, first_full_page_id));
}

Maybe_Row table_to_root_table_row(State *state, Table *table)
{
    Table *root_table = HANDLE(Row, TablePtr, table_open_by_id(state, state->file->header->root_table_id));
    Cell_LinkedList cells = Cell_linked_list_new();
    CellValue id_cell_value = HANDLE(Row, CellValue, cell_value_new(UINT_16, &(table->id)));
    Cell_linked_list_push_back(&cells, HANDLE(Row, Cell, cell_from_value(&id_cell_value)));
    CellValue name_cell_value = HANDLE(Row, CellValue, cell_value_new(STRING, &(table->name)));
    Cell_linked_list_push_back(&cells, HANDLE(Row, Cell, cell_from_value(&name_cell_value)));
    CellValue is_system_cell_value = HANDLE(Row, CellValue, cell_value_new(BOOL, &(table->is_system)));
    Cell_linked_list_push_back(&cells, HANDLE(Row, Cell, cell_from_value(&is_system_cell_value)));
    CellValue is_data_cell_value = HANDLE(Row, CellValue, cell_value_new(BOOL, &(table->is_data)));
    Cell_linked_list_push_back(&cells, HANDLE(Row, Cell, cell_from_value(&is_data_cell_value)));
    CellValue schema_cell_value = HANDLE(Row, CellValue, cell_value_new(TABLE_SCHEMA, &(table->schema)));
    Cell_linked_list_push_back(&cells, HANDLE(Row, Cell, cell_from_value(&schema_cell_value)));
    CellValue row_size_cell_value = HANDLE(Row, CellValue, cell_value_new(UINT_16, &(table->row_size)));
    Cell_linked_list_push_back(&cells, HANDLE(Row, Cell, cell_from_value(&row_size_cell_value)));
    CellValue first_page_id_cell_value = HANDLE(Row, CellValue, cell_value_new(UINT_32, &(table->first_page_id)));
    Cell_linked_list_push_back(&cells, HANDLE(Row, Cell, cell_from_value(&first_page_id_cell_value)));
    CellValue first_full_page_id_cell_value = HANDLE(Row, CellValue, cell_value_new(UINT_32, &(table->first_full_page_id)));
    Cell_linked_list_push_back(&cells, HANDLE(Row, Cell, cell_from_value(&first_full_page_id_cell_value)));
    Row row = row_new(root_table->schema);
    HANDLE_NOTHING(Row, row_set_cells(&row, cells));
    RESULT(Row, row);
}

Maybe_TablePtr root_table_get_table_by_id(State *state, uint16_t table_id)
{
    char *full_name = full_column_name_new(ROOT_TABLE_NAME, "id");
    Predicate *predicate = predicate_new(operator_equal, operand_new_by_ptr(full_name), operand_new_by_value((CellValue) { .type = UINT_16, .uint16 = table_id }));
    RowIterator it = HANDLE(TablePtr, RowIterator, row_iterator_new(state, state->file->header->root_table_id));
    while (!it.is_null)
    {
        if (HANDLE(TablePtr, uint8_t, predicate_apply(&it.row, predicate)))
        {
            Table *table = HANDLE(TablePtr, TablePtr, table_from_root_table_row(&it.row));
            row_iterator_free(&it);
            RESULT(TablePtr, table);
        }
        row_iterator_next(state, &it);
    }
    full_column_name_free(full_name);
    row_iterator_free(&it);
    RESULT(TablePtr, NULL);
}

Maybe_TablePtr root_table_get_table_by_name(State *state, char *name)
{
    char *full_name = full_column_name_new(ROOT_TABLE_NAME, "name");
    Predicate *predicate = predicate_new(operator_equal, operand_new_by_ptr(full_name), operand_new_by_value((CellValue) { .type = STRING, .string = name }));
    RowIterator it = HANDLE(TablePtr, RowIterator, row_iterator_new(state, state->file->header->root_table_id));
    while (!it.is_null)
    {
        if (HANDLE(TablePtr, uint8_t, predicate_apply(&it.row, predicate)))
        {
            Table *table = HANDLE(TablePtr, TablePtr, table_from_root_table_row(&it.row));
            row_iterator_free(&it);
            RESULT(TablePtr, table);
        }
        row_iterator_next(state, &it);
    }
    //predicate_free(&predicate);
    full_column_name_free(full_name);
    row_iterator_free(&it);
    RESULT(TablePtr, NULL);
}

Maybe_Nothing root_table_update_table(State *state, Table *table)
{
    char *full_name = full_column_name_new(ROOT_TABLE_NAME, "id");
    Row table_as_row = HANDLE(Nothing, Row, table_to_root_table_row(state, table));
    Predicate *predicate = predicate_new(operator_equal, operand_new_by_ptr(full_name), operand_new_by_value((CellValue) { .type = UINT_16, .uint16 = table->id }));
    RowIterator it = HANDLE(Nothing, RowIterator, row_iterator_new(state, state->file->header->root_table_id));
    while (!it.is_null)
    {
        if (HANDLE(Nothing, uint8_t, predicate_apply(&it.row, predicate)))
        {
            HANDLE_NOTHING(Nothing, table_delete_row(state, it.slot_iterator.ptr));
            break;
        }
        HANDLE_NOTHING(Nothing, row_iterator_next(state, &it));
    }
    full_column_name_free(full_name);
    row_iterator_free(&it);
    //predicate_free(&predicate);
    SlotPtr ptr = HANDLE(Nothing, SlotPtr, table_find_free_slot(state, state->file->header->root_table_id));
    HANDLE_NOTHING(Nothing, table_write_row(state, &table_as_row, ptr));
    row_free(&table_as_row);
    OK(Nothing);
}

Maybe_Nothing root_table_delete_table(State *state, Table *table)
{
    char *full_name = full_column_name_new(ROOT_TABLE_NAME, "id");
    Predicate *predicate = predicate_new(operator_equal, operand_new_by_ptr(full_name), operand_new_by_value((CellValue) { .type = UINT_16, .uint16 = table->id }));
    RowIterator it = HANDLE(Nothing, RowIterator, row_iterator_new(state, state->file->header->root_table_id));
    while (!it.is_null)
    {
        if (HANDLE(Nothing, uint8_t, predicate_apply(&it.row, predicate)))
        {
            HANDLE_NOTHING(Nothing, table_delete_row(state, it.slot_iterator.ptr));
            break;
        }
        HANDLE_NOTHING(Nothing, row_iterator_next(state, &it));
    }
    full_column_name_free(full_name);
    row_iterator_free(&it);
    OK(Nothing);
}

/*
 * open table
 * state->tables stores all tables
 * if table was opened then just return it from state->tables
 * else load table and save into state->tables
*/
Maybe_TablePtr table_open_by_id(State* state, uint16_t id)
{
    if (state->tables[id] == NULL)
    {
        state->tables[id] = HANDLE(TablePtr, TablePtr, root_table_get_table_by_id(state, id));
    }
    RESULT(TablePtr, state->tables[id]);
}


Maybe_Nothing table_update(State *state, uint16_t id)
{
    Table *table = HANDLE(Nothing, TablePtr, table_open_by_id(state, id));
    if (id == state->file->header->root_table_id)
    {
        state->file->header->root_table_first_page_id = table->first_page_id;
        state->file->header->root_table_first_full_page_id = table->first_full_page_id;
        HANDLE_NOTHING(Nothing, file_write_header(state->file));
    }
    else HANDLE_NOTHING(Nothing, root_table_update_table(state, table));
    OK(Nothing);
}


Maybe_Nothing system_tables_open(State *state)
{
    state->tables[state->file->header->root_table_id] = root_table_open(state);

    state->tables[state->file->header->free_table_id] = HANDLE(Nothing, TablePtr, table_open_by_id(state,
                                                                                                   state->file->header->free_table_id));
    if (state->tables[state->file->header->free_table_id] == NULL)
    {
        state->tables[state->file->header->free_table_id] = free_table_new(state);
        state_add_table_to_sync(state, state->file->header->free_table_id);
    }

    for (uint16_t i=state->file->header->first_pool_table_id; i<state->file->header->first_pool_table_id+state->file->header->pool_tables_count; i++)
    {
        state->tables[i] = HANDLE(Nothing, TablePtr, table_open_by_id(state, i));
        if (state->tables[i] == NULL)
        {
            state->tables[i] = pool_table_new(state, i);
            state_add_table_to_sync(state, i);
        }
    }

    HANDLE_NOTHING(Nothing, state_sync_tables(state));
    OK(Nothing);
}





// return the most fitted pool table id to store row slice with size size
Maybe_uint16_t select_pool_table(State* state, uint16_t size)
{
    Table* table;
    for (uint16_t i=state->file->header->first_pool_table_id; i<state->file->header->first_pool_table_id+state->file->header->pool_tables_count; i++)
    {
        table = HANDLE(uint16_t, TablePtr, table_open_by_id(state, i));
        if (table == NULL) EXCEPTION(uint16_t, "null ptr");
        if (table->row_size == size) RESULT(uint16_t, i);
        if (table->row_size < size)
        {
            if (i == state->file->header->first_pool_table_id) RESULT(uint16_t, i);
            table = HANDLE(uint16_t, TablePtr, table_open_by_id(state, i - 1));
            if (table == NULL) EXCEPTION(uint16_t, "null ptr");
            if (((float) size / (float) table->row_size) >= state->file->header->data_density) RESULT(uint16_t, i-1);
            RESULT(uint16_t, i);
        }
    }
    RESULT(uint16_t, state->file->header->first_pool_table_id + state->file->header->pool_tables_count - 1);
}



// pages ownership
// join page(page_id) to tables(table_id) not full pages
Maybe_Nothing table_join_page(State* state, uint16_t table_id, uint32_t page_id)
{
    //printf("table join page %"PRIu16"\n", table_id);
    Table* table = HANDLE(Nothing, TablePtr, table_open_by_id(state, table_id));
    if (table == NULL) EXCEPTION(Nothing, "table not exists");
    Page page = HANDLE(Nothing, Page, page_open(state->file, page_id));
    page.header->next_page_id = table->first_page_id;
    page.header->prev_page_id = 0;
    if (table->first_page_id != 0)
    {
        Page first_page = HANDLE(Nothing, Page, page_open(state->file, table->first_page_id));
        first_page.header->prev_page_id = page.header->id;
        HANDLE_NOTHING(Nothing, page_write(state->file, &first_page));
        page_close(&first_page);
    }
    HANDLE_NOTHING(Nothing, page_write(state->file, &page));
    table->first_page_id = page.header->id;
    page_close(&page);
    state_add_table_to_sync(state, table_id);
    OK(Nothing);
}

// join page(page_id) to tables(table_id) full pages
Maybe_Nothing table_join_full_page(State* state, uint16_t table_id, uint32_t page_id)
{
    //printf("table join full page %"PRIu16"\n", table_id);
    Table* table = HANDLE(Nothing, TablePtr, table_open_by_id(state, table_id));
    if (table == NULL) EXCEPTION(Nothing, "table not exists");
    Page page = HANDLE(Nothing, Page, page_open(state->file, page_id));
    page.header->next_page_id = table->first_full_page_id;
    page.header->prev_page_id = 0;
    if (table->first_full_page_id != 0)
    {
        Page first_full_page = HANDLE(Nothing, Page, page_open(state->file, table->first_full_page_id));
        first_full_page.header->prev_page_id = page.header->id;
        HANDLE_NOTHING(Nothing, page_write(state->file, &first_full_page));
        page_close(&first_full_page);
    }
    HANDLE_NOTHING(Nothing, page_write(state->file, &page));
    table->first_full_page_id = page.header->id;
    page_close(&page);
    state_add_table_to_sync(state, table_id);
    OK(Nothing);
}

// borrow page(page_id) from table(table_id)
Maybe_Nothing table_borrow_page(State* state, uint16_t table_id, uint32_t page_id)
{
    //printf("table borrow page %"PRIu16"\n", table_id);
    Table* table = HANDLE(Nothing, TablePtr, table_open_by_id(state, table_id));
    if (table == NULL) EXCEPTION(Nothing, "table not exists");
    Page page = HANDLE(Nothing, Page, page_open(state->file, page_id));
    uint32_t prev_page_id = page.header->prev_page_id;
    uint32_t next_page_id = page.header->next_page_id;
    if (prev_page_id != 0)
    {
        Page prev_page = HANDLE(Nothing, Page, page_open(state->file, prev_page_id));
        prev_page.header->next_page_id = next_page_id;
        HANDLE_NOTHING(Nothing, page_write(state->file, &prev_page));
        page_close(&prev_page);
    }
    if (next_page_id != 0)
    {
        Page next_page = HANDLE(Nothing, Page, page_open(state->file, next_page_id));
        next_page.header->prev_page_id = prev_page_id;
        HANDLE_NOTHING(Nothing, page_write(state->file, &next_page));
        page_close(&next_page);
    }
    if (table->first_page_id == page_id) table->first_page_id = next_page_id;
    if (table->first_full_page_id == page_id) table->first_full_page_id = next_page_id;
    page_close(&page);
    state_add_table_to_sync(state, table_id);
    OK(Nothing);
}



// return slot_ptr to free slot at table(table_id)
Maybe_SlotPtr table_find_free_slot(State *state, uint16_t table_id)
{
    Table* table = HANDLE(SlotPtr, TablePtr, table_open_by_id(state, table_id));
    if (table == NULL) EXCEPTION(SlotPtr, "null ptr");
    if (table->first_page_id == 0)
    {
        Table* free_table = HANDLE(SlotPtr, TablePtr, table_open_by_id(state, state->file->header->free_table_id));
        if (table == NULL) EXCEPTION(SlotPtr, "null ptr");
        if (free_table->first_page_id != 0)
        {
            uint32_t page_id = free_table->first_page_id;
            HANDLE_NOTHING(SlotPtr, table_borrow_page(state, state->file->header->free_table_id, page_id));
            Page page = HANDLE(SlotPtr, Page, page_open(state->file, page_id));
            page_resize(state->file, &page, table->row_size);
            HANDLE_NOTHING(SlotPtr, page_write(state->file, &page));
            page_close(&page);
            HANDLE_NOTHING(SlotPtr, table_join_page(state, table_id, page_id));
        }
        else
        {
            Page page = HANDLE(SlotPtr, Page, page_new(state->file, table->row_size));
            uint32_t page_id = page.header->id;
            HANDLE_NOTHING(SlotPtr, page_write(state->file, &page));
            page_close(&page);
            HANDLE_NOTHING(SlotPtr, table_join_page(state, table_id, page_id));
        }
    }

    SlotIterator it = HANDLE(SlotPtr, SlotIterator, slot_iterator_new(state->file, (SlotPtr) { .table_id = table_id, .page_id = table->first_page_id, .slot_id = 0 }));
    while (!it.is_null)
    {
        if (HANDLE(SlotPtr, uint8_t, page_is_slot_free(it.page_it.page, it.ptr.slot_id)))
        {
            slot_iterator_free(&it);
            RESULT(SlotPtr, it.ptr);
        }
        HANDLE_NOTHING(SlotPtr, slot_iterator_next(state->file, &it));
    }
    EXCEPTION(SlotPtr, "idk");
}



// slot reading
// recursive function to read a row, splitted on row slices(ptr) stored in pool tables, to acc
Maybe_Nothing row_slice_read(State *state, RawSlot *acc, SlotPtr init_ptr)
{
    SlotPtr ptr, next_slot_ptr;
    Page page;
    RawSlot slot;
    Cell row_slice;

    SlotPtr_LinkedList stack = SlotPtr_linked_list_new();
    SlotPtr_linked_list_push_front(&stack, init_ptr);

    while (!SlotPtr_linked_list_is_empty(&stack))
    {
        ptr = HANDLE(Nothing, SlotPtr, SlotPtr_linked_list_pop_front(&stack));

        page = HANDLE(Nothing, Page, page_open(state->file, ptr.page_id));
        slot = HANDLE(Nothing, RawSlot, page_pull_raw_slot(&page, ptr.slot_id));
        row_slice = HANDLE(Nothing, Cell, cell_extract(ROW_SLICE, &slot));

        if (acc->size - acc->offset < row_slice.var_size.size) EXCEPTION(Nothing, "small slot size");
        memcpy(acc->value + acc->offset, row_slice.var_size.payload, row_slice.var_size.size);
        acc->offset += row_slice.var_size.size;
        variable_size_cell_free(&row_slice.var_size);
        next_slot_ptr = HANDLE(Nothing, Cell, cell_extract(SECONDARY_PTR, &slot)).secondary_ptr.payload;
        page_close(&page);

        if (!is_slot_ptr_null(next_slot_ptr)) SlotPtr_linked_list_push_front(&stack, next_slot_ptr);
    }
    OK(Nothing);
}

// read row(ptr) from table
Maybe_Row table_read_row(State *state, SlotPtr ptr)
{
    Table* table = HANDLE(Row, TablePtr, table_open_by_id(state, ptr.table_id));
    if (table == NULL) EXCEPTION(Row, "table not exists");
    Page page = HANDLE(Row, Page, page_open(state->file, ptr.page_id));
    RawSlot slot = HANDLE(Row, RawSlot, page_pull_raw_slot(&page, ptr.slot_id));
    if (!table->is_data)
    {
        Cell primary_ptr = HANDLE(Row, Cell, cell_extract(PRIMARY_PTR, &slot));
        RawSlot acc = HANDLE(Row, RawSlot, raw_slot_new(primary_ptr.primary_ptr.size));
        HANDLE_NOTHING(Row, row_slice_read(state, &acc, primary_ptr.primary_ptr.payload));
        acc.offset = 0;
        slot = acc;
    }
    Cell_LinkedList cells = Cell_linked_list_new();
    ColumnSchema_LinkedListNode* schema_it = table->schema.head;
    while (schema_it != NULL)
    {
        Cell cell = HANDLE(Row, Cell, cell_extract(schema_it->value.type, &slot));
        Cell_linked_list_push_back(&cells, cell);
        schema_it = schema_it->next;
    }
    page_close(&page);
    if (!table->is_data) raw_slot_free(&slot);

    Row row = row_new(table->schema);
    HANDLE_NOTHING(Row, row_set_cells(&row, cells));

    RESULT(Row, row);
}



// slot writing
// recursive function to split row on row slices and store it in pool tables
Maybe_SlotPtr row_slice_write(State* state, RawSlot* acc)
{
    Table *table;
    Page page;
    Cell row_slice, secondary_ptr;
    SlotPtr next_slot_ptr = NULL_SLOT_PTR, slot_ptr;
    RawSlot slot;
    uint16_t slice_size, pool_table_id;
    while (acc->size - acc->offset > 0)
    {
        slice_size = acc->size - acc->offset + column_type_get_size(SECONDARY_PTR) + sizeof(uint16_t);
        if (state->is_syncing) pool_table_id = state->file->header->first_pool_table_id + state->file->header->pool_tables_count - 1;
        else pool_table_id = HANDLE(SlotPtr, uint16_t, select_pool_table(state, slice_size));
        table = HANDLE(SlotPtr, TablePtr, table_open_by_id(state, pool_table_id));
        if (table == NULL) EXCEPTION(SlotPtr, "null ptr");
        slice_size = MIN(acc->size - acc->offset,
                         table->row_size - column_type_get_size(SECONDARY_PTR) - (uint16_t) sizeof(uint16_t));

        row_slice.type = ROW_SLICE;
        row_slice.var_size = HANDLE(SlotPtr, VariableSizeCell, variable_size_cell_new(slice_size));
        memcpy(row_slice.var_size.payload, acc->value + acc->size - acc->offset - row_slice.var_size.size, row_slice.var_size.size);
        acc->offset += row_slice.var_size.size;

        secondary_ptr = (Cell) {.type = SECONDARY_PTR, .secondary_ptr = {next_slot_ptr}};

        slot_ptr = HANDLE(SlotPtr, SlotPtr, table_find_free_slot(state, table->id));
        next_slot_ptr = slot_ptr;
        page = HANDLE(SlotPtr, Page, page_open(state->file, slot_ptr.page_id));
        slot = HANDLE(SlotPtr, RawSlot, page_pull_raw_slot(&page, slot_ptr.slot_id));

        HANDLE_NOTHING(SlotPtr, cell_pack(&row_slice, &slot));
        variable_size_cell_free(&row_slice.var_size);
        HANDLE_NOTHING(SlotPtr, cell_pack(&secondary_ptr, &slot));
        HANDLE_NOTHING(SlotPtr, page_occupy_slot(&page, slot_ptr.slot_id));
        HANDLE_NOTHING(SlotPtr, page_write(state->file, &page));

        if (HANDLE(SlotPtr, uint8_t, page_is_full(&page)))
        {
            HANDLE_NOTHING(SlotPtr, table_borrow_page(state, table->id, slot_ptr.page_id));
            HANDLE_NOTHING(SlotPtr, table_join_full_page(state, table->id, slot_ptr.page_id));
        }

        page_close(&page);
    }

    RESULT(SlotPtr, next_slot_ptr);
}

/*
Maybe_SlotPtr row_slice_write1(State* state, RawSlot* acc)
{
    if (acc->size - acc->offset <= 0) RESULT(SlotPtr, NULL_SLOT_PTR);

    uint16_t slice_size = acc->size - acc->offset + column_type_get_size(SECONDARY_PTR) + sizeof(uint16_t);
    uint16_t pool_table_id = HANDLE(SlotPtr, uint16_t, select_pool_table(state, slice_size));
    Table *table = HANDLE(SlotPtr, TablePtr, table_open_by_id(state, pool_table_id));
    if (table == NULL) EXCEPTION(SlotPtr, "null ptr");
    slice_size = MIN(acc->size - acc->offset, table->row_size - column_type_get_size(SECONDARY_PTR) - sizeof(uint16_t));

    Cell row_slice;
    row_slice.type = ROW_SLICE;
    row_slice.var_size = HANDLE(SlotPtr, VariableSizeCell, variable_size_cell_new(slice_size));
    memcpy(row_slice.var_size.payload, acc->value + acc->offset, row_slice.var_size.size);
    acc->offset += row_slice.var_size.size;

    SlotPtr next_slot_ptr = NULL_SLOT_PTR;
    Cell secondary_ptr = (Cell) { .type = SECONDARY_PTR, .secondary_ptr = next_slot_ptr };

    SlotPtr slot_ptr = HANDLE(SlotPtr, SlotPtr, table_find_free_slot(state, table->id));
    Page page = HANDLE(SlotPtr, Page, page_open(state->file, slot_ptr.page_id));
    RawSlot slot = HANDLE(SlotPtr, RawSlot, page_pull_raw_slot(&page, slot_ptr.slot_id));

    HANDLE_NOTHING(SlotPtr, cell_pack(&row_slice, &slot));
    variable_size_cell_free(&row_slice.var_size);
    HANDLE_NOTHING(SlotPtr, cell_pack(&secondary_ptr, &slot));
    HANDLE_NOTHING(SlotPtr, page_occupy_slot(&page, slot_ptr.slot_id));
    HANDLE_NOTHING(SlotPtr, page_write(state->file, &page));

    if (HANDLE(SlotPtr, uint8_t, page_is_full(&page)))
    {
        HANDLE_NOTHING(SlotPtr, table_borrow_page(state, table->id, slot_ptr.page_id));
        HANDLE_NOTHING(SlotPtr, table_join_full_page(state, table->id, slot_ptr.page_id));
    }

    page_close(&page);

    RESULT(SlotPtr, slot_ptr);
}
*/

// write row to ptr
Maybe_Nothing table_write_row(State *state, Row *row, SlotPtr ptr)
{
    Table *table = HANDLE(Nothing, TablePtr, table_open_by_id(state, ptr.table_id));
    if (table == NULL) EXCEPTION(Nothing, "null ptr");

    Cell_LinkedList cells = row->cells;
    Page page = HANDLE(Nothing, Page, page_open(state->file, ptr.page_id));
    RawSlot slot = HANDLE(Nothing, RawSlot, page_pull_raw_slot(&page, ptr.slot_id));
    if (!table->is_data)
    {
        uint16_t row_size = 0;
        Cell_LinkedListNode *cells_it = cells.head;
        ColumnSchema_LinkedListNode *schema_it = table->schema.head;
        while (cells_it != NULL)
        {
            if (schema_it == NULL) EXCEPTION(Nothing, "illegal cells");
            if (cells_it->value.type != schema_it->value.type) EXCEPTION(Nothing, "illegal cells");
            row_size += HANDLE(Nothing, uint16_t, cell_get_size(&cells_it->value));
            cells_it = cells_it->next;
            schema_it = schema_it->next;
        }
        if (schema_it != NULL) EXCEPTION(Nothing, "illegal cells");


        RawSlot acc = HANDLE(Nothing, RawSlot, raw_slot_new(row_size));
        cells_it = cells.head;
        while (cells_it != NULL)
        {
            HANDLE_NOTHING(Nothing, cell_pack(&cells_it->value, &acc));
            cells_it = cells_it->next;
        }

        acc.offset = 0;
        SlotPtr primary_ptr = HANDLE(Nothing, SlotPtr, row_slice_write(state, &acc));
        Cell primary_ptr_cell = (Cell) { .type = PRIMARY_PTR, .primary_ptr = (PrimaryPtrCell) { .size = row_size, .payload = primary_ptr } };
        HANDLE_NOTHING(Nothing, cell_pack(&primary_ptr_cell, &slot));
        raw_slot_free(&acc);
    }
    else
    {
        Cell_LinkedListNode *cells_it = cells.head;
        ColumnSchema_LinkedListNode* schema_it = table->schema.head;
        while (cells_it != NULL)
        {
            if (schema_it == NULL) EXCEPTION(Nothing, "illegal cells");
            if (cells_it->value.type != schema_it->value.type) EXCEPTION(Nothing, "illegal cells");
            HANDLE_NOTHING(Nothing, cell_pack(&cells_it->value, &slot));
            cells_it = cells_it->next;
            schema_it = schema_it->next;
        }
        if (schema_it != NULL) EXCEPTION(Nothing, "illegal cells");
    }

    HANDLE_NOTHING(Nothing, page_occupy_slot(&page, ptr.slot_id));
    HANDLE_NOTHING(Nothing, page_write(state->file, &page));

    if (HANDLE(Nothing, uint8_t, page_is_full(&page)))
    {
        HANDLE_NOTHING(Nothing, table_borrow_page(state, table->id, ptr.page_id));
        HANDLE_NOTHING(Nothing, table_join_full_page(state, table->id, ptr.page_id));
    }

    page_close(&page);

    OK(Nothing);
}


//
Maybe_Nothing row_slice_delete(State *state, SlotPtr ptr)
{
    Table *table;
    Page page;
    RawSlot slot;
    Cell row_slice, next_ptr;

    while (!is_slot_ptr_null(ptr))
    {
        table = HANDLE(Nothing, TablePtr, table_open_by_id(state, ptr.table_id));
        if (table == NULL) EXCEPTION(Nothing, "table not exists");
        if (!(table->is_data & table->is_system)) EXCEPTION(Nothing, "not a pool table");

        page = HANDLE(Nothing, Page, page_open(state->file, ptr.page_id));
        slot = HANDLE(Nothing, RawSlot, page_pull_raw_slot(&page, ptr.slot_id));
        row_slice = HANDLE(Nothing, Cell, cell_extract(ROW_SLICE, &slot));
        variable_size_cell_free(&row_slice.var_size);
        next_ptr = HANDLE(Nothing, Cell, cell_extract(SECONDARY_PTR, &slot));
        page_close(&page);

        HANDLE_NOTHING(Nothing, table_delete_row(state, ptr));

        ptr = next_ptr.secondary_ptr.payload;
    }

    OK(Nothing);
}

//
Maybe_Nothing table_delete_row(State *state, SlotPtr ptr)
{
    Table *table = HANDLE(Nothing, TablePtr, table_open_by_id(state, ptr.table_id));
    if (table == NULL) EXCEPTION(Nothing, "table not exists");
    Page page = HANDLE(Nothing, Page, page_open(state->file, ptr.page_id));

    if (!table->is_data)
    {
        RawSlot slot = HANDLE(Nothing, RawSlot, page_pull_raw_slot(&page, ptr.slot_id));
        SlotPtr next_ptr = HANDLE(Nothing, Cell, cell_extract(PRIMARY_PTR, &slot)).primary_ptr.payload;
        HANDLE_NOTHING(Nothing, row_slice_delete(state, next_ptr));
    }

    uint8_t page_was_full = HANDLE(Nothing, uint8_t, page_is_full(&page));
    HANDLE_NOTHING(Nothing, page_free_slot(&page, ptr.slot_id));
    HANDLE_NOTHING(Nothing, page_write(state->file, &page));

    if (HANDLE(Nothing, uint8_t, page_is_empty(&page)))
    {
        HANDLE_NOTHING(Nothing, table_borrow_page(state, table->id, ptr.page_id));
        HANDLE_NOTHING(Nothing, table_join_page(state, state->file->header->free_table_id, ptr.page_id));
    }
    else if (page_was_full)
    {
        HANDLE_NOTHING(Nothing, table_borrow_page(state, table->id, ptr.page_id));
        HANDLE_NOTHING(Nothing, table_join_page(state, table->id, ptr.page_id));
    }

    page_close(&page);

    OK(Nothing);
}

Maybe_Nothing row_iterator_switch_to_full_pages(State *state, RowIterator *it)
{
    if (it == NULL) OK(Nothing);
    if (it->is_null) OK(Nothing);
    if (it->switched_to_full) OK(Nothing);
    Table *table = HANDLE(Nothing, TablePtr, table_open_by_id(state, it->slot_iterator.ptr.table_id));
    if (table == NULL) EXCEPTION(Nothing, "table is not exists");
    slot_iterator_free(&it->slot_iterator);
    it->slot_iterator = HANDLE(Nothing, SlotIterator, slot_iterator_new(state->file, (SlotPtr) { .table_id = table->id, .page_id = table->first_full_page_id, .slot_id = 0 }));
    it->switched_to_full = 1;
    OK(Nothing);
}

Maybe_Nothing row_iterator_next(State *state, RowIterator *it)
{
    if (it == NULL) OK(Nothing);
    if (it->is_null) OK(Nothing);

    HANDLE_NOTHING(Nothing, slot_iterator_next(state->file, &it->slot_iterator));
    while (!it->slot_iterator.is_null)
    {
        if (!HANDLE(Nothing, uint8_t, page_is_slot_free(it->slot_iterator.page_it.page, it->slot_iterator.ptr.slot_id))) break;
        HANDLE_NOTHING(Nothing, slot_iterator_next(state->file, &it->slot_iterator));
    }

    if (it->slot_iterator.is_null)
    {
        if (!it->switched_to_full) row_iterator_switch_to_full_pages(state, it);
        if (it->slot_iterator.is_null) it->is_null = 1;
    }

    row_free(&it->row);
    if (!it->is_null)
    {
        Row row = HANDLE(Nothing, Row, table_read_row(state, it->slot_iterator.ptr));
        it->row = row_new(ColumnSchema_linked_list_new());
        HANDLE_NOTHING(Nothing, row_join_row(&it->row, &row, it->table_name));
        row_free(&row);
    }
    OK(Nothing);
}

Maybe_RowIterator row_iterator_new(State *state, uint16_t table_id)
{
    Table *table = HANDLE(RowIterator, TablePtr, table_open_by_id(state, table_id));
    if (table == NULL) EXCEPTION(RowIterator, "table is not exists");

    RowIterator it = (RowIterator) { .is_null = 0, .switched_to_full = 0, .table_name = table->name, .slot_iterator = HANDLE(RowIterator, SlotIterator, slot_iterator_new(state->file, (SlotPtr) { .table_id = table_id, .page_id = table->first_page_id, .slot_id = 0 })) };
    while (!it.slot_iterator.is_null)
    {
        if (!HANDLE(RowIterator, uint8_t, page_is_slot_free(it.slot_iterator.page_it.page, it.slot_iterator.ptr.slot_id))) break;
        HANDLE_NOTHING(RowIterator , slot_iterator_next(state->file, &it.slot_iterator));
    }

    if (it.slot_iterator.is_null)
    {
        row_iterator_switch_to_full_pages(state, &it);
        if (it.slot_iterator.is_null) it.is_null = 1;
    }

    if (!it.is_null)
    {
        Row row = HANDLE(RowIterator, Row, table_read_row(state, it.slot_iterator.ptr));
        it.row = row_new(ColumnSchema_linked_list_new());
        HANDLE_NOTHING(RowIterator, row_join_row(&it.row, &row, it.table_name));
        row_free(&row);
    }
    RESULT(RowIterator, it);
}

void row_iterator_free(RowIterator *it)
{
    slot_iterator_free(&it->slot_iterator);
    if (!it->is_null) row_free(&it->row);
    it->is_null = 1;
}
