#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 5

int history_count = 0;

char *getinput() {
  char *line = NULL;
  size_t len;

  printf("Enter input: ");
  ssize_t read = getline(&line, &len, stdin);
  if (read == -1) {
    perror("Failed to read line\n");
    exit(1);
  }
  line[read - 1] = '\0';
  return line;
}

int main() {
  char **arr = malloc(MAX_SIZE * sizeof(char *));
  for (int i = 0; i < MAX_SIZE; i++) {
    arr[i] = NULL;
  }

  while (1) {
    char *input = getinput();

    if (history_count >= MAX_SIZE) {
      free(arr[0]);
      for (int i = 1; i < MAX_SIZE; i++) {
        arr[i - 1] = arr[i];
      }

      arr[MAX_SIZE - 1] = strdup(input);
    } else {
      arr[history_count] = input;
      history_count++;
    }
    if (strcmp(input, "print") == 0) {
      for (int i = 0; i < MAX_SIZE; i++) {
        if (arr[i]) {
          printf("%s\n", arr[i]);
        }
      }
    }
  }

  for (int i = 0; i < MAX_SIZE; i++) {
    free(arr[i]);
  }
  free(arr);
  return 0;
}
