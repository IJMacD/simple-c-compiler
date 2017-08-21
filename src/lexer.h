#ifndef LEXER_H
#define LEXER_H

#define TOKEN_NAME    1
#define TOKEN_NUMBER  2
#define TOKEN_STRING  3
#define TOKEN_OPERATOR  4

#define DEFAULT_TOKEN_COUNT 100

typedef struct token_struct {
  int type;
  char *value;
} token;

typedef struct token_list_struct {
  int length;
  token list[];
} token_list;

token_list* lexer(const char *);
void free_tokens(token_list *);

#endif
