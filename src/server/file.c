#include "file.h"

Maybe_Nothing file_init(File* file);
Maybe_Nothing file_read_header(File* file);


Maybe_Nothing file_write_header(File* file)
{
    fseek(file->stream, 0, SEEK_SET);
    if (fwrite(file->header, sizeof(FileHeader), 1, file->stream) != 1) EXCEPTION(Nothing, "can't write to storage file");
    OK(Nothing);
}

Maybe_Nothing file_read_header(File* file)
{
    fseek(file->stream, 0, SEEK_SET);
    if (fread(file->header, sizeof(FileHeader), 1, file->stream) == 1)
    {
        if (file->header->magic == FILE_MAGIC) OK(Nothing);
        EXCEPTION(Nothing, "corrupted storage file");
    }
    EXCEPTION(Nothing, "corrupted storage file");
}

void file_increment_pages_count(File* file)
{
    file->header->pages_count++;
}

Maybe_Nothing file_init(File* file)
{
    file->stream = fopen(file->name, "w+b");
    if (file->stream == NULL) EXCEPTION(Nothing, "can't open storage file");

    *(file->header) = (FileHeader) {
        .magic = FILE_MAGIC,
        .page_size = PAGE_SIZE,
        .pages_count = 0,
        .tables_count = 3 + MAX_POOL_TABLE_N,
        .data_density = DATA_DENSITY,
        .root_table_id = ROOT_TABLE_ID,
        .free_table_id = FREE_TABLE_ID,
        .pool_tables_count = MAX_POOL_TABLE_N + 1,
        .first_pool_table_id = FIRST_POOL_TABLE_ID,
        .root_table_first_page_id = 0,
        .root_table_first_full_page_id = 0,
        .first_page_offset = sizeof(FileHeader),
    };
    HANDLE_NOTHING(Nothing, file_write_header(file));
    OK(Nothing);
}

Maybe_File file_open(char* file_name)
{
    File file = (File) { .name = file_name, .stream = fopen(file_name, "r+b"), .header = malloc(sizeof(FileHeader)) };
    if (file.header == NULL) EXCEPTION(File, "can't allocate enough memory");
    if (file.stream == NULL)
    {
        HANDLE_NOTHING(File, file_init(&file));
        RESULT(File, file);
    }
    HANDLE_NOTHING(File, file_read_header(&file));
    RESULT(File, file);
}

Maybe_Nothing file_close(File* file)
{
    free(file->header);
    if (!fclose(file->stream)) OK(Nothing);
    EXCEPTION(Nothing, "can't close storage file");
}
