#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv){
  if (argc == 2){
    if(!strcmp(argv[1], "-h")){
      printf("Help should be printed\n");
    }
    else{
      printf("Wrong input was given. Type './copyfiles -h' to check the proper syntax\n");
      return -1; //if there is wrong input - exit
    }
  }
  else if(argc == 3){
    printf("it is assumed that two files names are here\n");
  }
  else if(argc == 4){
    if(!strcmp(argv[1], "-m")){
      printf("The -m option should be used\n");
    }
    else{
      return -1; //if there is wrong input - exit
      printf("Wrong input was given. Type './copyfiles -h' to check the proper syntax\n");
    }
  }
  else{
    printf("Wrong input was given. Type './copyfiles -h' to check the proper syntax\n");
    return -1;
  }
  return 0;
}
