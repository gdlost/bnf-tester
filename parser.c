#include <stdio.h>
#include <stdlib.h>

#include "data.h"

#define SYN_ERROR(msg) fprintf(stderr, "Syntax Error: " msg); \
    fprintf(stderr, "\n%s()\tReceived an %s", __func__,toks_name[lex_get_last_token()]); exit(1)

struct bnf_grammar current = {.rules_len = 0, .capacity = 128};

struct bnf_grammar *get_current_grammar()
{
    return &current;
}

void rule_name_def(struct bnf_rule *r)
{
    if (!lex_match(LEFT_ARROW_S)) {
        SYN_ERROR("Invalid rule syntax. Expected '<' token.");
    }

    if (!lex_match(IDENTIFIER_S)) {
        SYN_ERROR("Invalid rule syntax. Expected identifier");
    }
    
    if (!lex_match(RIGHT_ARROW_S)) {
        SYN_ERROR("Invalid rule syntax. Expected '>' token.");
    }

    if (!lex_match(DEFINITION_S)) {
        SYN_ERROR("Invalid rule syntax. Expected '::=' token.");
    }

    printf("Rule definition: %s\n", lex_get_last_string());
    r->name = copy_str_malloc(lex_get_last_string_length(), lex_get_last_string());
}

void check_rule_name()
{
    if (!lex_match(LEFT_ARROW_S)) {
        SYN_ERROR("Invalid rule syntax. Expected '<' token.");
    }

    if (!lex_match(IDENTIFIER_S)) {
        SYN_ERROR("Invalid rule syntax. Expected identifier");
    }
    
    if (!lex_match(RIGHT_ARROW_S)) {
        SYN_ERROR("Invalid rule syntax. Expected '>' token.");
    }
}


int term(struct bnf_atom *a)
{
    if (lex_match(STRING_S)) {
        a->terminal = ATOM_TERMINAL;
        a->strl = lex_get_last_string_length();
        a->str = copy_str_malloc(a->strl, lex_get_last_string());
        return 1;
    } else if (lex_match(LEFT_ARROW_S) 
            && lex_match(IDENTIFIER_S)
            && lex_match(RIGHT_ARROW_S)) {

        a->terminal = ATOM_NO_TERMINAL;
        a->strl = lex_get_last_string_length();
        a->str = copy_str_malloc(a->strl, lex_get_last_string());
        return 1;
    }

    return 0;
}

// Esta funcion debe armar un grupo de atoms (terminales y reglas) 
struct bnf_group *list()
{
    struct bnf_group *group = group_create();
    // Existe si o si un primer termino
    struct bnf_atom a;

    if (!term(&a)) {
        SYN_ERROR("A production rule must have at least one terminal or non terminal.");
    }

	group_append(group, &a);

	int ret = 0;

	do {
		ret = term(&a);
		if (ret)
			group_append(group, &a);
	} while (ret);

	return group;
}

// Esta funcion debe armar una regla
void expression(struct bnf_rule *r)
{
    struct bnf_group *g = list();
    rule_append(r, g);
    if (lex_match(OR_S)) {
        expression(r);
    }

    // eeeeeeeeeeeeeeeeeeeeeeeeeeeee
    //if (!(lex_match(EOL_S) || lex_match(EOF_S))) {
     //   SYN_ERROR("Expected end of line.");
    //}

	//if (!(lex_match(SEMI_COLON_S))) {
	//	SYN_ERROR("Expected end of rule ;\n");
//	}
}

// TODO: se debe avanzar el stream para los errores
void rule()
{
    struct bnf_rule *r = rule_create();
    rule_name_def(r);
    expression(r);
	if (!(lex_match(SEMI_COLON_S))) {
        lex_advance();
		SYN_ERROR("Expected end of rule ;");
	}
    grammar_append(&current, r);
}

void parser_begin() {
    rule();
}
