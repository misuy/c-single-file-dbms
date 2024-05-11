#ifndef FINITE_AUTOMATA_H_
#define FINITE_AUTOMATA_H_

#include <stdio.h>

#include "token.h"
#include "text.h"

typedef struct FiniteAutomataAlphabet
{
    uint32_t size;
    char *symbols;
} FiniteAutomataAlphabet;

typedef struct FiniteAutomataTransition
{
    char symbol;
    struct FiniteAutomataState *to;
} FiniteAutomataTransition;

typedef struct FiniteAutomataState
{
    uint8_t is_end;
    TokenSpec *token;
    uint32_t transitions_count;
    FiniteAutomataTransition *transitions;
} FiniteAutomataState;

typedef struct FiniteAutomataOutput
{
    TokenSpec *token;
    uint32_t token_value_size;
} FiniteAutomataOutput;

typedef struct FiniteAutomata
{
    FiniteAutomataState *init_state;
} FiniteAutomata;



FiniteAutomataAlphabet * finite_automata_alphabet_new(char *symbols);
FiniteAutomataAlphabet * finite_automata_alphabet_default(void);

FiniteAutomataState * token_spec_to_finite_automata(TokenSpec *token_spec, FiniteAutomataAlphabet *alphabet);

FiniteAutomata * finite_automata_new(TokenSpec* tokens[], uint32_t tokens_count, FiniteAutomataAlphabet *alphabet);
Token finite_automata_parse(FiniteAutomata *automata, Text *text);
void finite_automata_state_print(FiniteAutomataState *state, uint32_t rec);



#endif