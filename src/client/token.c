#include "token.h"

TokenSpec * token_spec_new(yytoken_kind_t type, uint32_t priority, char *regex)
{
    TokenSpec *spec = malloc(sizeof(TokenSpec));
    *spec = (TokenSpec) { .type = type, .priority = priority, .regex = regex };
    return spec;
}

void token_spec_free(TokenSpec *spec)
{
    if (spec != 0)
        free(spec);
}

void token_free(Token *token)
{
    if (token->value != 0)
        free(token->value);
}

TokenSpec** tokens_default(void)
{
    
    TokenSpec **tokens = malloc(sizeof(TokenSpec *) * DEFAULT_TOKENS_COUNT);
    tokens[0] = token_spec_new(LBRACE, 0, "(");
    tokens[1] = token_spec_new(RBRACE, 0, ")");
    tokens[2] = token_spec_new(DOT, 0, ".");
    tokens[3] = token_spec_new(COMMA, 0, ",");
    tokens[4] = token_spec_new(ASTERISK, 0, "*");
    tokens[5] = token_spec_new(SEMICOLON, 0, ";");

    tokens[6] = token_spec_new(AND, 1, "&");
    tokens[7] = token_spec_new(OR, 1, "|");
    tokens[8] = token_spec_new(NOT, 1, "!");

    tokens[9] = token_spec_new(EQUAL, 2, "==");
    tokens[10] = token_spec_new(NOT_EQUAL, 2, "!=");
    tokens[11] = token_spec_new(LESS, 2, "<");
    tokens[12] = token_spec_new(NOT_LESS, 2, ">=");
    tokens[13] = token_spec_new(GREATER, 2, ">");
    tokens[14] = token_spec_new(NOT_GREATER, 2, "<=");
    tokens[15] = token_spec_new(SUBSTR, 2, "substr");

    tokens[16] = token_spec_new(ASSIGN, 2, "=");

    tokens[17] = token_spec_new(FROM, 3, "from");
    tokens[18] = token_spec_new(IN, 3, "in");
    tokens[19] = token_spec_new(WHERE, 3, "where");
    tokens[20] = token_spec_new(SELECT, 3, "select");
    tokens[21] = token_spec_new(INSERT, 3, "insert");
    tokens[22] = token_spec_new(DELETE, 3, "delete");
    tokens[23] = token_spec_new(UPDATE, 3, "update");
    tokens[24] = token_spec_new(JOIN, 3, "join");
    tokens[25] = token_spec_new(ON, 3, "on");
    tokens[26] = token_spec_new(CREATE, 3, "create");

    tokens[27] = token_spec_new(BOOL_TYPE, 4, "bool");
    tokens[28] = token_spec_new(UINT_8_TYPE, 4, "uint_8");
    tokens[29] = token_spec_new(UINT_16_TYPE, 4, "uint_16");
    tokens[30] = token_spec_new(UINT_32_TYPE, 4, "uint_32");
    tokens[31] = token_spec_new(INT_32_TYPE, 4, "int_32");
    tokens[32] = token_spec_new(FLOAT_TYPE, 4, "float");
    tokens[33] = token_spec_new(STRING_TYPE, 4, "string");

    tokens[34] = token_spec_new(BOOL_CONSTANT, 5, "/{tf}");
    tokens[35] = token_spec_new(INTEGER_CONSTANT, 5, "/{0123456789}+");
    tokens[36] = token_spec_new(FLOAT_CONSTANT, 5, "/{0123456789}*./{0123456789}+");
    tokens[37] = token_spec_new(STRING_CONSTANT, 5, "\"/{.^\"^}*\"");
    tokens[38] = token_spec_new(VARIABLE, 5, "/{.^()/.,*;&|!=<>0123456789^}/{.^()/.,*;&|!=<> ^}*");

    return tokens;
}
