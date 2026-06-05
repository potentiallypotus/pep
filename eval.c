#include "stdio.h"
#include "stdlib.h"
#include "eval.h"
#include "lexer.h"
#include "parser.h"
#include "pep_error.h"

#define NUMERIC_BINOP(name, op) \
static Value name(Value left, Value right){ \
	Value val; \
	switch (left.type) { \
		case VAL_INT8: \
			val.type = VAL_INT8; \
			val.as.as_int8 = left.as.as_int8 op right.as.as_int8; \
		break; \
		case VAL_INT16: \
			val.type = VAL_INT16; \
			val.as.as_int16 = left.as.as_int16 op right.as.as_int16; \
		break; \
		case VAL_INT32: \
			val.type = VAL_INT32; \
			val.as.as_int32 = left.as.as_int32 op right.as.as_int32; \
		break; \
		case VAL_INT64: \
			val.type = VAL_INT64; \
			val.as.as_int64 = left.as.as_int64 op right.as.as_int64; \
		break; \
		case VAL_UINT8: \
			val.type = VAL_UINT8; \
			val.as.as_uint8 = left.as.as_uint8 op right.as.as_uint8; \
		break; \
		case VAL_UINT16: \
			val.type = VAL_UINT16; \
			val.as.as_uint16 = left.as.as_uint16 op right.as.as_uint16; \
		break; \
		case VAL_UINT32: \
			val.type = VAL_UINT32; \
			val.as.as_uint32 = left.as.as_uint32 op right.as.as_uint32; \
		break; \
		case VAL_UINT64: \
			val.type = VAL_UINT64; \
			val.as.as_uint64 = left.as.as_uint64 op right.as.as_uint64; \
		break; \
		case VAL_F16: \
			val.type = VAL_F16; \
			val.as.as_f16 = left.as.as_f16 op right.as.as_f16; \
		break; \
		case VAL_F32: \
			val.type = VAL_F32; \
			val.as.as_f32 = left.as.as_f32 op right.as.as_f32; \
		break; \
		case VAL_F64: \
			val.type = VAL_F64; \
			val.as.as_f64 = left.as.as_f64 op right.as.as_f64; \
		break; \
		default: \
			pep_error((PepError){ .message = #name "() unimplemented type" }); \
	} \
	return val; \
}

#define INT_BINOP(name, op) \
static Value name(Value left, Value right){ \
	Value val; \
	switch (left.type) { \
		case VAL_INT8: \
			val.type = VAL_INT8; \
			val.as.as_int8 = left.as.as_int8 op right.as.as_int8; \
		break; \
		case VAL_INT16: \
			val.type = VAL_INT16; \
			val.as.as_int16 = left.as.as_int16 op right.as.as_int16; \
		break; \
		case VAL_INT32: \
			val.type = VAL_INT32; \
			val.as.as_int32 = left.as.as_int32 op right.as.as_int32; \
		break; \
		case VAL_INT64: \
			val.type = VAL_INT64; \
			val.as.as_int64 = left.as.as_int64 op right.as.as_int64; \
		break; \
		case VAL_UINT8: \
			val.type = VAL_UINT8; \
			val.as.as_uint8 = left.as.as_uint8 op right.as.as_uint8; \
		break; \
		case VAL_UINT16: \
			val.type = VAL_UINT16; \
			val.as.as_uint16 = left.as.as_uint16 op right.as.as_uint16; \
		break; \
		case VAL_UINT32: \
			val.type = VAL_UINT32; \
			val.as.as_uint32 = left.as.as_uint32 op right.as.as_uint32; \
		break; \
		case VAL_UINT64: \
			val.type = VAL_UINT64; \
			val.as.as_uint64 = left.as.as_uint64 op right.as.as_uint64; \
		break; \
		default: \
			pep_error((PepError){ .message = #name "() unimplemented type" }); \
	} \
	return val; \
}

#define CMP_BINOP(name, op) \
static Value name(Value left, Value right){ \
	Value val; \
	val.type = VAL_INT64; \
	switch (left.type) { \
		case VAL_INT8: \
			val.as.as_int64 = left.as.as_int8 op right.as.as_int8; \
		break; \
		case VAL_INT16: \
			val.as.as_int64 = left.as.as_int16 op right.as.as_int16; \
		break; \
		case VAL_INT32: \
			val.as.as_int64 = left.as.as_int32 op right.as.as_int32; \
		break; \
		case VAL_INT64: \
			val.as.as_int64 = left.as.as_int64 op right.as.as_int64; \
		break; \
		case VAL_UINT8: \
			val.as.as_int64 = left.as.as_uint8 op right.as.as_uint8; \
		break; \
		case VAL_UINT16: \
			val.as.as_int64 = left.as.as_uint16 op right.as.as_uint16; \
		break; \
		case VAL_UINT32: \
			val.as.as_int64 = left.as.as_uint32 op right.as.as_uint32; \
		break; \
		case VAL_UINT64: \
			val.as.as_int64 = (int64_t)(left.as.as_uint64 op right.as.as_uint64); \
		break; \
		case VAL_F16: \
			val.as.as_int64 = left.as.as_f16 op right.as.as_f16; \
		break; \
		case VAL_F32: \
			val.as.as_int64 = left.as.as_f32 op right.as.as_f32; \
		break; \
		case VAL_F64: \
			val.as.as_int64 = left.as.as_f64 op right.as.as_f64; \
		break; \
		default: \
			pep_error((PepError){ .message = #name "() unimplemented type" }); \
	} \
	return val; \
}

NUMERIC_BINOP(evalPlus, +)
NUMERIC_BINOP(evalMinus, -)
NUMERIC_BINOP(evalStar, *)

static Value evalSlash(Value left, Value right){
	Value val;
	switch (left.type) {
		case VAL_INT8:
			if (right.as.as_int8 == 0) { pep_error((PepError){ .message = "division by zero" }); }
			val.type = VAL_INT8;   val.as.as_int8   = left.as.as_int8   / right.as.as_int8;   break;
		case VAL_INT16:
			if (right.as.as_int16 == 0) { pep_error((PepError){ .message = "division by zero" }); }
			val.type = VAL_INT16;  val.as.as_int16  = left.as.as_int16  / right.as.as_int16;  break;
		case VAL_INT32:
			if (right.as.as_int32 == 0) { pep_error((PepError){ .message = "division by zero" }); }
			val.type = VAL_INT32;  val.as.as_int32  = left.as.as_int32  / right.as.as_int32;  break;
		case VAL_INT64:
			if (right.as.as_int64 == 0) { pep_error((PepError){ .message = "division by zero" }); }
			val.type = VAL_INT64;  val.as.as_int64  = left.as.as_int64  / right.as.as_int64;  break;
		case VAL_UINT8:
			if (right.as.as_uint8 == 0) { pep_error((PepError){ .message = "division by zero" }); }
			val.type = VAL_UINT8;  val.as.as_uint8  = left.as.as_uint8  / right.as.as_uint8;  break;
		case VAL_UINT16:
			if (right.as.as_uint16 == 0) { pep_error((PepError){ .message = "division by zero" }); }
			val.type = VAL_UINT16; val.as.as_uint16 = left.as.as_uint16 / right.as.as_uint16; break;
		case VAL_UINT32:
			if (right.as.as_uint32 == 0) { pep_error((PepError){ .message = "division by zero" }); }
			val.type = VAL_UINT32; val.as.as_uint32 = left.as.as_uint32 / right.as.as_uint32; break;
		case VAL_UINT64:
			if (right.as.as_uint64 == 0) { pep_error((PepError){ .message = "division by zero" }); }
			val.type = VAL_UINT64; val.as.as_uint64 = left.as.as_uint64 / right.as.as_uint64; break;
		case VAL_F16:
			if (right.as.as_f16 == 0.0f) { pep_error((PepError){ .message = "division by zero" }); }
			val.type = VAL_F16;    val.as.as_f16    = left.as.as_f16    / right.as.as_f16;    break;
		case VAL_F32:
			if (right.as.as_f32 == 0.0f) { pep_error((PepError){ .message = "division by zero" }); }
			val.type = VAL_F32;    val.as.as_f32    = left.as.as_f32    / right.as.as_f32;    break;
		case VAL_F64:
			if (right.as.as_f64 == 0.0) { pep_error((PepError){ .message = "division by zero" }); }
			val.type = VAL_F64;    val.as.as_f64    = left.as.as_f64    / right.as.as_f64;    break;
		default:
			pep_error((PepError){ .message = "evalSlash() unimplemented type" });
	}
	return val;
}
static Value evalPercent(Value left, Value right){
	Value val;
	switch (left.type) {
		case VAL_INT8:
			if (right.as.as_int8 == 0) { pep_error((PepError){ .message = "modulo by zero" }); }
			val.type = VAL_INT8;   val.as.as_int8   = left.as.as_int8   % right.as.as_int8;   break;
		case VAL_INT16:
			if (right.as.as_int16 == 0) { pep_error((PepError){ .message = "modulo by zero" }); }
			val.type = VAL_INT16;  val.as.as_int16  = left.as.as_int16  % right.as.as_int16;  break;
		case VAL_INT32:
			if (right.as.as_int32 == 0) { pep_error((PepError){ .message = "modulo by zero" }); }
			val.type = VAL_INT32;  val.as.as_int32  = left.as.as_int32  % right.as.as_int32;  break;
		case VAL_INT64:
			if (right.as.as_int64 == 0) { pep_error((PepError){ .message = "modulo by zero" }); }
			val.type = VAL_INT64;  val.as.as_int64  = left.as.as_int64  % right.as.as_int64;  break;
		case VAL_UINT8:
			if (right.as.as_uint8 == 0) { pep_error((PepError){ .message = "modulo by zero" }); }
			val.type = VAL_UINT8;  val.as.as_uint8  = left.as.as_uint8  % right.as.as_uint8;  break;
		case VAL_UINT16:
			if (right.as.as_uint16 == 0) { pep_error((PepError){ .message = "modulo by zero" }); }
			val.type = VAL_UINT16; val.as.as_uint16 = left.as.as_uint16 % right.as.as_uint16; break;
		case VAL_UINT32:
			if (right.as.as_uint32 == 0) { pep_error((PepError){ .message = "modulo by zero" }); }
			val.type = VAL_UINT32; val.as.as_uint32 = left.as.as_uint32 % right.as.as_uint32; break;
		case VAL_UINT64:
			if (right.as.as_uint64 == 0) { pep_error((PepError){ .message = "modulo by zero" }); }
			val.type = VAL_UINT64; val.as.as_uint64 = left.as.as_uint64 % right.as.as_uint64; break;
		default:
			pep_error((PepError){ .message = "evalPercent() unimplemented type" });
	}
	return val;
}
INT_BINOP(evalAmpersand, &)
INT_BINOP(evalPipe, |)
CMP_BINOP(evalGT, >)
CMP_BINOP(evalGTE, >=)
CMP_BINOP(evalLT, <)
CMP_BINOP(evalLTE, <=)
CMP_BINOP(evalEqualEqual, ==)
CMP_BINOP(evalNotEqual, !=)

static Value evalBinary(tokenType op, Value left, Value right){
	if (left.type != right.type) {
		pep_error((PepError){ .message = "type mismatch: operands must be the same type" });
	}
	switch (op) {
		case TOKEN_PLUS:         return evalPlus(left, right);
		case TOKEN_MINUS:        return evalMinus(left, right);
		case TOKEN_STAR:         return evalStar(left, right);
		case TOKEN_SLASH:        return evalSlash(left, right);
		case TOKEN_PERCENT:      return evalPercent(left, right);
		case TOKEN_AMPERSAND:    return evalAmpersand(left, right);
		case TOKEN_PIPE:         return evalPipe(left, right);
		case TOKEN_GT:           return evalGT(left, right);
		case TOKEN_GTE:          return evalGTE(left, right);
		case TOKEN_LT:           return evalLT(left, right);
		case TOKEN_LTE:          return evalLTE(left, right);
		case TOKEN_EQUAL_EQUAL:  return evalEqualEqual(left, right);
		case TOKEN_BANG_EQUAL:   return evalNotEqual(left, right);
		default: {
			static char errbuf[64];
			snprintf(errbuf, sizeof(errbuf), "unimplemented operator: %s", tokentype_tostring(op));
			pep_error((PepError){ .message = errbuf });
		}
	}
}

Value eval(ASTnode* root){
	if(root->type ==AST_LITERAL){
		return root->data.Literal.val;
	}
	if(root->type == AST_VAR_DECL){
		Value val;
		return val;
	}
	//binary
	ASTnode* left, *right;
	left = root->data.Binary.left;
	right = root->data.Binary.right;
	return evalBinary(root->data.Binary.op, eval(left), eval(right));
}
