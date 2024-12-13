#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/stat.h>

#define READ 0
#define WRITE 1
int main(){
  int fds[2];
  int p = pipe( fds );
  if (p < 0){
    perror("Pipe creation failed");
    exit(1);
  }
  int buffSize = 10000;
  char buff[buffSize+1];
  buff[buffSize]=0;
  char buff1[buffSize+1];
  buff1[buffSize]=0;

  int f = fork();
  if (f < 0){
    perror("Fork failed");
    exit(1);
  }
  if (f == 0) {
    close( fds[WRITE] );
    read( fds[READ], buff1, sizeof(buff1));
    printf("pid %d, Recieved message: %s", getpid(), buff1);
  }
  else {
    close( fds[READ] );
    printf("pid %d, Enter text: ", getpid());
    fgets(buff, buffSize, stdin);
    write( fds[WRITE], buff, strlen(buff));
  }
  return 0;
}
