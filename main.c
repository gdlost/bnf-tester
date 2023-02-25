#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "data.h"

const char *toks_name[] = {
    TOKS(STRFY)
};

void execute_rule(struct bnf_grammar *G, struct bnf_rule *r);
void parser_begin();

int main(int argc, char *argv[]) {
    FILE *fp = fopen(argv[1], "r");

    lex_init(fp);

    while (lex_input()) {
        parser_begin();
    }

    printf("Grammar parsed\n");
    struct bnf_grammar *G = get_current_grammar();
    struct bnf_rule *first_rule = G->rules[0];
    // Escoger un grupo aleatoriamente
    srand(time(NULL));
    /*
    int which = rand() % first_rule->group_len;
    struct bnf_group *wg = first_rule->group_list[which];

    for (int i = 0;i<wg->atom_len;i++) {
        if (wg->atom_list[i].terminal == ATOM_TERMINAL) {
            printf("%s", wg->atom_list[i].str);
        } else {
            // buscar la regla
            // elegir un grupo
            // ejecutar
            // repetir el proceso
        }
    }
    */
    for (int i = 0; i < 100; i++) {
        execute_rule(G, first_rule);
        printf("\n");
    }

    fclose(fp);
    return 0;
}

struct bnf_rule *search_rule(const char *rname, struct bnf_grammar *G)
{
    for (int i = 0; i < 128; i++) {
        struct bnf_rule *r = G->rules[i];
        if (!strcmp(rname, r->name)) {
            return r;
        }
    }
    return NULL;
}

// Se le pasa el parametro G para poder buscar las otras reglas
// si se necesitan
void execute_rule(struct bnf_grammar *G, struct bnf_rule *r)
{
    // elegir grupo
    int which = rand() % r->group_len;
    struct bnf_group *wg = r->group_list[which];

    for (int i = 0;i<wg->atom_len;i++) {
        if (wg->atom_list[i].terminal == ATOM_TERMINAL) {
            printf("%s", wg->atom_list[i].str);
        } else {
            struct bnf_rule *R =search_rule(wg->atom_list[i].str, G);
            if (!R) {
                fprintf(stderr, "Error: unreachable rule!");
                exit(1);
            }
            execute_rule(G, R);
        }
    }
}
