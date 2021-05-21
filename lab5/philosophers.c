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

struct sembuf sem_op[2]; //to operate on mutex and the set of the semaphores
int mutex;
int s;

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
  sem_op[sem_set_id].sem_op = -1;//sem_op[sem_set_id].sem_op - 1;
  sem_op[sem_set_id].sem_flg = 0;
  semop(sem_set_id, &sem_op[sem_set_id], sem_num);
}

void unlock(int sem_set_id, int sem_num){
  sem_op[sem_set_id].sem_num = sem_num; //the semaphore number in the set
  sem_op[sem_set_id].sem_op = 1;//sem_op[sem_set_id].sem_op + 1;
  sem_op[sem_set_id].sem_flg = 0;
  semop(sem_set_id, &sem_op[sem_set_id], sem_num);
}

void eat(int i){
  sleep(1);
  printf("I am eating now: %d\n", i);
}

void think(int i){
  sleep(1);
  printf("I am thinking now: %d\n", i);
}

void put_away_forks(i){
  sem_op[s].sem_op = +1;
  sem_op[s].sem_num = i;
  semop(s, &sem_op[1], 1); //taking the 'fork' of the i'th philosopher

  sem_op[s].sem_op = +1;
  sem_op[s].sem_num = LEFT;
  semop(s, &sem_op[1], 1); //taking the 'fork' of the philosopher on the left
}

void grab_forks(int i){
  sem_op[s].sem_op = -1;
  sem_op[s].sem_num = i;
  semop(s, &sem_op[1], 1); //releasing the 'fork' of the i'th philosopher

  sem_op[s].sem_op = -1; //releasing the 'fork' of the philosopher on the left
  sem_op[s].sem_num = LEFT;
  semop(s, &sem_op[1], 1);
}

void sit_to_table(int i){
  lock(mutex, 0); //enter the critical section - sit to the table
  printf("I am at table! %d\n", i);
  unlock(mutex, 0); //when the philosopher is at the table, then the another one can sit
}

void philosopher(int i){
  sit_to_table(i);
  sleep(1);
  while(1){
    think(i); //think - wait before eat - protects from taking 'fork' two times in a row
    grab_forks(i);
    eat(i); //eat - eating must last for some time:)
    put_away_forks(i);
  }
}

void create_philosopher(){
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
  create_philosopher();
  return 0;
}
