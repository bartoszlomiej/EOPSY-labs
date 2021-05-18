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
/*
  int sem_key = getsem(int key, int semaphore_number_in_set, int flags_and_permissions)
  
  exemplary usage:
  int sem_set_id;
  sem_set_id = semget(10, 5, IPC_CREAT | 0600)
  if(sem_set_id == -1){
    perror("main: semget");
    exit(1);
  }
//another -> change 10 to IPC_PRIVATE and the semaphore_number_in_set to 1
 */

/*
//Mutex with  sem_op:
struct sembuf sem_op;
sem_op.sem_num = 0;
sem_op.sem_op = -1; //wait on the semaphore, unless it's value is non-negative
sem_op.sem_flg = 0;

*Critical Section Here*

sem_op.sem_num = 0;
sem_op.sem_op = 1; //signal the semaphore that it is free ???
sem_op.sem_flg = 0;
 */
void grab_forks(int left_fork_id){
  return;
}

void put_away_forks(int left_fork_id){
  return;
}
//-----------------------------------------------------------------------------------------------------------------
int get_semaphore(int id, int n){
  int sem_id = semget(id, n, IPC_CREAT | 0600);
  if (sem_id == -1) {
    perror("main: semget");
    exit(1);
  }
  return sem_id;
}

void initialize_semaphore(int sem_set_id){
  union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
				(Linux-specific) */
  };
  union semun sem_val;
  sem_val.val = 1;
  int ret_val = semctl(sem_set_id, 0, SETVAL, sem_val); //return value from the semctl, simply check.
  if (ret_val == -1) {
    perror("main: semctl");
    exit(1);
  }
}

void lock(int sem_set_id, struct sembuf *sem_op){
  /*struct sembuf semop;*/
  (&(*sem_op))->sem_num = 0;
  (&(*sem_op))->sem_op = -1;
  (&(*sem_op))->sem_flg = 0;
  semop(sem_set_id, sem_op, 1);
}

void unlock(int sem_set_id, struct sembuf *sem_op){
    sem_op->sem_num = 0;
    sem_op->sem_op = 1;
    sem_op->sem_flg = 0;
    semop(sem_set_id, sem_op, 1);
}
//-----------------------------------------------------------------------------------------------------------------
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
  struct sembuf sem_op;
  int a = get_semaphore(100, 1);
  initialize_semaphore(a);
  
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
    sleep(5);
  }
  if(t[0] != getpid()){
    //      lock(a, &sem_op);
      sleep(1);
      //      unlock(a, &sem_op);
  }
  
  philosopher(count);
}
//int get_semaphore(int id, int n){
//void initialize_semaphore(int sem_set_id){
//void lock(int sem_set_id, struct sembuf *sem_op){
//void unlock(int sem_set_id, struct sembuf *sem_op){
int main(){


  create_philosophers();
  //  unlock(a, &sem_op);
  return 0;
}
