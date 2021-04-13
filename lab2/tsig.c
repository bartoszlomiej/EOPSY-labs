#include <stdio.h>
#include <stdlib.h> //for the exit() function
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
//#include <wait.h>
#include <signal.h>


/*
 * OBJECTIVE:
 * 1) Call wait() function in the loop -> until receiving from system of all synchronized processes with the parent -- OK!
 * 2) Print a message that there are no more child processes -- OK!
 * 3) In this  loop do count child processes terminations and, at the very end of the parent process, print a message with the number of just received child  processes exit codes
 */

#define NUM_CHILD 5 //defined number of child processes

/*
void handle_sigchld(int sig){
  printf("STH%d\n", sig); //just for the future purpouse of using handling
}
*/

void synchronize(int status){
  wait(&status);
  if(WIFSIGNALED(status))
    printf("Wow, sth have happened. PID: %d\n", getpid());
}

void kill_children(int* t){
  for(int i = 1; i < NUM_CHILD; i++){
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
	exit(1); //error -> exit code 1
      }
      sleep(1);
    }
  }
}

int main(){
  int t[NUM_CHILD]; //the table with all processes ID's

  create_processes(t);
  
  if(t[0] == getpid()){
    int i = 1;
    while(i != NUM_CHILD){ //counting all children processes.
      if(wait(NULL) != -1) //waiting for all children to die.
	i = i + 1;
    }
    printf("All %d processes were created successfully\n", NUM_CHILD);
    printf("The children are dead.\n");
    //    kill_children(t);
  }
  printf("ala ma kota. Process PID: %d\n", getpid());
  //while(1);
  return 0;
}
