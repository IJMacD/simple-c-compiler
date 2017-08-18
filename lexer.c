#include <stdlib.h>
#include <string.h>

#define TOKEN_NAME    1
#define TOKEN_NUMBER  2
#define TOKEN_STRING  3

#define DEFAULT_TOKEN_COUNT 100

typedef struct token_struct {
  int type;
  char *value;
} token;

typedef struct token_list_struct {
  int length;
  token *list;
} token_list;

token_list* lexer(char *source);

/*    LEXER     */
token_list* lexer(char *source) {

    int index = 0;
    int len = strlen(source);

    token_list *tokens = malloc(sizeof(token_list));
    tokens->length = 0;
    tokens->list = malloc(DEFAULT_TOKEN_COUNT * sizeof(token));

    while(index < len) {
      char c = source[index];

      if(c >= 'a' && c <= 'z') {
        token *t = malloc(sizeof(token));
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

        tokens->list[tokens->length] = *t;
        tokens->length++;

        continue;
      }


      if(c >= '0' && c <= '9') {
        token *t = malloc(sizeof(token));
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

        tokens->list[tokens->length] = *t;
        tokens->length++;

        continue;
      }


      if(c == '"') {
        token *t = malloc(sizeof(token));
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

        tokens->list[tokens->length] = *t;
        tokens->length++;

        continue;
      }

      if(c == ' ') {
        index++;
        continue;
      }

      printf("Lexer Error: Unrecognised token.\n");
      exit(-1);
    }

    return tokens;
  }
