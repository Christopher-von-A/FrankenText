/**
 * @file frankenstein.c
 * @brief Random sentence generator using tokens from Mary Shelley's
 * *Frankenstein*.
 *
 * This program tokenizes the embedded text of *Frankenstein* and builds a
 * successor table that records which tokens follow which. It then generates
 * random sentences by following random successor chains.
 *
 * Example output:
 * @code
 * My dear Victor Frankenstein in the mountains and when it appeared to them it
 * was a moment of joy and wonder?
 *
 * Excellent! I will not hear you now!
 * @endcode
 */

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_COUNT 50000
#define MAX_SUCCESSOR_COUNT (MAX_WORD_COUNT / 10)

char book[] = {
#embed "pg84.txt"
    , '\0'};

char *tokens[MAX_WORD_COUNT];
size_t tokens_size = 0;
char *succs[MAX_WORD_COUNT][MAX_SUCCESSOR_COUNT];
size_t succs_sizes[MAX_WORD_COUNT];

/// Overwrites non-printable characters in `book` with a space.
void replace_non_printable_chars_with_space() {
  for (size_t i = 0; book[i] != '\0'; i++) {
    if (!isprint((unsigned char)book[i]))
      book[i] = ' ';
  }
}

/// Returns token id if token is known, otherwise creates a new entry and
/// returns the new token id
size_t token_id(char *token) {
  for (size_t i = 0; i < tokens_size; i++) {
    if (strcmp(tokens[i], token) == 0)
      return i;
  }
  if (tokens_size >= MAX_WORD_COUNT) {
    fprintf(stderr, "Token limit reached!\n");
    exit(EXIT_FAILURE);
  }
  tokens[tokens_size] = strdup(token);
  succs_sizes[tokens_size] = 0;
  return tokens_size++;
}

/// Appends a token to the successors list of a `token`
void append_to_succs(char *token, char *succ) {
  size_t id = token_id(token);
  size_t succ_id = token_id(succ);
  if (succs_sizes[id] < MAX_SUCCESSOR_COUNT)
    succs[id][succs_sizes[id]++] = tokens[succ_id];
}

/// Creates tokens from the `book` and fills `tokens` and `succs` arrays using
/// the `token`s.
void tokenize_and_fill_succs(char *delimiters, char *str) {
  char *saveptr;
  char *prev = NULL;
  char *token = strtok_r(str, delimiters, &saveptr);

  while (token) {
    token_id(token);
    if (prev)
      append_to_succs(prev, token);
    prev = token;
    token = strtok_r(NULL, delimiters, &saveptr);
  }
}

/// Returns last character of a string
char last_char(char *str) {
  size_t len = strlen(str);
  return len ? str[len - 1] : '\0';
}

/// Returns whether the token ends with `!`, `?` or `.`
bool token_ends_a_sentence(char *token) {
  char c = last_char(token);
  return c == '.' || c == '?' || c == '!';
}

/// Returns a random `token_id` that corresponds to a `token` that starts with a
/// capital letter.
size_t random_token_id_that_starts_a_sentence() {
  size_t id;
  do {
    id = rand() % tokens_size;
  } while (!isupper((unsigned char)tokens[id][0]));
  return id;
}

/// Generates a random sentence using `tokens`, `succs`, and `succs_sizes`.
char *generate_sentence(char *sentence, size_t sentence_size) {
  size_t current_token_id = random_token_id_that_starts_a_sentence();
  char *token = tokens[current_token_id];

  strcat(sentence, token);
  if (token_ends_a_sentence(token))
    return sentence;

  size_t sentence_len_next;

  do {
    if (succs_sizes[current_token_id] == 0)
      break;
    char *next_token =
        succs[current_token_id][rand() % succs_sizes[current_token_id]];
    sentence_len_next = strlen(sentence) + strlen(next_token) + 2;
    if (sentence_len_next >= sentence_size - 1)
      break;

    strcat(sentence, " ");
    strcat(sentence, next_token);

    if (token_ends_a_sentence(next_token))
      break;

    current_token_id = token_id(next_token);
  } while (sentence_len_next < sentence_size - 1);

  return sentence;
}

int main() {
  replace_non_printable_chars_with_space();

  char *delimiters = " \n\r";
  tokenize_and_fill_succs(delimiters, book);

  srand(time(NULL));
  char sentence[1000];

  // Initialize `sentence` with a mem* function (requirement)
  memset(sentence, 0, sizeof(sentence));

  // Generate and print a question sentence
  do {
    memset(sentence, 0, sizeof(sentence));
    generate_sentence(sentence, sizeof(sentence));
  } while (last_char(sentence) != '?');
  puts(sentence);
  puts("");

  // Generate and print an exclamation sentence
  do {
    memset(sentence, 0, sizeof(sentence));
    generate_sentence(sentence, sizeof(sentence));
  } while (last_char(sentence) != '!');
  puts(sentence);
}
