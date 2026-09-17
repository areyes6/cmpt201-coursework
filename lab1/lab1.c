#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *text = NULL;
  size_t len = 0;
  ssize_t read;
  char *saveptr;
  char *token;
  printf("Please enter some text: ");
  read = getline(&text, &len, stdin);
  if (read != -1) {
    printf("Tokens:\n");
    for (token = strtok_r(text, " ", &saveptr); token != NULL;
         token = strtok_r(NULL, " ", &saveptr)) {
      printf(" %s\n", token);
    }
  } else {
    perror("getline failed");
    exit(EXIT_FAILURE);
  }
  free(text);
  return 0;
}
