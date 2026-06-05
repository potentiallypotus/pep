#ifndef LEXER_H
#define LEXER_H

typedef enum tokenType{
		// --- Punctuators ---
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN, TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
    TOKEN_COMMA, TOKEN_DOT, TOKEN_SEMICOLON,

    // --- Operators ---
    // Note: Assignment operators must immediately follow their base operator
    TOKEN_PLUS,          // +
    TOKEN_PLUS_EQUAL,    // +=
    TOKEN_MINUS,         // -
    TOKEN_MINUS_EQUAL,   // -=
    TOKEN_STAR,          // *
    TOKEN_STAR_EQUAL,    // *=
    TOKEN_SLASH,         // /
    TOKEN_SLASH_EQUAL,   // /=
    TOKEN_PERCENT,       // %
    TOKEN_PERCENT_EQUAL, // %=
    TOKEN_AMPERSAND,     // &
    TOKEN_AND_EQUAL,     // &=
    TOKEN_PIPE,          // |
    TOKEN_OR_EQUAL,      // |=
		TOKEN_GT,
		TOKEN_GTE,
		TOKEN_LT,
		TOKEN_LTE,
    TOKEN_EQUAL,         // =
    TOKEN_EQUAL_EQUAL,   // ==
    TOKEN_BANG,          // !
    TOKEN_BANG_EQUAL,    // !=

    // --- Literals ---
    TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_STRING,

    // --- Keywords ---
    TOKEN_IF, TOKEN_ELSE, TOKEN_RETURN, TOKEN_INT,

    // --- Housekeeping ---
    TOKEN_EOF, TOKEN_ERROR
}tokenType;

struct token{
	enum tokenType type;
	char* value;
	int len;
	struct token* next;
};

struct token* lex(char*);
const char* tokentype_tostring(enum tokenType t);
#endif

