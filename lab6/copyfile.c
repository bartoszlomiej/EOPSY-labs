#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int open_file(char* filename){ /*Opens file iff the given file exist*/
  int pfd; //the file discriptor
  if((pfd = open(filename, O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1){
    fprintf(stderr, "Cannot open giver file.\n");
    exit(1);
  }
  else{
    printf("File was opened properly\n");
  }
  return pfd;
}

int open_destination_file(char* filename){ /*Opens or creates file and change it size to 0 of it exist*/
  int pfd;
  if((pfd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1){
    fprintf(stderr, "cannot open giver file.");
    exit(1);
  }
  else{
    printf("File was opened properly");
  }
  return pfd;
}



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
      exit(0); //prevent to further execution of the program if help was set
      break;
    case '?':
      printf("wrong parameters were given\n");
      printf("Type -h flag to show the help\n");
      exit(1);
    }
  }

  if(argc - mark_m > 3 || argc < 3){
    printf("wrong number of the arguments\n");
    exit(1);
  }
  
  for(; optind < argc; optind++){ //parsing later arguments
    printf("The next arguments: %s\n", argv[optind]); 
  }

  int file1 = open_file(argv[1]);
  int file2 = open_destination_file(argv[2]);

  if(!mark_m){
    copy_read_write(0, 0);
  }
  else{
    copy_mmap(0, 0);
  }

  /* closes the file descriptor */
  close(file1);
  close(file2);
  return 0;
}
