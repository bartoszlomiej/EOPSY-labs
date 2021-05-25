#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void copy_read_write(int fd_from, int fd_to){
  printf("read_write\n");
  return;
}

void copy_mmap(int fd_from, int fd_to){
  printf("mmap\n");
  return;
}

int main(int argc, char** argv){
  int opt;
  int mark_m = 0; //check if -m flag was set
  while(( opt = getopt(argc, argv, ":mh" )) != -1 ){ //parsing flags only
    switch(opt){
    case 'm':
      printf("-m,   ");
      mark_m = 1;
      break;
    case 'h':
      printf("-h,   ");
      return 0; //prevent to further execution of the program if help was set
      break;
    case '?':
      printf("wrong parameters were given\n");
      printf("Type -h flag to show the help\n");
      return -1;
    }
  }

  if(argc - mark_m > 3 || argc < 3){
    printf("wrong number of the arguments\n");
    return -1;
  }
  
  for(; optind < argc; optind++){ //parsing later arguments
    printf("The next arguments: %s\n", argv[optind]); 
  }

  if(!mark_m){
    copy_read_write(0, 0);
  }
  else{
    copy_mmap(0, 0);
  }
  
  return 0;
}
