#include "stdlib.h"
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
	val.type = VAL_INT64;
	val.as.as_int64 = (int64_t)(atoi(tok->value));
	return val;
}
Value createVal(struct token* tok){
	Value val;
	switch (tok->type) {
		case(TOKEN_NUMBER) :
			val = convertNumTok(tok);
			break;
		default:
			fprintf(stderr, "createVal(token): token type not implemented: Token = {type:%d, val:%s}\n", tok->type, tok->value);
			exit(1);
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


	}
	return val;
}
ASTnode* parseTerms(){
	ASTnode* left = parseFactor();

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
Value eval(ASTnode* root){
	Value val;
	return val;
}

