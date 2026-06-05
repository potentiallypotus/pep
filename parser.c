#include "stdlib.h"
#include "pep_error.h"
#include "string.h"
#include "stdbool.h"
#include "stdio.h"
#include "lexer.h"
#include "parser.h"

struct token* current;
ASTnode* newASTnode(){
	return malloc(sizeof(ASTnode));
}
ASTnode* createBinaryNode(tokenType op, ASTnode* left, ASTnode* right){
	ASTnode* new = newASTnode();
	new->type = AST_BINARY;
	new->data.Binary.op = op;
	new->data.Binary.left = left;
	new->data.Binary.right = right;
	return new;
}
Value convertNumTok(struct token* tok){
	Value val;
	const char* s = tok->value;
	if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
		val.type = VAL_INT64;
		val.as.as_int64 = (int64_t)strtoll(s, NULL, 16);
	} else if (s[0] == '0' && (s[1] == 'b' || s[1] == 'B')) {
		val.type = VAL_INT64;
		val.as.as_int64 = (int64_t)strtoll(s + 2, NULL, 2);
	} else if (s[tok->len - 1] == 'f' || s[tok->len - 1] == 'F') {
		val.type = VAL_F32;
		val.as.as_f32 = strtof(s, NULL);
	} else if (memchr(s, '.', tok->len)) {
		val.type = VAL_F64;
		val.as.as_f64 = strtod(s, NULL);
	} else {
		val.type = VAL_INT64;
		val.as.as_int64 = (int64_t)atoll(s);
	}
	return val;
}
Value createVal(struct token* tok){
	Value val;
	switch (tok->type) {
		case(TOKEN_NUMBER) :
			val = convertNumTok(tok);
			break;
		default: {
			static char errbuf[128];
			snprintf(errbuf, sizeof(errbuf), "unexpected token '%.*s' (%s)",
			         tok->len, tok->value, tokentype_tostring(tok->type));
			pep_error((PepError){ .message = errbuf, .line = tok->line, .col = tok->col });
		}
	}
	return val;
}

ASTnode* createLiteralNode(struct token* tok){
	Value val = createVal(tok);
	ASTnode * new = newASTnode();
	new->type = AST_LITERAL;
	new->data.Literal.val = val;
	return new;
}

void freeASTnode(ASTnode* node){
	if(!node)return;
	if(node->type == AST_BINARY){
		freeASTnode(node->data.Binary.left);
		freeASTnode(node->data.Binary.right);
	}else if (node->type == AST_VAR_DECL){
		freeASTnode(node->data.Var_decl.init);
	}
	free(node);
	return;
}
void advance(){
	if (current != NULL){
		current = current->next;
	}
}
bool check(tokenType type){
	if (current == NULL) return false;
	return current->type == type;
}
ASTnode* parseFactor(){
	if (check(TOKEN_NUMBER)){
		struct token* number = current;
		advance();
		return createLiteralNode(number);
	}
	pep_error((PepError){
		.message = "expected expression",
		.line    = current ? current->line : 0,
		.col     = current ? current->col  : 0,
	});
}
ASTnode* parseTerms(){
	ASTnode* left = parseFactor();
	while (check(TOKEN_STAR) || check(TOKEN_SLASH)){
		tokenType op = current->type;
		advance();
		ASTnode* right = parseFactor();
		left = createBinaryNode(op, left, right);
	}
	return left;
}
ASTnode* parseExpr(){
	ASTnode* left = parseTerms();
	while (check(TOKEN_PLUS) || check(TOKEN_MINUS)){
		tokenType op = current->type;
		advance();
		ASTnode* right = parseTerms();
		left = createBinaryNode(op, left, right);
	}
	return left;
}
ASTnode* parse(struct token* tok){
	current = tok;
	return parseExpr();
}

char* val_tostring(Value val){
	static char buffer[128];
	switch (val.type) {
		case VAL_INT8:   snprintf(buffer, sizeof(buffer), "%d",   (int)val.as.as_int8);   break;
		case VAL_INT16:  snprintf(buffer, sizeof(buffer), "%d",   (int)val.as.as_int16);  break;
		case VAL_INT32:  snprintf(buffer, sizeof(buffer), "%d",   val.as.as_int32);       break;
		case VAL_INT64:  snprintf(buffer, sizeof(buffer), "%lld", val.as.as_int64);       break;
		case VAL_UINT8:  snprintf(buffer, sizeof(buffer), "%u",   (unsigned)val.as.as_uint8);  break;
		case VAL_UINT16: snprintf(buffer, sizeof(buffer), "%u",   (unsigned)val.as.as_uint16); break;
		case VAL_UINT32: snprintf(buffer, sizeof(buffer), "%u",   val.as.as_uint32);      break;
		case VAL_UINT64: snprintf(buffer, sizeof(buffer), "%llu", val.as.as_uint64);      break;
		case VAL_F16:    snprintf(buffer, sizeof(buffer), "%g",   (double)val.as.as_f16); break;
		case VAL_F32:    snprintf(buffer, sizeof(buffer), "%g",   (double)val.as.as_f32); break;
		case VAL_F64:    snprintf(buffer, sizeof(buffer), "%g",   val.as.as_f64);         break;
		case VAL_STRING: snprintf(buffer, sizeof(buffer), "%s",   val.as.as_string);      break;
		default: {
			static char errbuf[64];
			snprintf(errbuf, sizeof(errbuf), "val_tostring: unhandled value type %d", val.type);
			pep_error((PepError){ .message = errbuf });
		}
	}
	return buffer;
}


