#include "pep_error.h"
#include <stdio.h>
#include <stdlib.h>

jmp_buf     pep_jmp;
int         pep_jmp_set  = 0;
const char* pep_source_line = NULL;

_Noreturn void pep_error(PepError err) {
    if (err.line > 0)
        fprintf(stderr, "[%d:%d] error: %s\n", err.line, err.col, err.message);
    else
        fprintf(stderr, "error: %s\n", err.message);

    const char* src = err.source_line ? err.source_line : pep_source_line;
    if (src) {
        fprintf(stderr, "  %s\n", src);
        if (err.col > 0)
            fprintf(stderr, "  %*s^\n", err.col - 1, "");
    }

    if (pep_jmp_set)
        longjmp(pep_jmp, 1);
    else
        exit(1);
}
