#ifndef TEXT_H_
#define TEXT_H_

#include "string.h"
#include "stdlib.h"

#include "inttypes.h"

typedef struct Text
{
    char *text;
    uint32_t offset;
    uint32_t size;
} Text;



Text * text_new(char *textt);
void text_free(Text *text);

#endif
