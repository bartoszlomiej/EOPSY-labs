#include <stdio.h>
#include <stdlib.h> //for the exit() function
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


#define NUM_CHILD 5 //defined number of child processes

int mark = 0;

void ignore(){}

void handle_sigterm(){
  printf("The children are making revolution. We don't want to die!!!\n\n");
}

void kill_children(int* t){
  for(int i = 1; i < NUM_CHILD; i++){
    if(t[i] > 0)
      kill(t[i], SIGTERM); //terminates all processes apart from the oldest one.
  }
}

void create_processes(int* t){
  t[0] = getpid(); //pid of the oldest process
  for(int i = 1; i < NUM_CHILD; i++ ){
    if(t[0] == getpid()){//the processes will be created ONLY by the oldest process
      t[i] = fork();
      
      if(t[i] == -1){//optionally -> check if all codes are equal to 0 -> call the success.
	kill_children(t);
	printf("The child proccess was not created properly\n");
	exit(1); //error -> exit code 1
      }
      if(getpid() != t[0]){
	signal(SIGINT, SIG_IGN);
	signal(SIGTERM, handle_sigterm);
      }
      sleep(1);
    }
    if (mark > 0 && getpid() == t[0]){ //killing already existin children if mark was spotted
      kill_children(t);
      printf("The process of making children was interrupted ( ͡° ͟ʖ ͡°)\n" );
    }
  }
  if(t[NUM_CHILD-1] != 0) //means that the last child was born.
    printf("All %d processes were created successfully\n", NUM_CHILD);
}

void synchronize(int* t){
  if(t[0] == getpid()){
    signal(SIGCHLD, SIG_DFL);
    int i = 1;
    while(i != NUM_CHILD){ //counting all children processes terminations.
      if(wait(NULL) != -1) //waiting for all children to die.
	i = i + 1;
      printf("O chuj.jpg: %d\n", i);
    }
    printf("The children are dead.\n");
  }
  else{
    printf("The parent ID: %d\n", getppid());
    sleep(10);
    printf("The child process was completed. Process PID: %d\n", getpid());
  }
}

void ignore_all(){
  for(int i = 0; i < 32; i++){
    signal(i, SIG_IGN); //    signal(i, ignore);
  }
}

void reset_signals(){
  for(int i = 0; i < 32; i++){
    signal(i, SIG_DFL);
  }
}

void handle_sigint(){
  printf("The keyboard interrupt was spotted!!!\n");
  mark = mark + 1;
}


/* TODO:
 * FIX -> the synchronization when creation of children was interrupted.
 * Interesting -> if at least 3 children were already created -> works properly
 * Probably -> just after creation of children, they should learn how to ignore the interrupt
 */
int main(){
  int t[NUM_CHILD]; //the table with all processes ID's
  ignore_all();
  signal(SIGINT, handle_sigint);

  for(int i = 0; i < NUM_CHILD; i++)//fun fact: on fedora the table is not initialized with 0,while on xubuntu it is
    t[i] = 0;

  create_processes(t);
  
  //  if(mark == 0) //if children are dead, then there is nothing to synchronize
  synchronize(t);
  //  if(t[0] == getpid()) //just for the dbg of the interrupt handler
    //    printf("What's my mark?: %d\n", mark);
  //    while(mark < 10);
  if(getpid() != t[0])
    reset_signals();
  //  while(mark < 10);
  return 0;
}
