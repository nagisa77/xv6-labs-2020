#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

/*
  Write a simple version of the UNIX xargs program: 
  read lines from the standard input and run a command for each line, 
  supplying the line as arguments to the command. 
*/


int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("params error\n"); 
    exit(1); 
  }

  char *xargs_argv[MAXARG] = {0};
  int xargs_argc = 0; 
  for (int i = 1; i < argc; ++i) {
    xargs_argv[i - 1] = argv[i]; 
    ++xargs_argc; 
  }

  char word[100] = {0}; 
  char c = '#'; 
  int word_p = 0; 
  int xargs_argv_p = xargs_argc; 
  while (read(0, &c, sizeof(char))) {
    if (c == '\n') {
      char* arg = malloc(word_p * sizeof(char)); 
      memcpy(arg, word, word_p); 
      xargs_argv[xargs_argv_p++] = arg;
      word_p = 0; 

      if (fork() == 0) {
        exec(xargs_argv[0], &xargs_argv[0]);
      } else {
        wait(0);
      }
      xargs_argv_p = xargs_argc;
    } else if (c == ' ') {
      char* arg = malloc(word_p * sizeof(char)); 
      memcpy(arg, word, word_p); 
      xargs_argv[xargs_argv_p++] = arg;
      word_p = 0; 
    } else {
      word[word_p++] = c;
    }
  }
  exit(1); 
}