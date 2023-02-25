# BNF Tester

## Espanol / Spanish

BNF Tester genera casos de prueba para una gramatica BNF dada. Para ejemplos,
ve la carpeta `grammar_tests`.

Importante notar que cada regla debe terminar con un `;`. Se pueden
escribir comentarios usando el caracter `#`.

### Compilar

`$ gcc main.c parser.c repr.c lex.c -o bnf_tester`

### Uso

`$ ./bnf_tester grammar_file.bnf`