#ifndef PARSER_H
#define PARSER_H
#include "stdint.h"
#include "lexer.h"


typedef enum {
    VAL_INT8, VAL_INT16, VAL_INT32, VAL_INT64,
    VAL_UINT8, VAL_UINT16, VAL_UINT32, VAL_UINT64,
    VAL_F16, VAL_F32, VAL_F64,
    VAL_STRING
} ValueType;

typedef struct {
    ValueType type;
    union {
        int8_t   as_int8;
        int16_t  as_int16;
        int32_t  as_int32;
        int64_t  as_int64;
        uint8_t  as_uint8;
        uint16_t as_uint16;
        uint32_t as_uint32;
        uint64_t as_uint64;
        _Float16 as_f16;
        float    as_f32;
        double   as_f64;
        char*    as_string;
    } as;
} Value;
typedef enum {AST_LITERAL, AST_BINARY, AST_VAR_DECL}ASTtype;
typedef struct ASTnode{
	ASTtype type;
	union{
		struct{Value val;}Literal;
		struct{tokenType op; struct ASTnode* left; struct ASTnode* right;}Binary;
		struct{char* name; struct ASTnode* init;}Var_decl;
	}data;
}ASTnode;

ASTnode* parse(struct token* tok);
Value eval(ASTnode*);
#endif
