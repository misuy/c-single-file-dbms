#ifndef LEX_H_
#define LEX_H_

#include "finite_automata.h"
#include "tree.h"

#define ERROR_WIDTH 20

int yylex(YYSTYPE *yylval, FiniteAutomata *automata, Text *text);
void yyerror(FiniteAutomata *automata, Text *text, Node **node, char *msg);

#endif
