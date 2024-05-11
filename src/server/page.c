#include "page.h"

Maybe_Page page_open(File* file, uint32_t page_id)
{
    if (page_id == 0) EXCEPTION(Page, "nonexistent page");
    if (page_id > file->header->pages_count) EXCEPTION(Page, "nonexistent page");
    uint8_t* raw_page = malloc(file->header->page_size);
    if (raw_page == NULL) EXCEPTION(Page, "can't allocate enough memory for a page");

    fseek(file->stream, file->header->first_page_offset + file->header->page_size * (page_id - 1), SEEK_SET);
    if (fread(raw_page, file->header->page_size, 1, file->stream) != 1)
    {
        free(raw_page);
        EXCEPTION(Page, "can't read page");
    }


    Page page = (Page) { .raw_ptr = raw_page, .header = (PageHeader*) raw_page };
    page.free_mask = (uint8_t*) page.raw_ptr + page.header->free_mask_offset;
    page.slots = (uint8_t*) page.raw_ptr + page.header->slots_offset;
    RESULT(Page, page);
}

void page_close(Page* page)
{
    free(page->raw_ptr);
}

Maybe_Nothing page_write(File* file, Page* page)
{
    fseek(file->stream, file->header->first_page_offset + file->header->page_size * (page->header->id - 1), SEEK_SET);
    if (fwrite(page->raw_ptr, file->header->page_size, 1, file->stream) == 1) OK(Nothing);
    EXCEPTION(Nothing, "can't write page");
}

void page_resize(File* file, Page* page, uint16_t row_size)
{
    page->header->slot_size = row_size;
    page->header->slots_count = (uint16_t) floor((double) (file->header->page_size - page->header->free_mask_offset) / (1 + page->header->slot_size)); // slots_count <= (page_size - free_mask_offset) / (1 + slot_size)
    page->header->slots_offset = page->header->free_mask_offset + page->header->slots_count;
    page->slots = page->raw_ptr + page->header->slots_offset;

    for (int i=0; i<page->header->slots_count; i++) *(page->free_mask + i) = 1;
}

Maybe_Page page_new(File* file, uint16_t row_size)
{
    uint8_t* raw_page = malloc(file->header->page_size);
    if (raw_page == NULL) EXCEPTION(Page, "can't allocate enough memory for a page");

    Page page = (Page) { .raw_ptr = raw_page, .header = (PageHeader*) raw_page };
    *(page.header) = (PageHeader) { .id = file->header->pages_count + 1, .prev_page_id = 0, .next_page_id = 0, .free_mask_offset = sizeof(PageHeader)};
    page.free_mask = page.raw_ptr + page.header->free_mask_offset;
    file_increment_pages_count(file);
    file_write_header(file);
    page_resize(file, &page, row_size);
    RESULT(Page, page);
}


Maybe_uint8_t page_is_slot_free(Page *page, uint16_t slot_id)
{
    if (page == NULL) EXCEPTION(uint8_t , "null ptr");
    if (slot_id >= page->header->slots_count) EXCEPTION(uint8_t, "nonexistent slot");
    if (page->free_mask == NULL) EXCEPTION(uint8_t, "corrupted free mask");
    RESULT(uint8_t, *(page->free_mask + slot_id));
}

Maybe_uint8_t page_is_full(Page *page)
{
    if (page == NULL) EXCEPTION(uint8_t , "null ptr");
    for (uint16_t i = 0; i < page->header->slots_count; i++)
    {
        if (HANDLE(uint8_t, uint8_t, page_is_slot_free(page, i))) RESULT(uint8_t, 0);
    }
    RESULT(uint8_t, 1);
}

Maybe_uint8_t page_is_empty(Page *page)
{
    if (page == NULL) EXCEPTION(uint8_t , "null ptr");
    for (uint16_t i = 0; i < page->header->slots_count; i++)
    {
        if (!HANDLE(uint8_t, uint8_t, page_is_slot_free(page, i))) RESULT(uint8_t, 0);
    }
    RESULT(uint8_t, 1);
}

Maybe_Nothing page_occupy_slot(Page *page, uint16_t slot_id)
{
    if (page == NULL) EXCEPTION(Nothing, "null ptr");
    if (slot_id >= page->header->slots_count) EXCEPTION(Nothing, "nonexistent slot");
    if (page->free_mask == NULL) EXCEPTION(Nothing, "corrupted free mask");
    *(page->free_mask + slot_id) = 0;
    OK(Nothing);
}

Maybe_Nothing page_free_slot(Page *page, uint16_t slot_id)
{
    if (page == NULL) EXCEPTION(Nothing, "null ptr");
    if (slot_id >= page->header->slots_count) EXCEPTION(Nothing, "nonexistent slot");
    if (page->free_mask == NULL) EXCEPTION(Nothing, "corrupted free mask");
    *(page->free_mask + slot_id) = 1;
    OK(Nothing);
}

Maybe_RawSlot page_pull_raw_slot(Page *page, uint16_t slot_id)
{
    if (slot_id >= page->header->slots_count) EXCEPTION(RawSlot, "nonexistent row");
    RESULT(RawSlot, ((RawSlot) { .size = page->header->slot_size, .offset = 0, .value = page->slots + page->header->slot_size * slot_id }));
}



Maybe_PageIterator page_iterator_new(File* file, uint32_t page_id)
{
    PageIterator it = (PageIterator) { .is_null = 0, .id = page_id, .page = NULL };
    if (it.id == 0) it.is_null = 1;
    else
    {
        it.page = malloc(sizeof(Page));
        *it.page = HANDLE(PageIterator, Page, page_open(file, page_id));
    }
    RESULT(PageIterator, it);
}

Maybe_Nothing page_iterator_next(File* file, PageIterator* it)
{
    if (it == NULL) OK(Nothing);
    if (it->is_null) OK(Nothing);
    it->id = it->page->header->next_page_id;
    page_close(it->page);
    if (it->id == 0)
    {
        it->is_null = 1;
        OK(Nothing);
    }
    *it->page = HANDLE(Nothing, Page, page_open(file, it->id));
    OK(Nothing);
}

Maybe_Nothing page_iterator_prev(File* file, PageIterator* it)
{
    if (it == NULL) OK(Nothing);
    if (it->is_null) OK(Nothing);
    it->id = it->page->header->prev_page_id;
    page_close(it->page);
    if (it->id == 0)
    {
        it->is_null = 1;
        OK(Nothing);
    }
    *it->page = HANDLE(Nothing, Page, page_open(file, it->id));
    OK(Nothing);
}

void page_iterator_free(PageIterator* it)
{
    if (!it->is_null) page_close(it->page);
    free(it->page);
}