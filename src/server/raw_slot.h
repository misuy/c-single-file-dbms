#ifndef RAW_SLOT_H_
#define RAW_SLOT_H_

#include "inttypes.h"

#include "util.h"



typedef struct RawSlot
{
    uint16_t size;
    uint16_t offset;
    uint8_t *value;
} RawSlot;

MAYBE(RawSlot, RawSlot)



Maybe_RawSlot raw_slot_new(uint16_t);
void raw_slot_free(RawSlot *);

#endif