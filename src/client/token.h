#ifndef TOKEN_H_
#define TOKEN_H_

#include <inttypes.h>
#include <stdlib.h>

#include "syntax.h"

typedef struct TokenSpec
{
    yytoken_kind_t type;
    uint32_t priority;
    char *regex;
} TokenSpec;

typedef struct Token
{
    yytoken_kind_t type;
    char *value;
} Token;

#define DEFAULT_TOKENS_COUNT 39

TokenSpec * token_spec_new(yytoken_kind_t type, uint32_t priority, char *regex);
void token_free(Token *token);
void token_spec_free(TokenSpec *spec);
TokenSpec ** tokens_default(void);

#endif