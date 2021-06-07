#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void grab_forks(int philo_id){
  return;
}

void put_away_forks(int philo_id){
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
void *philosopher(void* philo_id){
  int *id = (int *)philo_id;
  printf("Hi, my name is: %d\n", *id);
}

int main(){
  pthread_t tid;
  for(int i =0; i < 5; i++){
    printf("loop %d", i);
    pthread_create(&tid, NULL, philosopher, (void*)&tid); //behaves complitely unpredictable
  }
  printf("The End\n");
}
