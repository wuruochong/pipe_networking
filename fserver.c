#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "pipe_networking.h"

void process( char * s );
void sub_server( int from_client, int to_client );
static void sighandler(int signo){
  if (signo == SIGINT){
    printf("SIGINT received, removing WKP and shutting down server\n");
    remove("mario");
    exit(0);
  }
}
int main() {

  int to_client, from_client;
  char buffer[HANDSHAKE_BUFFER_SIZE];

  while (1) {
    signal(SIGINT, sighandler); 
    from_client = server_handshake1( buffer );

    int f = fork();
    if ( f == 0 ) {

      to_client = server_handshake2( buffer, from_client );
      sub_server( from_client, to_client );

      exit(0);
    }else{
      sleep(1);
      signal(SIGINT, sighandler);
    }
  }
  return 0;;
}


void sub_server( int from_client, int to_client ) {

  char buffer[MESSAGE_BUFFER_SIZE];
  while (read( from_client, buffer, sizeof(buffer) )) {

    printf("[SERVER %d] received: %s\n", getpid(), buffer );
    process( buffer );
    write( to_client, buffer, sizeof(buffer));
  }

}
void process( char * s ) {

  while ( *s ) {
    *s = (*s - 'a' + 13) % 26 + 'a';
    s++;
  }
}
