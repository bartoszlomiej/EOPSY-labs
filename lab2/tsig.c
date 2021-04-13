#include <stdio.h>
#include <stdlib.h> //for the exit() function
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


/*
 * OBJECTIVE:
 * 1) Kill children properly
 * 2) Write the success after creation of all processes successfully
 * 3) Exit with error code 1 if child was not created properly
 */

#define NUM_CHILD 5 //defined number of child processes

/*
void handle_sigchld(int sig){
  printf("STH%d\n", sig); //just for the future purpouse of using handling
}
*/

void kill_children(int* t){
  for(int i = 1; i < NUM_CHILD; i++){
    //    printf("The children was killed properly: %d\n", t[i]);
    kill(t[i], SIGTERM); //terminates all processes apart from the oldest one.
  }
}

void create_processes(int* t){
  t[0] = getpid(); //pid of the oldest process
  for(int i = 1; i < NUM_CHILD; i++ ){
    if(t[0] == getpid()){ //the processes will be created ONLY by the one (the oldest) processp
      t[i] = fork();
      if(t[i] == -1){//optionally -> check if all codes are equal to 0 -> call the success.
	kill_children(t);
	printf("The child proccess was not created properly");
	exit(1);
      }
      sleep(1);
    }
  }
}

int main(){
  int t[NUM_CHILD]; //the table with all processes ID's
  create_processes(t);
  printf("ala ma kota\n");
  if(t[0] == getpid()){
    printf("All %d processes were created successfully", NUM_CHILD);
    sleep(3);
    kill_children(t);
  }

  //  while(1);
  return 0;
}
