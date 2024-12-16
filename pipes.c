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

char* rot13(char* c){
    if(c == NULL){
      return NULL;
    }
    char temp[100000];
    strcpy(temp, c);
    char temp2[100000];
    for (int i = 0; i<strlen(temp); i++){
      char curr = temp[i];
      if((curr >= 97 && curr <= 122) || (curr >= 65 && curr <= 90)){
        if(curr > 109 || (curr > 77 && curr < 91)){
          curr -= 13;
        }
        else{
          curr += 13;
        }
      }
      temp2[i] = curr;
    }
    strcpy(c,temp2);
    return c;
}

int main(){
  while (1){
    int fds[2];
    int p = pipe( fds );
    int fds1[2];
    int p1 = pipe( fds1 );
    if (p < 0){
      perror("Pipe creation failed");
      exit(1);
    }
    if (p1 < 0){
      perror("Pipe 1 creation failed");
      exit(1);
    }
    int f = fork();
    if (f < 0){
      perror("Fork failed");
      exit(1);
    }
    int buffSize = 10000;
    char buff[buffSize+1];
    buff[buffSize]=0;
    char buff1[buffSize+1];
    buff1[buffSize]=0;
    char buff2[buffSize+1];
    buff2[buffSize]=0;
    if (f == 0) {
      close( fds[WRITE] );
      read( fds[READ], buff1, sizeof(buff1));
      strcpy(buff1,rot13(buff1));
      close (fds1[READ]);
      write( fds1[WRITE], buff1, strlen(buff1));
      return 0;
    }
    else {
      close( fds[READ] );
      printf("pid %d, Enter text: ", getpid());
      fgets(buff, buffSize, stdin);
      char* copy = (char *) malloc(10000);
      sscanf(buff, "%[^\n]", copy);
      write( fds[WRITE], copy, strlen(copy));
      free(copy);
      int status;
      pid_t cpid;
      cpid = wait(&status);
      close(fds1[WRITE]);
      read( fds1[READ], buff2, sizeof(buff2));
      printf("Result 0: %s\n", buff2);
    }
  }
}
