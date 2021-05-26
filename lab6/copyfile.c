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
  struct stat st;
  fstat(fd_from, &st);
  int size = st.st_size;
  char *from, *to; //pointer to shared memory object
  printf("size: %d\n", size);
  ftruncate(fd_to, size); //adjusting the size of the file to the desired value
  //  char* buf = (char*)calloc(size,  sizeof(char));
  /*
    PROT_WRITE/PORT_READ - pages may be written/read
    MAP_PRIVATE - create a private copy-on-write mapping
    MAP_SHARED - share this mapping
   */
  from = mmap(0, size, PROT_READ, MAP_PRIVATE, fd_from, 0);
  to = mmap(0, size, PROT_WRITE, MAP_SHARED, fd_to, 0);
  
  memcpy(to, from, size);
  
  munmap(from, size); //unmapping the source file
  munmap(to, size); //unmapping the destination file
  return;
}

int open_file(char* filename){ /*Opens file iff the given file exist*/
  int pfd; //the file discriptor
  pfd = open(filename, O_RDONLY);
  if(pfd == -1){
    fprintf(stderr, "Cannot open given file.\n");
    exit(1);
  }
  else{
    printf("File was opened properly\n");
  }
  return pfd;
}

int open_destination_file(char* filename){ /*Opens or creates file and change it size to 0 of it exist*/
  int pfd;
  if((pfd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1){
    fprintf(stderr, "cannot open given file.\n");
    exit(1);
  }
  else{
    printf("File was opened properly\n");
  }
  return pfd;
}

void help(){
  printf("This program copies one file into the another file.\n");
  printf("Usage:\n");
  printf("./copyfile <source file> <destination file> - copy one file to another file with the posix read/write funcitons :\n");
  printf("./copyfile -m <source file> <destination file> - map files to memory regions with mmap() and then copy them with the memcpy functions\n");
}


int main(int argc, char** argv){
  int opt;
  int mark_m = 0; //check if -m flag was set
  while(( opt = getopt(argc, argv, ":mh" )) != -1 ){ //parsing flags only
    switch(opt){
    case 'm':
      mark_m = 1;
      break;
    case 'h':
      help();
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
