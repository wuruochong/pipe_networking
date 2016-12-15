#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <pipe_networking.h>


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
  itoa(getpid(),fn, MESSAGE_BUFFER_SIZE);
  int r = mkfifo(fn, 0644);
  if (r){
    printf("error: %s\n", strerror(errno));
    return;
  }
  write(fd, fn, sizeof(*fn));
  printf("Sent private FIFO name through WKP\n");
  int fd2 = open("fn", O_RDONLY);
  printf("Opened private FIFO, waiting for response\n");
  char buf[MESSAGE_BUFFER_SIZE];
  read(fd2, buf, sizeof(*buf));
  printf("Message received: %s, removing private FIFO\n", buf);
  execlp("rm","rm",fn, NULL);
  char buf2[MESSAGE_BUFFER_SIZE] = "howdy?";
  write(fd, buf2, sizeof(*buf2));
  printf("Confirmation message sent");
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
    printf("Connection established\n");
    char buf[MESSAGE_BUFFER_SIZE];
    read(fd, buf, sizeof(*buf));
    printf("Message received private FIFO: %s", buf);
    execlp("rm","rm","mario", NULL);
    printf("WKP removed\n");
    int fd2 = open(*buf, O_WRONLY);
    char buf2[MESSAGE_BUFFER_SIZE] = "hello";
    write(fd2, buf2, sizeof(*buf2));
    printf("Message sent\n");
    read(fd, buf, sizeof(*buf));
    printf("Message received: %s\n", buf);
    return fd2;
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
