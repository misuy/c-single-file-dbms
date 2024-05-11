#include "finite_automata.h"

void finite_automata_alphabet_free(FiniteAutomataAlphabet *alphabet);
uint8_t * regex_next_symbols_mask(char *regex, uint32_t *offset, FiniteAutomataAlphabet *alphabet);
FiniteAutomataState * token_spec_to_finite_automata_impl(TokenSpec *token_spec, FiniteAutomataAlphabet *alphabet, uint32_t *offset, uint8_t *rec_mask);
FiniteAutomataOutput finite_automata_parse_impl(FiniteAutomataState *state, Text *text, uint32_t local_offset);

FiniteAutomataAlphabet * finite_automata_alphabet_new(char *symbols)
{
    FiniteAutomataAlphabet *alphabet = malloc(sizeof(FiniteAutomataAlphabet));
    alphabet->symbols = symbols;
    alphabet->size = strlen(symbols);
    return alphabet;
}

FiniteAutomataAlphabet * finite_automata_alphabet_default(void)
{
    return finite_automata_alphabet_new("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZá0123456789=<>&|!.,;*()_:/\" ");
}

void finite_automata_alphabet_free(FiniteAutomataAlphabet *alphabet)
{
    if (alphabet != 0)
        free(alphabet);
}



uint8_t * regex_next_symbols_mask(char *regex, uint32_t *offset, FiniteAutomataAlphabet *alphabet)
{
    if (*(regex + *offset) == 0)
        return 0;

    uint8_t *mask = malloc(sizeof(uint8_t) * alphabet->size);
    for (uint32_t i=0; i<alphabet->size; i++)
    {
        mask[i] = 0;
    }

    if ((*(regex + *offset) == '/') & (*(regex + *offset + 1) == '{'))
    {
        uint32_t left = *offset + 2;
        uint32_t right = left;
        while (*(regex + right) != '}')
            right++;
        uint8_t not = 0;
        for (uint32_t i=left; i<right; i++)
        {
            if (*(regex + i) == '/')
            {
                i++;
                for (uint32_t j=0; j<alphabet->size; j++)
                    if (alphabet->symbols[j] == *(regex + i))
                        mask[j] = !not;
            }
            else if (*(regex + i) == '^')
                not = !not;
            else
            {
                if (*(regex + i) == '.')
                {
                    for (uint32_t j=0; j<alphabet->size; j++)
                        mask[j] = !not;
                }
                else
                {
                    for (uint32_t j=0; j<alphabet->size; j++)
                        if (alphabet->symbols[j] == *(regex + i))
                            mask[j] = !not;
                }
            }
        }
        *offset = right + 1;
    }
    else
    {
        if (*(regex + *offset) == '/')
            (*offset)++;
        for (uint32_t i=0; i<alphabet->size; i++)
            if (alphabet->symbols[i] == *(regex + *offset))
                mask[i] = 1;
        (*offset)++;
    }
    return mask;
}



FiniteAutomataState * token_spec_to_finite_automata_impl(TokenSpec *token_spec, FiniteAutomataAlphabet *alphabet, uint32_t *offset, uint8_t *rec_mask)
{
    FiniteAutomataState *state = malloc(sizeof(FiniteAutomataState));
    state->is_end = 0;
    state->token = 0;
    state->transitions_count = 0;
    state->transitions = 0;
    if (rec_mask != 0)
    {
        state->transitions_count = 0;
        for (uint32_t i=0; i<alphabet->size; i++)
            if (rec_mask[i])
                state->transitions_count++;
    }

    uint8_t *mask = regex_next_symbols_mask(token_spec->regex, offset, alphabet);

    if (mask == 0)
    {
        state->is_end = 1;
        state->token = token_spec;
    }
    else
    {
        /*
        for (uint32_t i=0; i<alphabet->size; i++)
        {
            printf("%"PRIu8".", mask[i]);
        }
        printf("//\n");
        */

        for (uint32_t i=0; i<alphabet->size; i++)
            if (mask[i])
                state->transitions_count++;
    }
    uint8_t is_rec = 0;
    uint8_t e_transition = 0;

    if ((token_spec->regex[*offset] == '*') | ((token_spec->regex[*offset] == '+')))
    {
        if (token_spec->regex[*offset] == '*')
            e_transition = 1;
        is_rec = 1;
        (*offset)++;
    }

    if (e_transition)
        state->transitions_count++;

    state->transitions = malloc(sizeof(FiniteAutomataTransition) * state->transitions_count);

    uint32_t shift = 0;

    if (rec_mask != 0)
    {
        for (uint32_t i=0; i<alphabet->size; i++)
        {
            if (rec_mask[i])
            {
                state->transitions[shift] = (FiniteAutomataTransition) { .symbol = alphabet->symbols[i], .to = state };
                shift++;
            }
        }
    }

    if (mask != 0)
    {
        FiniteAutomataState *next_state;
        if (is_rec)
            next_state = token_spec_to_finite_automata_impl(token_spec, alphabet, offset, mask);
        else
            next_state = token_spec_to_finite_automata_impl(token_spec, alphabet, offset, 0);

        for (uint32_t i=0; i<alphabet->size; i++)
        {
            if (mask[i])
            {
                state->transitions[shift] = (FiniteAutomataTransition) { .symbol = alphabet->symbols[i], .to = next_state };
                shift++;
            }
        }

        if (e_transition)
        {
            state->transitions[shift] = (FiniteAutomataTransition) { .symbol = 0, .to = next_state };
            shift++;
        }

        free(mask);
    }

    return state;
}

