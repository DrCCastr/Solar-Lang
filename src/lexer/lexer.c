#include "lexer.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>//a

#if defined(_WIN32)
    #include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
    #include <unistd.h>
#endif

#define NUM_KEYWORDS 1

static token_t* list = NULL;
static token_t* head = NULL;

static const char* keywords[NUM_KEYWORDS] = {
    "Var",
};

char* strdup_safe(const char* s) {
    char* copy = malloc(strlen(s) + 1);
    if (copy) strcpy(copy, s);
    return copy;
}

token_type_t is_keyword(const char* word) {
    for (size_t i = 0; i < NUM_KEYWORDS; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return keyword;
        }
    }
    return ident;
}

void lexer_push(token_type_t type, const char* content, token_pos_t pos) {
    token_t* new_token = (token_t*) malloc(sizeof(token_t));
    if (!new_token) {
        fprintf(stderr, "Internal error: Failure in alloc token memory.\n");
        exit(EXIT_FAILURE);
    }

    new_token->type = type;
    new_token->content = strdup_safe(content);
    new_token->pos = pos;
    new_token->next = NULL;

    if (!list) {
        head = new_token;
        list = new_token;
    } else {
        list->next = new_token;
        list = new_token;
    }
}

token_pos_t new_token_pos(size_t line, size_t start_pos, size_t end_pos) {
    token_pos_t pos;
    pos.line = line;
    pos.start = start_pos;
    pos.end = end_pos;
    return pos;
}

uint8_t valid_char(char chr) {
    return (chr == '\t' || chr == ' ' || isdigit(chr) || isalpha(chr) || chr == '_');
}

token_t* lexer_parse(const char* source) {
    if (!source) return NULL;

    list = NULL;
    head = NULL;

    size_t pos = 0;
    size_t line = 1;
    size_t len = strlen(source);

    for (size_t i = 0; i < len; i++) {
        char current = source[i];
        pos++;

        switch (current) {
            case ';': lexer_push(semicolon, ";", new_token_pos(line, pos, pos)); break;
            case '=': lexer_push(equals, "=", new_token_pos(line, pos, pos)); break;
            case '(': lexer_push(open_param, "(", new_token_pos(line, pos, pos)); break;
            case ')': lexer_push(close_param, ")", new_token_pos(line, pos, pos)); break;
            case '+': case '-': case '*': case '/':
                lexer_push(binary_op, (char[2]){current, '\0'}, new_token_pos(line, pos, pos));
                break;
            case '\n':
                line++;
                pos = 0;
                break;
            default:
                if (!valid_char(current)) {
                    fprintf(stderr, "Illegal character '%c' at %zu-%zu:%zu\n", current, line, pos, pos);
                    exit(EXIT_FAILURE);
                }
                break;
        }

        if (isdigit(current)) {
            size_t start_pos = pos;
            size_t start = i;

            while (i < len && isdigit(source[i])) {
                i++;
                pos++;
            }

            size_t num_len = i - start;
            char* content = (char*) malloc(num_len + 1);
            if (!content) {
                fprintf(stderr, "Error: Failure to alloc memory for number at %zu-%zu:%zu.\n", line, start_pos, pos);
                exit(EXIT_FAILURE);
            }

            strncpy(content, &source[start], num_len);
            content[num_len] = '\0';

            lexer_push(ident, content, new_token_pos(line, start_pos, pos - 1));
            free(content);

            i--;
        }

        if (isalpha(current)) {
            size_t start_pos = pos;
            size_t start = i;

            while (i < len && (isalnum(source[i]) || source[i] == '_')) {
                i++;
                pos++;
            }

            size_t str_len = i - start;
            char* content = (char*) malloc(str_len + 1);
            if (!content) {
                fprintf(stderr, "Error: Failure to alloc memory for identifier/keyword at %zu-%zu:%zu.\n", line, start_pos, pos);
                exit(EXIT_FAILURE);
            }

            strncpy(content, &source[start], str_len);
            content[str_len] = '\0';

            token_type_t type = is_keyword(content);
            lexer_push(type, content, new_token_pos(line, start_pos, pos - 1));

            free(content);
            i--;
        }
    }

    return head;
}

void lexer_free() {
    token_t* current = head;
    while (current) {
        token_t* temp = current;
        current = current->next;
        free(temp->content);
        free(temp);
    }
}
