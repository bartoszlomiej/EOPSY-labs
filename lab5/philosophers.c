#include "stdlib.h"
#include <stdio.h>
#include <string.h>
#include <sys/sem.h>
#include <unistd.h>

/*
The idea:
-There are 5 philosophers.
-There are 5 forks
-In order to eat, a philosopher must have 2 forks
-If each philosopher grabs the fork on the right, then waits for the fork on the left -> the deadlock occurs
-The philosophers should be fair (if he don't have the left fork -> he don't keep the one on the right)
-Each philosopher should be able to eat as much as the rest

In other words:
-Each philosopher is a process (?)
-The must have functions:
a) void grab_forks(int left_fork_id);
b) void put_away_forks(int left_fork_id);
-where the left_fork_id is an integer identifying semaphores associated with forks
 */

//Remember to print on the standard output verbose messages from philosophers

int forks[] = {1, 1, 1, 1, 1};

void eat(){//just for dbg
  printf("Hejo\n");
  return;
}
void donteat(){//just for dbg
  printf("Glodny\n");
  return;
}

//There are 5 philosophers -> from 0 to 4
void philosopher(int number){
  printf("My pid: %d, my count: %d\n", getpid(), number);
  if(number == 0){ //border case
    if((forks[0] == 1) && (forks[4] == 1))
      eat(); //just for dbg
    else
      donteat();
  }
  else if(number == 4){
    if((forks[4] == 1) && (forks[3] == 1))
      eat(); //just for dbg
    else
      donteat();
  }
  else if((forks[number] == 1) && (forks[number - 1] == 1))
      eat(); //just for dbg
    else
      donteat();
}

void create_philosophers(){
  int t[5];
  int count = -1;
  t[0] = getpid();
  for(int i = 1; i < 5; i++){
    if(t[0] == getpid()){
      count = count + 1; //!!!!!!!!! Might cause na error
      t[i] = fork(); //creating 5 processes -> our philosophers
    }
  }
  if(t[0] == getpid())
    count = count + 1; //!!!!!!!!! Might cause na error
    
  philosopher(count);
}

int main(){
  create_philosophers();
  return 0;
}
