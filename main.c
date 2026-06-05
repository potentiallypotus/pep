#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

void doLine(char* line, size_t size) {
	if (size != 0 && line[size-1] == '\n')
		line[--size] = '\0';
	struct token* tok = lex(line);
	struct token* cur = tok;
	while (cur != NULL) {
		struct token* next = cur->next;
		printf("[%d] %.*s\n", cur->type, cur->len, cur->value);
		free(cur);
		cur = next;
	}
}

int main(void) {
	char* line = NULL;
	size_t lineLen = 0;
	ssize_t bytesRead;

	while (1) {
		printf("pep >> ");
		bytesRead = getline(&line, &lineLen, stdin);
		if (bytesRead < 0) {
			free(line);
			return 1;
		}
		doLine(line, bytesRead);

	}
	free(line);
}