FiniteAutomataState * token_spec_to_finite_automata(TokenSpec *token_spec, FiniteAutomataAlphabet *alphabet)
{
    uint32_t offset = 0;
    return token_spec_to_finite_automata_impl(token_spec, alphabet, &offset, 0);
}



FiniteAutomata * finite_automata_new(TokenSpec* tokens[], uint32_t tokens_count, FiniteAutomataAlphabet *alphabet)
{
    FiniteAutomataState *state = malloc(sizeof(FiniteAutomataState));
    state->is_end = 0;
    state->token = 0;

    state->transitions_count = tokens_count;
    state->transitions = malloc(sizeof(FiniteAutomataTransition) * tokens_count);
    for (uint32_t i=0; i<tokens_count; i++)
    {
        state->transitions[i] = (FiniteAutomataTransition) { .symbol = 0, .to = token_spec_to_finite_automata(tokens[i], alphabet) };
    }

    FiniteAutomata *automata = malloc(sizeof(FiniteAutomata));
    automata->init_state = state;

    return automata;
}



FiniteAutomataOutput finite_automata_parse_impl(FiniteAutomataState *state, Text *text, uint32_t local_offset)
{
    FiniteAutomataOutput ret, next_ret;
    if (state->is_end)
        ret = (FiniteAutomataOutput) { .token = state->token, .token_value_size = local_offset };
    else
        ret = (FiniteAutomataOutput) { .token = 0, .token_value_size = 0 };
    next_ret = (FiniteAutomataOutput) { .token = 0, .token_value_size = 0 };

    for (uint32_t i=0; i<state->transitions_count; i++)
    {
        if (state->transitions[i].symbol == 0)
        {
            next_ret = finite_automata_parse_impl(state->transitions[i].to, text, local_offset);

            if (next_ret.token != 0)
            {
                if (ret.token == 0)
                    ret = next_ret;
                else
                {
                    if (next_ret.token_value_size > ret.token_value_size)
                        ret = next_ret;
                    else if (next_ret.token_value_size == ret.token_value_size)
                    {
                        if (next_ret.token->priority < ret.token->priority)
                            ret = next_ret;
                    }
                }
            }
        }
    }

    if ((text->offset + local_offset) >= text->size)
        return ret;

    char symbol = text->text[text->offset + local_offset];

    for (uint32_t i=0; i<state->transitions_count; i++)
    {
        if (state->transitions[i].symbol == symbol)
        {
            next_ret = finite_automata_parse_impl(state->transitions[i].to, text, local_offset+1);
            
            if (next_ret.token != 0)
            {
                if (ret.token == 0)
                    ret = next_ret;
                else
                {
                    if (next_ret.token_value_size > ret.token_value_size)
                        ret = next_ret;
                    else if (next_ret.token_value_size == ret.token_value_size)
                    {
                        if (next_ret.token->priority < ret.token->priority)
                            ret = next_ret;
                    }
                }
            }
        }
    }

    return ret;
}

Token finite_automata_parse(FiniteAutomata *automata, Text *text)
{
    Token token = (Token) { .type = YYEOF, .value = "" };

    while ((text->offset < text->size) & ((text->text[text->offset] == ' ') | (text->text[text->offset] == '\n')))
        text->offset++;
    FiniteAutomataOutput output = finite_automata_parse_impl(automata->init_state, text, 0);
    if (output.token == 0)
        return token;

    token.type = output.token->type;
    token.value = malloc(sizeof(char) * output.token_value_size + 1);
    memcpy(token.value, text->text + text->offset, output.token_value_size);
    text->offset += output.token_value_size;
    token.value[output.token_value_size] = 0;

    return token;
}



void finite_automata_state_print(FiniteAutomataState *state, uint32_t rec)
{
    for (uint32_t i=0; i<rec; i++)
        printf(" ");
    printf("%"PRIu32"\n", state->transitions_count);
    for (uint32_t i=0; i<state->transitions_count; i++)
    {
        for (uint32_t j=0; j<rec; j++)
            printf(" ");
        printf("%c:\n", (state->transitions + i)->symbol);
        for (uint32_t j=0; j<rec; j++)
            printf(" ");
        printf("{\n");
        if ((state->transitions + i)->to != state)
        {
            finite_automata_state_print((state->transitions + i)->to, rec + 1);
        }
        else
        {
            for (uint32_t j=0; j<rec; j++)
                printf(" ");
            printf("rec!!\n");
        }
        for (uint32_t j=0; j<rec; j++)
            printf(" ");
        printf("}\n");
    }
}