#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    keyword,
    ident,
    semicolon,
    equals,
    open_param,
    close_param,
    binary_op
} token_type_t;

typedef struct {
    size_t line;
    size_t start;
    size_t end;
} token_pos_t;

typedef struct token {
    token_type_t type;
    char* content;
    token_pos_t pos;
    struct token* next;
} token_t;

token_type_t is_keyword(const char* word);
void lexer_push(token_type_t type, const char* content, token_pos_t pos);
token_pos_t new_token_pos(size_t line, size_t start_pos, size_t end_pos);
token_t* lexer_parse(const char* source);
void lexer_free();

#endif
