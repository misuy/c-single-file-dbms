#ifndef SLOT_H_
#define SLOT_H_

#include "util.h"
#include "file.h"
#include "page.h"



typedef struct SlotPtr
{
    uint16_t table_id;
    uint32_t page_id;
    uint16_t slot_id;
} SlotPtr;

#define NULL_SLOT_PTR (SlotPtr) { .table_id = 0, .page_id = 0, .slot_id = 0 }

LINKED_LIST_HEADER(SlotPtr)


typedef struct SlotIterator
{
    uint8_t is_null;
    SlotPtr ptr;
    PageIterator page_it;
} SlotIterator;

MAYBE(SlotIterator, SlotIterator)



uint8_t is_slot_ptr_null(SlotPtr);

Maybe_SlotIterator slot_iterator_new(File *, SlotPtr);
Maybe_Nothing slot_iterator_next(File *, SlotIterator *);
void slot_iterator_free(SlotIterator *);

#endif