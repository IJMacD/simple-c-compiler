#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

/*    LEXER     */
token_list* lexer(const char *source) {

  int index = 0;
  int len = strlen(source);

  token_list *tokens = malloc(sizeof(token_list) + DEFAULT_TOKEN_COUNT * sizeof(token));
  tokens->length = 0;

  while(index < len) {
    char c = source[index];

    if (tokens->length >= DEFAULT_TOKEN_COUNT) {
      fprintf(stderr, "Lexer Error: Token pool exhausted.\n");
      exit(-1);
    }

    if(c >= 'a' && c <= 'z') {
      token *t = &tokens->list[tokens->length++];

      t->type = TOKEN_NAME;

      int count = 1;
      int start = index;

      index++;

      while(source[index] >= 'a' && source[index] <= 'z' && index < len) {
        index++;
        count++;
      }

      t->value = malloc(count + 1);
      memcpy(t->value, &source[start], count);
      t->value[count] = '\0';

      continue;
    }


    if(c >= '0' && c <= '9') {
      token *t = &tokens->list[tokens->length++];

      t->type = TOKEN_NUMBER;

      int count = 1;
      int start = index;

      index++;

      while(source[index] >= '0' && source[index] <= '9' && index < len) {
        index++;
        count++;
      }

      t->value = malloc(count + 1);
      memcpy(t->value, &source[start], count);
      t->value[count] = '\0';

      continue;
    }


    if(c == '"') {
      token *t = &tokens->list[tokens->length++];

      t->type = TOKEN_STRING;

      int start = index + 1;
      int count = 0;

      index++;

      while(source[index] != '"' && index < len) {
        index++;
        count++;
      }

      index++; // Move past trailing '"'

      t->value = malloc(count + 1);
      memcpy(t->value, &source[start], count);
      t->value[count] = '\0';

      continue;
    }

    if (c == '+' || c == '-' || c == '*' || c == '/') {
      token *t = &tokens->list[tokens->length++];

      t->type = TOKEN_OPERATOR;
      t->value = malloc(2);
      t->value[0] = c;
      t->value[1] = '\0';

      index++;

      continue;
    }

    if(c == ' ') {
      index++;
      continue;
    }

    if(c == '\n' || (c == '\r' && source[index+1] == '\n')) {
      // New line is the end
      break;
    }

    fprintf(stderr, "Lexer Error: Unrecognised token {%c}.\n", c);
    exit(-1);
  }

  return tokens;
}

void free_tokens(token_list *tokens) {
  while(tokens->length--) {
    free(tokens->list[tokens->length].value);
  }
  free(tokens);
}
