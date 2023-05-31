#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int input_fd, int output_fd) {
  close(output_fd); 

  int has_number_before = 0;
  read(input_fd, &has_number_before, sizeof(int)); 
  if (!has_number_before) {
    close(input_fd); 
    exit(0); 
  }
  
  int number = 0; 
  int is_first_number = 1; 
  int first_number = 0; 
  int has_send_has_number = 0;
  int has_number = 0; 
  int fds[2] = {0}; 

  pipe(fds); 
  int next_input_fd = fds[0]; 
  int next_output_fd = fds[1];
  int pid = fork();
  if (pid > 0) {
    close(next_input_fd); 
    while (read(input_fd, &number, sizeof(int))) {
      if (is_first_number) {
        is_first_number = 0; 
        first_number = number;
        printf("prime %d\n", number); 
      } else {
        if (number % first_number) {
          if (!has_send_has_number) {
            has_send_has_number = 1; 
            has_number = 1; 
            write(next_output_fd, &has_number, sizeof(int)); 
          }
          write(next_output_fd, &number, sizeof(int)); 
        } else {
          // drop 
        }
      }
    }
    if (!has_number) {
      write(next_output_fd, &has_number, sizeof(int)); 
    }
    close(input_fd);
    close(next_output_fd); 
    wait(0); 
  } else if (pid == 0) {
    close(input_fd); 
    primes(next_input_fd, next_output_fd); 
  }
  exit(0); 
}

int main(int argc, char *argv[]) {
  int fds[2] = {0}; 
  pipe(fds); 

  for (int i = 1; i <= 35; ++i) {
    write(fds[1], &i, sizeof(int)); 
  }

  primes(fds[0], fds[1]); 

  exit(0); 
  return 0; 
}