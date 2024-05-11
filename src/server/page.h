#ifndef PAGE_H_
#define PAGE_H_

#include "inttypes.h"

#include "util.h"
#include "file.h"
#include "raw_slot.h"

typedef struct PageHeader
{
    uint32_t id;
    uint32_t prev_page_id;
    uint32_t next_page_id;
    uint16_t slot_size;
    uint16_t slots_count;
    uint16_t free_mask_offset;
    uint16_t slots_offset;
} PageHeader;

MAYBE(PageHeader, PageHeader)


typedef struct Page
{
    uint8_t* raw_ptr;
    struct PageHeader* header;
    uint8_t* free_mask;
    uint8_t* slots;
} Page;

LINKED_LIST_HEADER(Page)


typedef struct PageIterator
{
    uint8_t is_null;
    uint32_t id;
    Page* page;
} PageIterator;

MAYBE(PageIterator, PageIterator)


Maybe_Page page_open(File *, uint32_t);
void page_close(Page *);
Maybe_Page page_new(File *, uint16_t);
Maybe_Nothing page_write(File *, Page *);
void page_resize(File *, Page *, uint16_t);

Maybe_uint8_t page_is_slot_free(Page *, uint16_t);
Maybe_uint8_t page_is_full(Page *);
Maybe_uint8_t page_is_empty(Page *);
Maybe_Nothing page_occupy_slot(Page *, uint16_t);
Maybe_Nothing page_free_slot(Page *, uint16_t);

Maybe_RawSlot page_pull_raw_slot(Page *, uint16_t);

Maybe_PageIterator page_iterator_new(File *, uint32_t);
Maybe_Nothing page_iterator_next(File *, PageIterator *);
Maybe_Nothing page_iterator_prev(File *, PageIterator *);
void page_iterator_free(PageIterator *);

#endif