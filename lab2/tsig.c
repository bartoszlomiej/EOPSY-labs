#include <stdio.h>
#include <stdlib.h> //for the exit() function
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


/*
 * 
 * 
 * 
 */

void NUM_CHILD(){
  printf("Hello World!\n");
}

void parent(){
  int success = 0;
  success = fork();
  if( success == -1 ){
    //call the SIGTERM signal here    
    printf("The child proccess was not created properly");
    exit(1);
  }
  printf("Process: %d\n", success);
  NUM_CHILD();
  sleep(1);
}

int main(){
  //  NUM_CHILD();
  parent();
  parent();
  return 0;
}



