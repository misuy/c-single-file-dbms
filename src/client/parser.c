#include "parser.h"

int parse(Node **node, FiniteAutomata *automata, Text *text)
{
    yypstate *parser = yypstate_new();

    int res = yypull_parse(parser, automata, text, node);

    yypstate_delete(parser);

    return res;
}