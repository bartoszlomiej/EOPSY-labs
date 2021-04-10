#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


/*
 * create the NUM_CHILD processes. The NUM_CHILD must be defined in the program
 * use the fork function
 * insert one second delays between consecutive fork() calls.
 */

void Create_Chlid(){
  printf("Hello World!\n");
}

int main(){
  Create_Chlid();
  sleep(1);
  fork();
  Create_Chlid();
  sleep(1);
  fork();
  Create_Chlid();
  return 0;
}



