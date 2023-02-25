#ifndef DATA_H
#define DATA_H

#define LINE_MAX 512

struct bnf_atom {
    int terminal; // 1: terminal, 0: no terminal
    const char *str;
    int strl;
};

// Representa un literal
#define ATOM_TERMINAL 1
// Representa una "referencia" a una regla
#define ATOM_NO_TERMINAL 0

struct bnf_group {
    // Apunta a la cabeza
    struct bnf_atom *atom_list; 
    int atom_len;
    int atom_capacity;
};

struct bnf_rule {
    const char *name;
    struct bnf_group **group_list;
    int group_len;     
	int group_capacity;
};

struct bnf_grammar {
    struct bnf_rule *rules[128];
    int rules_len;
    int capacity;
};

const char *lex_get_last_string();

int lex_get_last_token();
int lex_get_last_string_length();
int lex_advance();
int lex_peek();
int lex_input();
int lex_init(FILE *fp);
int lex_match(int d);

#define TOKS(OP) \
    OP(INVALID_S),\
    OP(OR_S), \
    OP(LEFT_ARROW_S), \
    OP(RIGHT_ARROW_S), \
    OP(DEFINITION_S), \
    OP(STRING_S), \
    OP(IDENTIFIER_S), \
    OP(SPACE_S), \
    OP(EOL_S), \
    OP(EOF_S), \
    OP(SEMI_COLON_S)

#define STRNUM(x) x
#define STRFY(x) #x

enum lex_tokens {
    TOKS(STRNUM)
};

extern const char *toks_name[];

struct bnf_group *group_create();
void group_append(struct bnf_group *g, struct bnf_atom *a) ;
struct bnf_rule *rule_create();
void rule_append(struct bnf_rule *r, struct bnf_group *g);
char *copy_str_malloc(int len, const char *s);

void grammar_append(struct bnf_grammar *G, struct bnf_rule *r);
struct bnf_grammar *get_current_grammar();
#endif // DATA_H
