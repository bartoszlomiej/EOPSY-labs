#include "stdlib.h"
#include <stdio.h>
#include <string.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <signal.h>

//Remember to print on the standard output verbose messages from philosophers

#define N 5
#define LEFT (i - 1 + N) % N

struct sembuf sem_op[2]; //to operate on mutex and the set of the semaphores
struct sembuf sem_op_mutex;
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
  sem_op_mutex.sem_num = sem_num;
  sem_op_mutex.sem_op = -1;//sem_op[sem_set_id].sem_op - 1;
  sem_op_mutex.sem_flg = 0;
  if(semop(sem_set_id, &sem_op_mutex, 1)){
    printf("ERROR");
  }
}

void unlock(int sem_set_id, int sem_num){
  sem_op_mutex.sem_num = sem_num; //the semaphore number in the set
  sem_op_mutex.sem_op = 1;//sem_op[sem_set_id].sem_op + 1;
  sem_op_mutex.sem_flg = 0;
  if(semop(sem_set_id, &sem_op_mutex, 1)){
    printf("ERROR");
  }
}

void eat(int i){
  printf("I am eating now: %d\n", i);
}

void think(int i){
  usleep(50); //the delay that protects from taking the fork two times in a row by the same process
  printf("Thinking now: %d\n", i);
}

void put_away_forks(int i){  
  sem_op[0].sem_op = +1;
  sem_op[0].sem_num = i;
  sem_op[0].sem_flg = 0;

  sem_op[1].sem_op = +1;
  sem_op[1].sem_num = LEFT;
  sem_op[1].sem_flg = 0;

  printf("Eating finished: %d\n", i);
  if(semop(s, sem_op, 2) == -1) //taking the 'fork' of the philosopher on the left
    printf("Error:((");
}

void grab_forks(int i){
  sem_op[0].sem_op = -1;
  sem_op[0].sem_num = i;
  sem_op[0].sem_flg = 0;

  sem_op[1].sem_op = -1; //releasing the 'fork' of the philosopher on the left
  sem_op[1].sem_num = LEFT;
  sem_op[1].sem_flg = 0;

  if(semop(s, sem_op, 2) == -1) //taking the 'fork' of the philosopher on the left
    printf("Error:((");
}

void sit_to_table(int i){
  lock(mutex, 0); //enter the critical section - sit to the table
  printf("I am at table! %d\n", i);
  unlock(mutex, 0); //when the philosopher is at the table, then the another one can sit
  sleep(1);
}

void philosopher(int i){
  sit_to_table(i); //to ensure that all philosophers will start at the same time
  int j = 10;
  int dinner = 0;
  while(j){
    think(i); //think - wait before eat - protects from taking 'fork' two times in a row
    grab_forks(i);
    usleep(100);
    eat(i); //eat - eating must last for some time:)
    dinner++;
    put_away_forks(i);
    usleep(100);
    j--;
  }
  printf("I, the philosopher: %d, ate %d portions!\n", i, dinner);
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
    usleep(500);
    count = count + 1; //the first philosopher number incrementation
  }
  
  philosopher(count);
  if(t[0] == getpid()){
    int i = 1;
    while(i != N){ //counting all children processes terminations.
      if(wait(NULL) != -1) //waiting for all children to die. - to exit correctly
	i = i + 1;
    }
  }
}

int main(){
  create_philosopher();
  return 0;
}
