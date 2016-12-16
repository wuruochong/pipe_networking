#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

/*#define READ 0
#define WRITE 1
*/
#define MESSAGE_BUFFER_SIZE 256

int client_handshake(int * p){
  umask(0);
  int fd = open("mario", O_WRONLY);
  *p = fd;
  printf("WKP connected, creating private FIFO\n");
  char fn[MESSAGE_BUFFER_SIZE];
  sprintf(fn, "%d", getpid());
  printf("Private FIFO:%s\n", fn);
  int r = mkfifo(fn, 0644);
  if (r){
    printf("error: %s\n", strerror(errno));
    return;
  }
  write(fd, fn, MESSAGE_BUFFER_SIZE);
  printf("Sent private FIFO name through WKP\n");

  printf("Opening private FIFO, waiting for response\n");
  int fd2 = open(fn, O_RDONLY);
  
  char buf[MESSAGE_BUFFER_SIZE];
  read(fd2, buf, MESSAGE_BUFFER_SIZE);
  printf("Message received: %s\n", buf);
  int f = fork();
  if( f == 0 ){
    execlp("rm","rm", fn, NULL);
    exit(0);
  }else{
    printf("Private FIFO removed\n");
  }
  char buf2[MESSAGE_BUFFER_SIZE] = "howdy?";
  write(fd, buf2,MESSAGE_BUFFER_SIZE);
  printf("Confirmation message sent\n");
  return fd2;
}

int server_handshake(int * p){
    umask(0);
    int r = mkfifo("mario", 0644);
    if (r){
      printf("error: %s\n", strerror(errno));
      return;
    }
    printf("WKP created, listening for connection\n");
    int fd = open("mario",O_RDONLY);
    *p = fd;
    printf("Connection Attempt\n");
    char buf[MESSAGE_BUFFER_SIZE];
    read(fd, buf, MESSAGE_BUFFER_SIZE);
    printf("Message received, Private FIFO: %s\n", buf);
    
    int f = fork();
    if ( f == 0 ){
      execlp("rm","rm","mario", NULL);
    }    
    printf("WKP removed\n");

    printf("Accessing private FIFO\n");    
    int fd2 = open(buf, O_WRONLY);
    printf("Private FIFO Opened\n");
    char buf2[MESSAGE_BUFFER_SIZE] = "hello";
    write(fd2, buf2, MESSAGE_BUFFER_SIZE);
    printf("Message sent\n");
    read(fd, buf, MESSAGE_BUFFER_SIZE);
    printf("Message received: %s\n", buf);
    return fd2;
}

int server_handshake1(int * p, char * buffer ){
  umask(0);
  int r = mkfifo("mario", 0644);
  if (r){
    printf("error: %s\n", strerror(errno));
    return;
  }
  printf("WKP created, listening for connection\n");
  int fd = open("mario",O_RDONLY);
  *p = fd;
  printf("Connection Attempt\n");
  read(fd, buf, MESSAGE_BUFFER_SIZE);
  printf("Message received, Private FIFO: %s\n", buf);
}


/*int main() {

  int fds[2];

  pipe( fds );


  int f = fork();
  if ( f == 0 ) {
    close(fds[WRITE]);

    printf("[child] listening\n");
    char s[20];
    //int x;
    read( fds[READ], s, sizeof(s) );
    printf("[child] got: %s\n", s);
  }
  else {
    close(fds[READ]);

    printf("[parent] sleeping\n");
    sleep(5);
    write( fds[WRITE], "hello there", 12 );
  }
  return 0;
}
*/
