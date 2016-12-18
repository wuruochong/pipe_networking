#define MESSAGE_BUFFER_SIZE 256
#define HANDSHAKE_BUFFER_SIZE 256
int client_handshake(int * p);
int server_handshake(int * p);
int server_handshake1( char * buffer );
int server_handshake2( char * buffer, int from_client );
