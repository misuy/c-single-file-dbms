#include "raw_slot.h"

Maybe_RawSlot raw_slot_new(uint16_t size)
{
    uint8_t *ptr = malloc(size);
    if (ptr == NULL) EXCEPTION(RawSlot, "can't allocate enough memory for a raw slot");
    RESULT(RawSlot, ((RawSlot) { .size = size, .offset = 0, .value = ptr }));
}

void raw_slot_free(RawSlot *raw_slot)
{
    free(raw_slot->value);
}
