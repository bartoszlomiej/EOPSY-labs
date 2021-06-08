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
pthread_mutex_t m;
pthread_t tid[N];

void test(int i){
  if( state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING){
    state[i] = EATING;
  }
}

void grab_forks(int i){
  pthread_mutex_lock(&m);
  state[i] = HUNGRY;
  test(i);
  pthread_mutex_unlock(&m);
  return;
}

void put_away_forks(int i){
  pthread_mutex_lock(&m);
  state[i] = THINKING;
  test(LEFT);
  test(RIGHT);
  pthread_mutex_unlock(&m);
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
void *philosopher(void *philo_id){ //it would be nice if it could be switched to int
  pthread_mutex_lock(&m);
  sleep(1);
  pthread_mutex_unlock(&m);
  int *i = (int *)philo_id;
  printf("Hi, my name is: %d\n", *i);
  int j = 0;
  while(j < 3){
    j++;
    //    grab_forks(*i);
    if(state[*i] == EATING) //should it be here?
      printf("Who is eating? %d", *i);
    //    put_away_forks(*i);
  }
  return philo_id; //just to get rid of warning
}

int main(){
  pthread_mutex_init(&m, NULL);
  for(int i =0; i < 5; i++){
    printf("loop %d", i);
    pthread_create(&tid, NULL, philosopher, (void *)&tid);
  }
  pthread_exit(NULL);
  printf("The End\n");
}
