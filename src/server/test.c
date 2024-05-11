#include "time.h"

#include "interface.h"

#define TEST_TABLE_NAME "test"
#define TEST1_TABLE_NAME "test1"

typedef struct TestResult
{
    uint32_t elements_count;
    float time;
} TestResult;

ColumnSchema_LinkedList test_table_schema(void);
void ** random_test_table_row_get(void);
void test_table_row_free(void **row);

void test_general(char *storage_file_name);



ColumnSchema_LinkedList test_table_schema(void)
{
    ColumnSchema_LinkedList schema = ColumnSchema_linked_list_new();
    ColumnSchema_linked_list_push_back(&schema, column_schema_new("test_bool", BOOL));
    ColumnSchema_linked_list_push_back(&schema, column_schema_new("test_uint8", UINT_8));
    ColumnSchema_linked_list_push_back(&schema, column_schema_new("test_uint16", UINT_16));
    ColumnSchema_linked_list_push_back(&schema, column_schema_new("test_uint32", UINT_32));
    ColumnSchema_linked_list_push_back(&schema, column_schema_new("test_int32", INT_32));
    ColumnSchema_linked_list_push_back(&schema, column_schema_new("test_float", FLOAT));
    return schema;
}

void ** random_test_table_row_get(void)
{
    uint8_t test_bool = 0;
    if (((float) rand() / RAND_MAX) > 0.5) test_bool = 1;
    uint8_t test_uint8 = (uint8_t) (((float) rand() / RAND_MAX) * UINT8_MAX);
    uint16_t test_uint16 = (uint16_t) (((float) rand() / RAND_MAX) * UINT16_MAX);
    uint32_t test_uint32 = (uint32_t) (((float) rand() / RAND_MAX) * UINT32_MAX);
    int32_t test_int32 = (int32_t) ((((float) rand() / RAND_MAX) - 0.5) * INT32_MAX);
    float test_float = (float) ((float) rand() / RAND_MAX);

    void **row = malloc(sizeof(void *) * 6);
    row[0] = malloc(sizeof(uint8_t));
    row[1] = malloc(sizeof(uint8_t));
    row[2] = malloc(sizeof(uint16_t));
    row[3] = malloc(sizeof(uint32_t));
    row[4] = malloc(sizeof(int32_t));
    row[5] = malloc(sizeof(float));
    *((uint8_t *) row[0]) = test_bool;
    *((uint8_t *) row[1]) = test_uint8;
    *((uint16_t *) row[2]) = test_uint16;
    *((uint32_t *) row[3]) = test_uint32;
    *((int32_t *) row[4]) = test_int32;
    *((float *) row[5]) = test_float;

    return row;
}

void test_table_row_free(void **row)
{
    free(row[0]);
    free(row[1]);
    free(row[2]);
    free(row[3]);
    free(row[4]);
    free(row[5]);
    free(row);
}


void test_general(char *storage_file_name)
{
    srand(time(NULL));

    State state = storage_connect(storage_file_name);

    printf("create or open TEST table\n");
    ColumnSchema_LinkedList schema = test_table_schema();
    Table *table = table_open(&state, TEST_TABLE_NAME);
    if (table == 0)
        table = table_create(&state, TEST_TABLE_NAME, &schema);

    Table *table2 = table_open(&state, TEST1_TABLE_NAME);
    if (table2 == 0)
        table2 = table_create(&state, TEST1_TABLE_NAME, &schema);


    Row_LinkedList rows = Row_linked_list_new();
    for (uint32_t i=0; i<50; i++)
    {
        void **row = random_test_table_row_get();
        rows_add_row(&rows, row_new_by_values(&schema, row));
        test_table_row_free(row);
    }
    InsertQuery insert_query = insert_query_new(table, rows);
    insert_query_execute(&state, &insert_query);
    insert_query_free(&insert_query);

    Row_LinkedList rows2 = Row_linked_list_new();
    for (uint32_t i=0; i<50; i++)
    {
        void **row = random_test_table_row_get();
        rows_add_row(&rows2, row_new_by_values(&schema, row));
        test_table_row_free(row);
    }
    InsertQuery insert_query2 = insert_query_new(table2, rows2);
    insert_query_execute(&state, &insert_query2);
    insert_query_free(&insert_query2);

    uint8_t true_value = 1;
    Predicate *predicate = predicate_new(operator_equal, operand_new_by_ptr("test.test_bool"), operand_new_by_value(TRY(CellValue, cell_value_new(BOOL, &true_value))));
    Predicate *join_predicate = predicate_new(operator_less, operand_new_by_ptr("test.test_float"), operand_new_by_ptr("test1.test_float"));
    DataIterator it = data_iterator_table_new(&state, table->id);
    DataIterator join_it = data_iterator_join_new(&state, &it, table2->id, join_predicate);
    SelectQuery select_query = select_query_new(&join_it, predicate, Projection_linked_list_new());
    Cell_LinkedList_LinkedList cells = TRY(Cell_LinkedList_LinkedList, select_query_execute(&state, &select_query));
    select_query_free(&select_query);

    Cell_LinkedList_LinkedListNode *cells_it = cells.head;
    while (cells_it != NULL)
    {
        printf("\n");
        cells_print(cells_it->value);
        cells_it = cells_it->next;
    }
    
    storage_disconnect(&state);
}


int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("usage: lab1 [mode (1 -- 500 insert 400 delete test; 2 -- general tests)]\nlab1 1 [storage_file_name] [insert_results_file_name] [delete_results_file_name]\nlab1 2 [storage_file_name]\n");
        exit(1);
    }
    if (argv[1][0] == '1')
    {
        if (argc != 5)
        {
            printf("usage: lab1 [mode (1 -- 500 insert 400 delete test; 2 -- general tests)]\nlab1 1 [storage_file_name] [insert_results_file_name] [delete_results_file_name]\nlab1 2 [storage_file_name]\n");
            exit(1);
        }
        //test_500_insert_400_delete(argv[2], argv[3], argv[4]);
    }
    else if (argv[1][0] == '2')
    {
        if (argc != 3)
        {
            printf("usage: lab1 [mode (1 -- 500 insert 400 delete test; 2 -- general tests)]\nlab1 1 [storage_file_name] [insert_results_file_name] [delete_results_file_name]\nlab1 2 [storage_file_name]\n");
            exit(1);
        }
        test_general(argv[2]);
    }
    else
    {
        printf("usage: lab1 [mode (1 -- 500 insert 400 delete test; 2 -- general tests)]\nlab1 1 [storage_file_name] [insert_results_file_name] [delete_results_file_name]\nlab1 2 [storage_file_name]\n");
        exit(1);
    }
}