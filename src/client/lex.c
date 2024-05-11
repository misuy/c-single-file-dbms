#include "lex.h"

int yylex(YYSTYPE *yylval, FiniteAutomata *automata, Text *text)
{
    Token token = finite_automata_parse(automata, text);
    
    if (token.type == YYEOF)
        return YYEOF;

    *yylval = malloc(sizeof(Node));
    if (token.type == BOOL_CONSTANT)
    {
        (*yylval)->type = NODE_BOOL_CONSTANT;
        if (token.value[0] == 't')
            (*yylval)->bool_constant.value = 1;
        else
            (*yylval)->bool_constant.value = 0;
    }
    else if (token.type == INTEGER_CONSTANT)
    {
        (*yylval)->type = NODE_INTEGER_CONSTANT;
        (*yylval)->integer_constant.value = strtol(token.value, 0, 10);
    }
    else if (token.type == FLOAT_CONSTANT)
    {
        (*yylval)->type = NODE_FLOAT_CONSTANT;
        (*yylval)->float_constant.value = strtod(token.value, 0);
    }
    else if (token.type == STRING_CONSTANT)
    {
        (*yylval)->type = NODE_STRING_CONSTANT;
        uint32_t str_size = strlen(token.value) - 2;
        (*yylval)->string_constant.value = malloc(str_size + 1);
        memcpy((*yylval)->string_constant.value, token.value + 1, str_size);
        (*yylval)->string_constant.value[str_size] = 0;
    }
    else if ((token.type == BOOL_TYPE) | (token.type == UINT_8_TYPE) | (token.type == UINT_16_TYPE) | (token.type == UINT_32_TYPE) | (token.type == INT_32_TYPE) | (token.type == FLOAT_TYPE) | (token.type == STRING_TYPE))
    {
        (*yylval)->type = NODE_TYPE;
        switch (token.type)
        {
            case BOOL_TYPE:
                (*yylval)->typee.type = TYPE_BOOL;
                break;
            case UINT_8_TYPE:
                (*yylval)->typee.type = TYPE_UINT_8;
                break;
            case UINT_16_TYPE:
                (*yylval)->typee.type = TYPE_UINT_16;
                break;
            case UINT_32_TYPE:
                (*yylval)->typee.type = TYPE_UINT_32;
                break;
            case INT_32_TYPE:
                (*yylval)->typee.type = TYPE_INT_32;
                break;
            case FLOAT_TYPE:
                (*yylval)->typee.type = TYPE_FLOAT;
                break;
            case STRING_TYPE:
                (*yylval)->typee.type = TYPE_STRING;
                break;
            default:
                break;
        }
    }
    else if (token.type == VARIABLE)
    {
        (*yylval)->type = NODE_VARIABLE;
        (*yylval)->variable.name = malloc(strlen(token.value) + 1);
        memcpy((*yylval)->variable.name, token.value, strlen(token.value) + 1);
    }
    else
    {
        free(*yylval);
    }

    yytoken_kind_t type = token.type;
    token_free(&token);
    return type;
}

void yyerror(FiniteAutomata *automata __attribute__((unused)), Text *text, Node **node, char *msg __attribute__((unused)))
{
    Node *tree = (*node);
    (*node) = malloc(sizeof(Node));
    (*node)->type = NODE_ERROR;
    (*node)->error.error_offset = text->offset;
    (*node)->error.tree = tree;
    uint32_t copy_left = MIN(ERROR_WIDTH, text->offset);
    uint32_t copy_right = MIN(ERROR_WIDTH, text->size - text->offset);
    (*node)->error.error_message = malloc((copy_left + copy_right) * 4 + 4);

    uint32_t offset = 0;
    memset((*node)->error.error_message + offset, '=', copy_left + copy_right);
    offset += copy_left + copy_right;
    (*node)->error.error_message[offset] = '\n';
    offset++;

    memcpy((*node)->error.error_message + offset, text->text+text->offset-copy_left, copy_left + copy_right);
    offset += copy_left + copy_right;
    (*node)->error.error_message[offset] = '\n';
    offset++;

    memset((*node)->error.error_message + offset, ' ', copy_left + copy_right);
    (*node)->error.error_message[offset + copy_left] = '^';
    offset += copy_left + copy_right;
    (*node)->error.error_message[offset] = '\n';
    offset++;

    memset((*node)->error.error_message + offset, '=', copy_left + copy_right);
    offset += copy_left + copy_right;
    (*node)->error.error_message[offset] = 0;
    offset++;
}