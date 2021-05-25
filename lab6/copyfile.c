#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void copy_read_write(int fd_from, int fd_to){
  /*-------------Read part---------------*/
  struct stat st;
  fstat(fd_from, &st);
  int size = st.st_size;
  char* buf = (char*)calloc(size,  sizeof(char));
    size_t bytes_read;
  /* Reads data from the file associated with the file descriptor into the buffer pointed to by buf */
  bytes_read = read(fd_from, buf, size);
  
  printf("The bytes_read: %ld\n", bytes_read);
  if(bytes_read == -1){
    fprintf(stderr, "Error while reading data from the file.\n");
    exit(1);
  }

  /*-------------Write part---------------*/
  int bytes_written = write(fd_to, buf, size);
  if(bytes_written == -1){
    fprintf(stderr, "Error while writting data to the file.\n");
    exit(1);
  }
  return;
}

void copy_mmap(int fd_from, int fd_to){
  printf("mmap\n");
  return;
}

int open_file(char* filename){ /*Opens file iff the given file exist*/
  int pfd; //the file discriptor
  pfd = open(filename, O_RDONLY);
  if(pfd == -1){
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
  if((pfd = open(filename, O_WRONLY | O_CREAT | O_TRUNC)) == -1){
    fprintf(stderr, "cannot open giver file.\n");
    exit(1);
  }
  else{
    printf("File was opened properly\n");
  }
  return pfd;
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

  int file1 = open_file(argv[1 + mark_m]); /*keeps the source file descriptor*/
  int file2 = open_destination_file(argv[2 + mark_m]); /*keeps the destination file descriptor*/
  
  if(!mark_m){
    copy_read_write(file1, file2);
  }
  else{
    copy_mmap(file1, file2);
  }

  /* closing the file descriptors*/
  if(close(file1) == 0)
    printf("source file was closed properly\n");
  if(close(file2) == 0)
    printf("destination file was closed properly\n");

  return 0;
}
