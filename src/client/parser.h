#ifndef PARSER_H_
#define PARSER_H_

#include "syntax.h"
#include "token.h"
#include "tree.h"
#include "finite_automata.h"



int parse(Node **node, FiniteAutomata *automata, Text *text);



#endif