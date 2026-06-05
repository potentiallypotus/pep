#include "lexer.h"
#include <stdlib.h>
#include <ctype.h>

static int has_compound(enum tokenType t) {
	return t == TOKEN_PLUS || t == TOKEN_MINUS || t == TOKEN_STAR
		|| t == TOKEN_SLASH || t == TOKEN_PERCENT || t == TOKEN_AMPERSAND
		|| t == TOKEN_PIPE || t == TOKEN_GT || t == TOKEN_LT || t == TOKEN_EQUAL
		|| t == TOKEN_BANG;
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
	case '!': newTok.type = TOKEN_BANG;      valid = 1; break;
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

const char* tokentype_tostring(enum tokenType t) {
	switch (t) {
		case TOKEN_LEFT_PAREN:    return "TOKEN_LEFT_PAREN";
		case TOKEN_RIGHT_PAREN:   return "TOKEN_RIGHT_PAREN";
		case TOKEN_LEFT_BRACE:    return "TOKEN_LEFT_BRACE";
		case TOKEN_RIGHT_BRACE:   return "TOKEN_RIGHT_BRACE";
		case TOKEN_COMMA:         return "TOKEN_COMMA";
		case TOKEN_DOT:           return "TOKEN_DOT";
		case TOKEN_SEMICOLON:     return "TOKEN_SEMICOLON";
		case TOKEN_PLUS:          return "TOKEN_PLUS";
		case TOKEN_PLUS_EQUAL:    return "TOKEN_PLUS_EQUAL";
		case TOKEN_MINUS:         return "TOKEN_MINUS";
		case TOKEN_MINUS_EQUAL:   return "TOKEN_MINUS_EQUAL";
		case TOKEN_STAR:          return "TOKEN_STAR";
		case TOKEN_STAR_EQUAL:    return "TOKEN_STAR_EQUAL";
		case TOKEN_SLASH:         return "TOKEN_SLASH";
		case TOKEN_SLASH_EQUAL:   return "TOKEN_SLASH_EQUAL";
		case TOKEN_PERCENT:       return "TOKEN_PERCENT";
		case TOKEN_PERCENT_EQUAL: return "TOKEN_PERCENT_EQUAL";
		case TOKEN_AMPERSAND:     return "TOKEN_AMPERSAND";
		case TOKEN_AND_EQUAL:     return "TOKEN_AND_EQUAL";
		case TOKEN_PIPE:          return "TOKEN_PIPE";
		case TOKEN_OR_EQUAL:      return "TOKEN_OR_EQUAL";
		case TOKEN_GT:            return "TOKEN_GT";
		case TOKEN_GTE:           return "TOKEN_GTE";
		case TOKEN_LT:            return "TOKEN_LT";
		case TOKEN_LTE:           return "TOKEN_LTE";
		case TOKEN_EQUAL:         return "TOKEN_EQUAL";
		case TOKEN_EQUAL_EQUAL:   return "TOKEN_EQUAL_EQUAL";
		case TOKEN_BANG:          return "TOKEN_BANG";
		case TOKEN_BANG_EQUAL:    return "TOKEN_BANG_EQUAL";
		case TOKEN_IDENTIFIER:    return "TOKEN_IDENTIFIER";
		case TOKEN_NUMBER:        return "TOKEN_NUMBER";
		case TOKEN_STRING:        return "TOKEN_STRING";
		case TOKEN_IF:            return "TOKEN_IF";
		case TOKEN_ELSE:          return "TOKEN_ELSE";
		case TOKEN_RETURN:        return "TOKEN_RETURN";
		case TOKEN_INT:           return "TOKEN_INT";
		case TOKEN_EOF:           return "TOKEN_EOF";
		case TOKEN_ERROR:         return "TOKEN_ERROR";
		default:                  return "TOKEN_UNKNOWN";
	}
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
