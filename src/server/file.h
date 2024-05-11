#ifndef FILE_H_
#define FILE_H_

#include "inttypes.h"
#include "stdio.h"
#include "math.h"
#include "memory.h"

#include "util.h"


#define FILE_MAGIC 0x0F0F
#define PAGE_SIZE 4096
#define MAX_TABLES_COUNT 1000
#define DATA_DENSITY 0.7
#define ROOT_TABLE_ID 0
#define FREE_TABLE_ID 1
#define FIRST_POOL_TABLE_ID 2
#define MAX_POOL_TABLE_N ((uint16_t) log2((PAGE_SIZE - sizeof(FileHeader)) / 2) - 3)
#define POOL_TABLE_ROW_SIZE(n) ((PAGE_SIZE - sizeof(FileHeader) - ((2 << n) / 2)) / ((2 << n) / 2))


typedef struct FileHeader
{
    uint16_t magic;
    uint16_t page_size;
    uint32_t pages_count;
    uint16_t tables_count;
    float data_density;
    uint16_t root_table_id;
    uint16_t free_table_id;
    uint16_t pool_tables_count;
    uint16_t first_pool_table_id;
    uint32_t root_table_first_page_id;
    uint32_t root_table_first_full_page_id;
    uint16_t first_page_offset;
} FileHeader;

MAYBE(FileHeader, FileHeader)


typedef struct File
{
    char* name;
    FILE* stream;
    FileHeader* header;
} File;

MAYBE(File, File)



Maybe_File file_open(char *);
Maybe_Nothing file_close(File *);
void file_increment_pages_count(File *);
Maybe_Nothing file_write_header(File *);

#endif