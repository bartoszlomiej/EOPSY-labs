#include <stdio.h>
#include <stdlib.h> //for the exit() function
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


#define NUM_CHILD 5 //defined number of child processes

int mark = 0;

void kill_children(int* t){
  for(int i = 1; i < NUM_CHILD; i++){
    if(t[i] > 0)
      kill(t[i], SIGTERM); //terminates all processes apart from the oldest one.
  }
}

void create_processes(int* t){
  t[0] = getpid(); //pid of the oldest process
  for(int i = 1; i < NUM_CHILD; i++ ){
    if((t[0] == getpid()) && (mark == 0)){//the processes will be created ONLY by the oldest process
      t[i] = fork();
      if(t[i] == -1){//optionally -> check if all codes are equal to 0 -> call the success.
	kill_children(t);
	printf("The child proccess was not created properly\n");
	exit(1); //error -> exit code 1
      }
      sleep(1);
    }
    else if (mark > 0){ //killing already existin children if mark was spotted
      kill_children(t);
      return;
    }
  }
  if(t[NUM_CHILD-1] != 0) //means that the last child was born.
    printf("All %d processes were created successfully\n", NUM_CHILD);
}

void synchronize(int* t){
  if(t[0] == getpid()){
    int i = 1;
    while(i != NUM_CHILD){ //counting all children processes terminations.
      if(wait(NULL) != -1) //waiting for all children to die.
	i = i + 1;
    }
    printf("The children are dead.\n");
  }
  else{
    printf("The parent ID: %d\n", getppid());
    sleep(10);
    printf("The child process was completed. Process PID: %d\n", getpid());
  }
}

void ignore(){}

void ignore_all(){
  for(int i = 0; i < 32; i++){
    signal(i, ignore);
  }
}

void handle_sigint(){
  printf("The keyboard interrupt was spotted!!!\n");
  mark = mark + 1;
}


/*
void handle_sigchld(int sig){
  printf("STH%d\n", sig); //just for the future purpouse of using handling
}
*/

/*
 * OBJECTIVE:
 * -SIGNALS
 * 1) Force ignoring of all signals (signal / (sigaction->restore default handler for SIGCHLD signal))
 * 2) set your own keyboard interrupt signal handler (SIGINT)
 * 3) 
 */

int main(){
  int t[NUM_CHILD]; //the table with all processes ID's
  ignore_all();
  signal(SIGINT, handle_sigint);

  for(int i = 0; i < NUM_CHILD; i++)//fun fact: on fedora the table is not initialized with 0,while on xubuntu it is
    t[i] = 0;

  create_processes(t);
  if(mark == 0) //if children are dead, then there is nothing to synchronize
    synchronize(t);
  //  if(t[0] == getpid()) //just for the dbg of the interrupt handler
    //    printf("What's my mark?: %d\n", mark);
  //    while(mark < 10);
  return 0;
}
