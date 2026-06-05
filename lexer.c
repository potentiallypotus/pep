#include "lexer.h"
#include <stdlib.h>
#include <ctype.h>

static int has_compound(enum tokenType t) {
	return t == TOKEN_PLUS || t == TOKEN_MINUS || t == TOKEN_STAR
		|| t == TOKEN_SLASH || t == TOKEN_PERCENT || t == TOKEN_AMPERSAND
		|| t == TOKEN_PIPE || t == TOKEN_GT || t == TOKEN_LT || t == TOKEN_EQUAL;
}

int is_identifier_part(char c) {
	return isalnum(c) || c == '_';
}

struct token* getToken(char** line) {
	while (isspace(**line))
		(*line)++;

	if (!**line) return NULL;

	char* start = *line;
	struct token newTok;
	newTok.len = 1;
	newTok.next = NULL;
	newTok.value = start;
	int valid = 0;
	char c = *start;
	(*line)++;

	switch (c) {
	case '(': newTok.type = TOKEN_LEFT_PAREN;  valid = 1; break;
	case ')': newTok.type = TOKEN_RIGHT_PAREN; valid = 1; break;
	case '{': newTok.type = TOKEN_LEFT_BRACE;  valid = 1; break;
	case '}': newTok.type = TOKEN_RIGHT_BRACE; valid = 1; break;
	case ',': newTok.type = TOKEN_COMMA;       valid = 1; break;
	case '.': newTok.type = TOKEN_DOT;         valid = 1; break;
	case ';': newTok.type = TOKEN_SEMICOLON;   valid = 1; break;
	case '+': newTok.type = TOKEN_PLUS;      valid = 1; break;
	case '-': newTok.type = TOKEN_MINUS;     valid = 1; break;
	case '*': newTok.type = TOKEN_STAR;      valid = 1; break;
	case '/': newTok.type = TOKEN_SLASH;     valid = 1; break;
	case '%': newTok.type = TOKEN_PERCENT;   valid = 1; break;
	case '&': newTok.type = TOKEN_AMPERSAND; valid = 1; break;
	case '|': newTok.type = TOKEN_PIPE;      valid = 1; break;
	case '<': newTok.type = TOKEN_LT;        valid = 1; break;
	case '>': newTok.type = TOKEN_GT;        valid = 1; break;
	case '=': newTok.type = TOKEN_EQUAL;     valid = 1; break;
	case '"':
		newTok.type = TOKEN_STRING;
		while (**line != '"' && **line != '\0') {
			if (**line == '\\') {
				(*line)++;
				newTok.len++;
				if (**line != '\0') {
					(*line)++;
					newTok.len++;
				}
			} else {
				(*line)++;
				newTok.len++;
			}
		}
		if (**line == '"') {
			(*line)++;
			newTok.len++;
		}
		valid = 1;
		break;
	default:
		if (isalpha(c) || c == '_') {
			newTok.type = TOKEN_IDENTIFIER;
			while (is_identifier_part(**line)) {
				(*line)++;
				newTok.len++;
			}
			valid = 1;
		} else if (isdigit(c)) {
			newTok.type = TOKEN_NUMBER;
			while (isdigit(**line)) {
				(*line)++;
				newTok.len++;
			}
			valid = 1;
		}
		break;
	}

	if (valid && has_compound(newTok.type) && **line == '=') {
		newTok.type++;
		(*line)++;
		newTok.len++;
	}

	if (!valid) return NULL;

	struct token* tok = malloc(sizeof(struct token));
	*tok = newTok;
	return tok;
}

struct token* lex(char* line) {
	char* pos = line;
	struct token* head = NULL;
	struct token* tail = NULL;
	struct token* newTok = NULL;

	while ((newTok = getToken(&pos))) {
		if (head == NULL) {
			head = newTok;
			tail = newTok;
			continue;
		}
		tail->next = newTok;
		tail = newTok;
	}
	return head;
}
