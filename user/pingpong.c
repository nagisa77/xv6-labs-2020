#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int pipes[2] = { 0 }; 
  pipe(pipes); 
  char send_byte = '@'; 
  char receive_byte = '$'; 
  int pid = fork();
  if (pid == 0) {
    // children
    read(pipes[0], &receive_byte, sizeof(char)); 
    close(pipes[0]); 
    printf("%d: received ping\n", getpid()); 
    write(pipes[1], &send_byte, sizeof(char)); 
    close(pipes[1]); 
  } else if (pid > 0) {
    // parent
    write(pipes[1], &send_byte, sizeof(char)); 
    close(pipes[1]); 
    wait(0); 
    read(pipes[0], &receive_byte, sizeof(char)); 
    close(pipes[0]); 
    printf("%d: received pong\n", getpid()); 
  } 

  exit(0); 
}
