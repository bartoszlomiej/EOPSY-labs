#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv){
  int c;
  int i = 0;
  while(( c = getopt(argc, argv, ":mh" )) != -1 ){ //parsing flags only
    i = i + 1;
    switch(c){
    case 'm':
      printf("-m,   ");
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
  for(; optind < argc; optind++){ //parsing later arguments
    printf("The next arguments: %s\n:", argv[optind]); 
  }
  return 0;
}
