#include "stdlib.h"
#include <stdio.h>
#include <string.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/ipc.h>

/*
The idea:
-There are 5 philosophers.
-There are 5 forks
-In order to eat, a philosopher must have 2 forks
-If each philosopher grabs the fork on the right, then waits for the fork on the left -> the deadlock occurs
-The philosophers should be fair (if he don't have the left fork -> he don't keep the one on the right)
-Each philosopher should be able to eat as much as the rest

In other words:
-Each philosopher is a process
-The must have functions:
a) void grab_forks(int left_fork_id);
b) void put_away_forks(int left_fork_id);
-where the left_fork_id is an integer identifying semaphores associated with forks
 */

//Remember to print on the standard output verbose messages from philosophers

#define N 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT (i + N - 1) % N
#define RIGHT (i + N + 1) % N

struct sembuf sem_op;
int mutex;
int s;
int state[N]; //all states are 0 by default

/*

void eat(){//just for dbg
  printf("Hejo\n");
  return;
}
void donteat(){//just for dbg
  printf("Glodny\n");
  return;
}

void grab_forks(int left_fork_id){
  return;
}

void put_away_forks(int left_fork_id){
  return;
}
*/
//-----------------------------------------------------------------------------------------------------------------
int get_semaphore(int id, int n){
  int sem_id = semget(id, n, IPC_CREAT | 0600);
  if (sem_id == -1) {
    perror("main: semget");
    exit(1);
  }
  return sem_id;
}

void initialize_semaphore(int sem_set_id, int sem_num){ /* Due to the semctl documentation it must be declared */
  union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
				(Linux-specific) */
  };
  union semun sem_val;
  sem_val.val = 1;
  int ret_val = semctl(sem_set_id, sem_num, SETVAL, sem_val); //return value from the semctl, simply check.
  if (ret_val == -1) {
    perror("main: semctl");
    exit(1);
  }
}

void lock(int sem_set_id, int sem_num){
  /*struct sembuf semop;*/
  sem_op.sem_num = sem_num;
  sem_op.sem_op = -1;
  sem_op.sem_flg = 0;
  semop(sem_set_id, &sem_op, 1);
}

void unlock(int sem_set_id, int sem_num){
  sem_op.sem_num = sem_num; //the semaphore number in the set
  sem_op.sem_op = 1;
  sem_op.sem_flg = 0;
  semop(sem_set_id, &sem_op, 1);
}
//-----------------------------------------------------------------------------------------------------------------
//There are 5 philosophers -> from 0 to 4
//void philosopher(int number){
  
  /*
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
  */
//}

//-----------------------------------------------------------------------------------------------------------------
void eat(){
  printf("eating Philosopher: %d\n", getpid());
}

void think(){
  printf("//thinking Philosopher: %d\n", getpid());
}
//-----------------------------------------------------------------------------------------------------------------

void test(i){
  if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING){
    state[i] = EATING;
    unlock(s, i);
  }
}

void put_away_forks(i){
  lock(mutex, 0);
  state[i] = THINKING;
  test(LEFT);
  test(RIGHT);
  unlock(mutex, 0);
}

void grab_forks(int i){
  lock(mutex, 0);
  state[i] = HUNGRY;
  test(i);
  unlock(mutex, 0);
  lock(s, i);
}

void philosopher(int i){
  int j = 0;
  while(j < 2){
    think( );
    grab_forks(i);
    eat( );
    put_away_forks(i);
    j++;
  }
}
//-----------------------------------------------------------------------------------------------------------------
void create_philosophers(){
  mutex = get_semaphore(100, 1); //mutex creation
  s = get_semaphore(200, 5); //the set of 5 semaphores

  initialize_semaphore(mutex, 0);
  for(int i = 0; i < N; i++){
    initialize_semaphore(s, i);
  }
  
  int t[5];
  int count = -1;
  t[0] = getpid();
  for(int i = 1; i < 5; i++){
    if(t[0] == getpid()){
      count = count + 1; //Incrementing philosopher no by 1 before creation
      t[i] = fork(); //creating 5 processes -> our philosophers
    }
  }
  if(t[0] == getpid()){
    count = count + 1; //the first philosopher number incrementation
    //    sleep(5);
  }
  /*
  if(t[0] != getpid()){
    //      lock(a, &sem_op);
      sleep(1);
      //      unlock(a, &sem_op);
  }
  */
  philosopher(count);
}
//int get_semaphore(int id, int n){
//void initialize_semaphore(int sem_set_id){
//void lock(int sem_set_id, int num){
//void unlock(int sem_set_id, int num){
int main(){  
  create_philosophers();
  return 0;
}
