#include "slot.h"

LINKED_LIST(SlotPtr);

Maybe_SlotIterator slot_iterator_new(File *file, SlotPtr ptr)
{
    SlotIterator it = (SlotIterator) { .is_null = 0, .ptr = ptr, .page_it = HANDLE(SlotIterator, PageIterator, page_iterator_new(file, ptr.page_id)) };
    if (is_slot_ptr_null(it.ptr) | it.page_it.is_null) it.is_null = 1;
    RESULT(SlotIterator, it);
}

Maybe_Nothing slot_iterator_next(File *file, SlotIterator* it)
{
    if (it == NULL) OK(Nothing);
    if (it->is_null) OK(Nothing);
    if ((it->ptr.slot_id + 1) < it->page_it.page->header->slots_count) it->ptr.slot_id++;
    else
    {
        HANDLE_NOTHING(Nothing, page_iterator_next(file, &it->page_it));
        if (it->page_it.is_null) it->is_null = 1;
        else
        {
            it->ptr.page_id = it->page_it.id;
            it->ptr.slot_id = 0;
        }
    }
    OK(Nothing);
}

void slot_iterator_free(SlotIterator *it)
{
    page_iterator_free(&it->page_it);
}

uint8_t is_slot_ptr_null(SlotPtr ptr)
{
    SlotPtr null_ptr = NULL_SLOT_PTR;
    return ((ptr.table_id == null_ptr.table_id) & (ptr.page_id == null_ptr.page_id) & (ptr.slot_id == null_ptr.slot_id));
}