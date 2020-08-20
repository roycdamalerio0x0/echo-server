
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int main( int argc, char *argv[] ) {

  int sockfd, 
      status;
  int msg_len,
      bytes_sent,
      bytes_recv;

  struct addrinfo hints,
                  *info,
                  *p;
  char *msg;


  if ( argc < 3 ) {
    printf("Usage %s [IP ADDRESS] [PORT] [MESSAGE]", argv[0]);
    exit(EXIT_FAILURE);
  }
 
  memset(&msg, '\0', sizeof msg);
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if((status = getaddrinfo(argv[1], argv[2], &hints, &info)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(EXIT_FAILURE);
  }

  if( (sockfd = socket(info->ai_family, info->ai_socktype, info->ai_protocol)) < 0 ) {
    perror("Could not create socket\n");
    exit(EXIT_FAILURE);
  }

  if( connect(sockfd, info->ai_addr, info->ai_addrlen ) > 0) {
    printf("Connected to Echo server ...\n");
  }


  msg = argv[3];
  msg_len = strlen(msg)+1;
  bytes_sent = send( sockfd, msg, msg_len, 0 );
  if( bytes_sent != -1 )
    printf("[*]Bytes sent to Echo server: %d\n", bytes_sent);

  if( (bytes_recv = recv(sockfd, msg, bytes_sent, 0)) != -1 )
    printf("[*]Received bytes: %d\n[*]Received data: %s", bytes_recv, msg);

  close(sockfd);
  freeaddrinfo(info);
}
