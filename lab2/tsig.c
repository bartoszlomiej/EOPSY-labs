#include <stdio.h>
#include <stdlib.h> //for the exit() function
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

#define NUM_CHILD 5 //defined number of child processes
#define WITH_SIGNALS

#ifdef WITH_SIGNALS
int mark = 0;

void ignore(){}

void handle_sigterm(){
  printf("Child[%d]. Recived sigterm signal. The children are making revolution. They don't want to die!!!\n", getpid());
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
	printf("Parent[%d]. The child proccess was not created properly\n", getpid());
	exit(1); //error -> exit code 1
      }
      if(getpid() != t[0]){ //the child are learning to ignore interrupt and handle their terminations
	signal(SIGINT, SIG_IGN);
	struct sigaction act;
	memset(&act, '\0', sizeof(act));
	act.sa_sigaction = &handle_sigterm;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGTERM, &act, NULL);
      }
      sleep(1);
    }
    if (mark == 1 && getpid() == t[0]){ //killing already existin children if mark was spotted
      kill_children(t);
      printf("Parent[%d]. Sending SIGTERM signal. The process of making children was interrupted ( ͡° ͟ʖ ͡°)\n", getpid());
      mark = mark + 1;
    }
  }
  if(t[NUM_CHILD-1] != 0) //means that the last child was born.
    printf("Parent[%d]. All %d processes were created successfully\n", getpid(), NUM_CHILD);
}

void synchronize(int* t){
  if(t[0] == getpid()){
    int i = 1;
    while(i != NUM_CHILD){ //counting all children processes terminations.
      if(wait(NULL) != -1) //waiting for all children to die.
	i = i + 1;
    }
    printf("Parent[%d]. The children are dead.\n", getpid());
  }
  else{
    printf("Child[%d]. The parent ID: %d\n", getpid(), getppid());
    sleep(10);
    printf("Child[%d]. The process is finished.\n", getpid());
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
  printf("Parent[%d]. The SIGINT signal was recived\n", getpid());
  mark = mark + 1;
}

int main(){
  int t[NUM_CHILD]; //the table with all processes ID's
  ignore_all();
  signal(SIGINT, handle_sigint);
  signal(SIGCHLD, SIG_DFL);
  for(int i = 0; i < NUM_CHILD; i++)//fun fact: on fedora the table is not initialized with 0,while on xubuntu it is
    t[i] = 0;

  create_processes(t);
  
  synchronize(t);

  if(getpid() != t[0])
    reset_signals();

  return 0;
}
#endif

#ifndef WITH_SIGNALS

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
	printf("Parent[%d]. The child proccess was not created properly\n", getpid());
	exit(1); //error -> exit code 1
      }
      sleep(1);
    }
  }
  if(t[NUM_CHILD-1] != 0) //means that the last child was born.
    printf("Parent[%d]. All %d processes were created successfully\n", getpid(), NUM_CHILD);
}

void synchronize(int* t){
  if(t[0] == getpid()){
    int i = 1;
    while(i != NUM_CHILD){ //counting all children processes terminations.
      if(wait(NULL) != -1) //waiting for all children to die.
	i = i + 1;
    }
    printf("Parent[%d]. The children are dead.\n", getpid());
  }
  else{
    printf("Child[%d]. The parent ID: %d\n", getpid(), getppid());
    sleep(10);
    printf("Child[%d]. The process is finished.\n", getpid());
  }
}

int main(){
  int t[NUM_CHILD]; //the table with all processes ID's

  for(int i = 0; i < NUM_CHILD; i++)//fun fact: on fedora the table is not initialized with 0,while on xubuntu it is
    t[i] = 0;

  create_processes(t);
  synchronize(t);

  return 0;
}
#endif
