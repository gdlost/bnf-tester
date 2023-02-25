#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "data.h"

static char line[LINE_MAX];
static int cc; // current character
static char last_str[64];
static int ls_len; // last string len
static int last_token;

static FILE *lfp = NULL;

// Ya que se usa lex() para el peek, es necesario guardar este
// token en el stream, es decir, hacer como si no se hubiese pedido
static int lex_pending = 0;
static int lex_pending_token = 0; 

static int line_nr = 0;

static int lex();

int lex_get_last_token()
{
    return last_token;
}

const char *lex_get_last_string() {
    return last_str;
}
int lex_get_last_string_length() {
    return ls_len;
}

// Esta funcion verifica si aun queda entrada
int lex_input()
{
    if (lex_peek() == EOF_S) {
        return 0;
    }
    return 1;
}

// esta funcion se encarga de leer la primera linea del archivo
int lex_init(FILE *fp)
{
    lfp = fp;
    fgets(line, LINE_MAX, fp);
    cc = 0;
    line_nr++;
    printf("LINE %d\n", line_nr);
    return 0;
}

int lex_getline()
{
    if (!fgets(line, LINE_MAX, lfp)) {
        return 0;
    }
    cc = 0;
    line_nr++;
    printf("LINE %d\n", line_nr);
    return 1;
}

int lex_advance()
{
    int t;

    if (lex_pending) {
        lex_pending = 0;
        t = lex_pending_token;
        lex_pending_token = INVALID_S;
        last_token = t;
        return t;
    }

    t = lex();
    while (t == SPACE_S || t == EOL_S) {
        if (t == EOL_S) {
            if (!lex_getline()) {
                t = EOF_S;
            } else {
                t = lex();
            }
        } else {
            t = lex();
        }
    }
    
    last_token = t;
    return t;
}

int lex_peek()
{
    if (lex_pending) {
        return lex_pending_token;
    }

    int t;
    t = lex();
    while (t == SPACE_S || t == EOL_S) {
        if (t == EOL_S) {
            if (!lex_getline()) {
                t = EOF_S;
            } else {
                t = lex();
            }
        } else {
            t = lex();
        }
    }
 
    lex_pending = 1;
    lex_pending_token = t;
    return t;
}

static int lex() {
    int c = 0;
    const char *p = line;
    int i = cc;
    int adv = 1;
    char strdelim_c = '"';

    switch (p[i]) {
        case '|': c = OR_S; break;
        case '<': c = LEFT_ARROW_S; break;
        case '>': c = RIGHT_ARROW_S; break;
        case ';': c = SEMI_COLON_S; break;
        case ':': {
            if (p[i+1] == ':' && p[i+2] == '=') {
                c = DEFINITION_S;
                adv = 3;
            } 
            break;
        }
        case '\t':
        case ' ': {
            int sp = 0;
            while (p[i+sp] == ' ' || p[i+sp] == '\t') {
                sp++;
            }
            adv = sp;
            c = SPACE_S;
            break;
        }
        // # = comment
        case '#':
        case '\n': c = EOL_S; break;
        case '\0': c = EOF_S; break;
        // Lo mas probable, es que fgets de MinGW elimina los CR de los strings
        // Se asume que es un CR
        case '\r': c = EOL_S; break;
        case '\'': {
            strdelim_c = '\'';
        }
        case '"': {
            int len = 0;
            while (p[i+1+len] != strdelim_c) {
                //if (!strchr("| !#$%&()*+,_./:;=?@[\]^", p[i+1+len])) {
                    // throw error!!
                //}
                // En caso de que se llegue al final de la linea
                if (p[i+1+len] == '\n' || p[i+1+len] == '\0') {
                    // Error: string sin terminar
                }
                len++;
            }
            memset(last_str, 0, ls_len);
            strncpy(last_str, p + i + 1, len);
            adv = len + 2;
            ls_len = len;
            c = STRING_S;
            break;
        }
        default: {
            if (isalpha(p[i])) {
                int a = 1;
                while (isalnum(p[i+a]) || strchr("_-", p[i+a])) {
                    a++;
                }
                c = IDENTIFIER_S;
                adv = a;
                memset(last_str, 0, ls_len);
                ls_len = a;
                strncpy(last_str, p + i, a);
            } else {
                // throw error
                printf("Error: Algo llego hasta aca, y es un %c (%d)\n", p[i], p[i]);
            }
            break;
        }
    }    

    cc += adv;
    return c;
}

int lex_match(int d)
{
    if (lex_peek() == d) {
        lex_advance();
        return 1;
    }
    return 0;
}
