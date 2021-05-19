#include "stdlib.h"
#include <stdio.h>
#include <string.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/ipc.h>

//Remember to print on the standard output verbose messages from philosophers

#define N 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT (i + N - 1) % N
#define RIGHT (i + N + 1) % N

struct sembuf sem_op[2];
int mutex;
int s;
int state[N]; //all states are 0 by default

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
  sem_op[sem_set_id].sem_num = sem_num;
  sem_op[sem_set_id].sem_op = sem_op[sem_set_id].sem_op - 1;
  sem_op[sem_set_id].sem_flg = 0;
  semop(sem_set_id, &sem_op[sem_set_id], sem_num);
}

void unlock(int sem_set_id, int sem_num){
  sem_op[sem_set_id].sem_num = sem_num; //the semaphore number in the set
  sem_op[sem_set_id].sem_op = sem_op[sem_set_id].sem_op + 1;
  sem_op[sem_set_id].sem_flg = 0;
  semop(sem_set_id, &sem_op[sem_set_id], sem_num);
}
//-----------------------------------------------------------------------------------------------------------------
void eat(){
  sleep(1);
}

void think(){
  sleep(1);
  printf("xd");
}
//-----------------------------------------------------------------------------------------------------------------

void test(i){
  for(int i = 0; i < 5; i++){
    printf("state[%d]: %d; ", i, state[i]);
  }
  printf("\n");

  if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING){
    state[i] = EATING;
    printf("Philosopher %d takes fork %d and %d\n", i, LEFT, i);
    printf("Philosopher %d is eating\n", i);
    unlock(s, i);
  }
}

void put_away_forks(i){
  lock(mutex, 0);
  state[i] = THINKING;
  printf("Philosopher %d putting fork %d and %d down\n",  i, LEFT, i);
  test(LEFT);
  test(RIGHT);
  unlock(mutex, 0);
}

void grab_forks(int i){
  lock(mutex, 0);
  state[i] = HUNGRY;
  printf("Philosopher %d is Hungry\n", i);
  test(i);
  unlock(mutex, 0);
  lock(s, i);
}

void philosopher(int i){
  int j = 0;
  sleep(1);
  while(1){
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
  }
  philosopher(count);
}

int main(){
  create_philosophers();
  return 0;
}
