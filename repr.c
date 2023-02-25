#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

#define ALLOC_ERROR(msg) fprintf(stderr, "Memory Allocation Error: " msg); \
    fprintf(stderr, "%s():%s%d",__func__, __FILE__, __LINE__); exit(1)

char *copy_str_malloc(int len, const char *s)
{
    char *m = malloc(len + 1);
    if (!m) return NULL;
    strncpy(m, s, len);
    m[len] = '\0';
    return m;
}

void grammar_append(struct bnf_grammar *G, struct bnf_rule *r)
{
    G->rules[G->rules_len] = r;
    G->rules_len++;
}


#define GROUP_BASE_CAPACITY 16
#define GROUP_GROW 4

struct bnf_rule *rule_create()
{
	struct bnf_rule *r = calloc(1, sizeof(struct bnf_rule));
    r->group_capacity = GROUP_BASE_CAPACITY;
    r->group_len = 0;
    r->group_list = malloc(sizeof(struct bnf_group*) * GROUP_BASE_CAPACITY);
    return r;
}

void rule_append(struct bnf_rule *r, struct bnf_group *g)
{
    if (r->group_len == r->group_capacity - 1) {
        void *nptr = realloc(r->group_list,
            sizeof(struct bnf_group * ) * (r->group_capacity + GROUP_GROW));

        if (!nptr) {
            ALLOC_ERROR("rule cannot be reallocated");
        }

        r->group_list = nptr;
    }
    r->group_list[r->group_len] = g;
    r->group_len++;

}

struct bnf_group *group_create()
{
    struct bnf_group *g = calloc(1, sizeof(struct bnf_group));
    g->atom_capacity = GROUP_BASE_CAPACITY;
    g->atom_len = 0;
    g->atom_list = malloc(sizeof(struct bnf_atom) * GROUP_BASE_CAPACITY);
    if (!g->atom_list) {
        ALLOC_ERROR("group cannot be created\n");
    }

    return g;
}

void group_append(struct bnf_group *g, struct bnf_atom *a)
{
    if (g->atom_len == g->atom_capacity - 1) {
        struct bnf_atom *nptr = realloc(g->atom_list, 
                sizeof(struct bnf_atom) * (g->atom_capacity + GROUP_GROW));
        
        if (!nptr) {
            ALLOC_ERROR("group cannot be reallocated\n");
        }

        g->atom_list = nptr;
        g->atom_capacity += GROUP_GROW;
    }

    g->atom_list[g->atom_len].str = a->str;
    g->atom_list[g->atom_len].strl = a->strl;
    g->atom_list[g->atom_len].terminal= a->terminal;

    g->atom_len++;
}

struct bnf_atom *atom_create(int terminal, const char *s, int len)
{
    struct bnf_atom *a = calloc(1, sizeof(struct bnf_atom));

    if (!a) {
        ALLOC_ERROR("atom cannot be created\n");
        return NULL;
    }

    a->terminal = terminal;
    a->str = copy_str_malloc(len, s);

    return a;    
}
