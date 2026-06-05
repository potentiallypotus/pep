#include "lexer.h"
#include "parser.h"
#include "pep_error.h"
#include <stdio.h>
#include <stdlib.h>

void printAST(ASTnode *node, int depth) {
  if (node == NULL)
    return;

  // Print indentation based on current depth
  for (int i = 0; i < depth; i++)
    printf("  ");

  switch (node->type) {
  case AST_LITERAL:
    printf("LITERAL: ");
    // Here is where you use your ValueType to format correctly
    if (node->data.Literal.val.type == VAL_INT64) {
      printf("%lld\n", node->data.Literal.val.as.as_int64);
    }
    // Add other types as you implement them!
    break;

  case AST_BINARY:
    printf("BINARY OP: %d\n", node->data.Binary.op);
    printAST(node->data.Binary.left, depth + 1);
    printAST(node->data.Binary.right, depth + 1);
    break;

  case AST_VAR_DECL:
    printf("VAR DECL: %s\n", node->data.Var_decl.name);
    printAST(node->data.Var_decl.init, depth + 1);
    break;
  }
}
void doLine(char *line, size_t size, int line_num) {
  if (size != 0 && line[size - 1] == '\n')
    line[--size] = '\0';
  pep_source_line = line;
  struct token *tok = lex(line, line_num);
  ASTnode *root = parse(tok);
  Value val = eval(root);
  printf("%s\n", val_tostring(val));
}

int main(void) {
  char *line = NULL;
  size_t lineLen = 0;
  ssize_t bytesRead;
  int line_num = 1;

  pep_jmp_set = 1;
  while (1) {
    printf("pep >> ");
    bytesRead = getline(&line, &lineLen, stdin);
    if (bytesRead < 0) {
      free(line);
      return 1;
    }
    if (setjmp(pep_jmp) == 0)
      doLine(line, bytesRead, line_num++);
  }
  free(line);
}
