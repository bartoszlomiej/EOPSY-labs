#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 5
#define LEFT (i + N - 1)  % N
#define RIGHT (i + 1) % N

#define THINKING 0
#define HUNGRY 1
#define EATING 2

int state[N];
pthread_mutex_t m; //mutex global variable
pthread_cond_t cond[N]; //thread condition variable

pthread_t tid[N]; //stores the threads id's
int counter;

void test(int i){
  if( state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING){
    state[i] = EATING;
    int out = pthread_cond_signal(&cond[i]);
    if(out != 0)
      perror("test: pthread_cond_signal[i]");
    printf("Eating %d\n", i);
    /*    if(pthread_cond_signal(&cond[RIGHT]) != 0)
      perror("test: pthread_cond_signal[RIGHT]");
    */
  }
}

void grab_forks(int i){
  pthread_mutex_lock(&m);
  state[i] = HUNGRY;
  test(i);
  while(state[i] != EATING){
    pthread_cond_wait(&cond[i], &m);
  }
  pthread_mutex_unlock(&m);

  return;
}

void put_away_forks(int i){
  pthread_mutex_lock(&m);
  state[i] = THINKING;
  test(LEFT);
  test(RIGHT);
  printf("I am thinking: %d\n", i);
  pthread_mutex_unlock(&m);
  usleep(10);
  return;
}

/*
  int philo_id - the integer identifying semaphores associated with each fork.

  Use pthreads_XXX and pthread_mutex_XXX interfaces

  Additional questions:
  1. Would it be sufficient just to add to the old algorithm from task5
  additional mutex variable to organize critical sections in functions
  grab_forks() and put_away_forks() for making changes to values of two mutexes
  indivisably?  If not, why?

  2. Why m mutex is initialized with 1 and mutexes from the array s are
  initialized with 0's?
 */
void *philosopher(){ //it would be nice if it could be switched to int
  pthread_mutex_lock(&m);
  counter++;
  int id = counter; //as each thread must have unique id
  pthread_mutex_unlock(&m);
  sleep(2);
  printf("Hi, my name is: %d\n", id);
  int j = 0;
  while(j < 3){
    j++;
    usleep(200);
    grab_forks(id);
    usleep(200);
    //    if(state[counter] == EATING) //should it be here?
    put_away_forks(id);
    usleep(200);
  }
  return 0;
}

int main(){
  int s = 0;
  counter = -1;
  pthread_mutex_init(&m, NULL);
  for(int i =0; i < N; i++){
    if(pthread_cond_init(&cond[i], NULL) != 0) //initializing the conditional variable
      perror("main: pthread_cond_init");
  }
  for(int i =0; i < N; i++){

    s = pthread_create(&(tid[i]), NULL, philosopher, NULL);
    if(s != 0){
      perror("main: pthread_create");
      exit(1);
    }
  }
  /*Just to show that 5 different threads were used*/
  sleep(1);
  for(int i = 0; i < N; i++){
    if(pthread_join(tid[i], NULL) == 0)
      printf("TID: %ld is finished\n", tid[i]);
    else
      perror("main: pthread_join");
  }
  //  pthread_exit(NULL);
  printf("The End\n");
}
