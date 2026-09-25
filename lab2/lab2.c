#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  char *line = NULL;
  size_t len;

  while (1) {
    printf("Enter programs to run: \n");
    ssize_t read = getline(&line, &len, stdin);
    if (read == -1) {
      perror("Failed to read line\n");
      break;
    } else if (read > 0) {
      line[read - 1] = '\0';
    }

    pid_t pid = fork();

    if (pid == -1) { // fork() fails when -1 is returned
      perror("Fork failed!\n");
      continue;
    } else if (pid != 0) { // parent process, fork() returns child pid
      int status = 0;
      if (waitpid(pid, &status, 0) == -1) { // waitpid failed and returned -1
        perror("waitpid failed\n");
        exit(EXIT_FAILURE);
      }
      if (WIFEXITED(status)) { // if child terminated normally
        printf("Child exited\n");
      }
    } else { // child process, fork() returns 0
      if (execl(line, line, (char *)NULL) == -1) {
        perror("execl failed\n");
        free(line);
        exit(EXIT_FAILURE);
      }
    }
  }

  free(line);
  return 0;
}
