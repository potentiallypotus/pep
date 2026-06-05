#ifndef PEP_ERROR_H
#define PEP_ERROR_H
#include <setjmp.h>

typedef struct {
    const char* message;
    int         line;
    int         col;
    const char* source_line;
} PepError;

extern jmp_buf      pep_jmp;
extern int          pep_jmp_set;
extern const char*  pep_source_line;

_Noreturn void pep_error(PepError err);

#endif
