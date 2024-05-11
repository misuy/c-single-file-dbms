#include "text.h"

Text * text_new(char *textt)
{
    Text *text = malloc(sizeof(Text));
    text->text = textt;
    text->size = strlen(textt);
    text->offset = 0;
    return text;
}

void text_free(Text *text)
{
    if (text->text != 0)
        free(text->text);
}