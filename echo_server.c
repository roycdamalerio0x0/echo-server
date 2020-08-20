
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BACK_LOG 10



int main( int argc, char *argv[]) {

  int sockfd, 
      newfd,
      status;
  int bytes_recv,
      bytes_sent;
  int port;

  struct addrinfo hints,
                  *s_inf;
  struct sockaddr_storage addr;
  socklen_t a_size;

  char buff[1024];
  char ipstr[INET_ADDRSTRLEN];

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if( (status = getaddrinfo(NULL, argv[1], &hints, &s_inf)) != 0 ) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  }

  // Create a socket
  if( (sockfd = socket(s_inf->ai_family, s_inf->ai_socktype, s_inf->ai_protocol)) == -1 ) {
    perror("Error creating socket");
    exit(EXIT_FAILURE);
  }

  // Bind to a port
  if( bind(sockfd, s_inf->ai_addr, s_inf->ai_addrlen) == -1 ) {
    perror("Error binding to port");
    exit(EXIT_FAILURE);
  }
  
  //Let's start listening to
  //incoming connection
  if( listen(sockfd, BACK_LOG) == -1 ) {
    perror("Error occured.. Cannot listen");
    exit(EXIT_FAILURE);
  } 
  
  printf("[*]Listening on port: %s\n", argv[1]);
  
  a_size = sizeof addr;

  if ( (newfd = accept(sockfd, (struct sockaddr *)&addr, &a_size)) == -1 ) {
     perror("Error accept incoming connection");
     exit(EXIT_FAILURE);
  }
  if( getpeername(newfd, (struct sockaddr *)&addr, &a_size) != -1 ) {
     struct sockaddr_in *s = (struct sockaddr_in *)&addr;
     port = ntohs(s->sin_port);
     inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
  } else
     perror("Error getting client's IP ADDRESS");
    
  printf("[*]Sender IP Address: %s\n", ipstr);

      
  if( (bytes_recv = recv(newfd, buff, sizeof buff,0)) != -1 ) {
     printf("[*]Bytes receive from sender: %d\n", bytes_recv);
     printf("[*]Sender message: %s\n", buff);
   
     if( (bytes_sent = send(newfd, buff, bytes_recv, 0)) != -1 )
       printf("[*]Echoed Bytes: %d\n[*]Echoed Message: %s", bytes_sent, buff);
  }

  close(newfd);
  close(sockfd);
  freeaddrinfo(s_inf);
}
