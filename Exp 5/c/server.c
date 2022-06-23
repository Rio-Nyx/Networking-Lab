#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

int main(){
  float message[1];
  int server_socket=socket(AF_INET,SOCK_STREAM,0);

  //define the server address
  struct sockaddr_in server_address;
  server_address.sin_family=AF_INET;
  server_address.sin_port=htons(9876);
  server_address.sin_addr.s_addr=INADDR_ANY;  //communication bw the same machine

  //binding the socket with specified IP
  bind(server_socket,(struct sockaddr*) &server_address, sizeof(server_address));

  //listening for connection 
  listen(server_socket,3); //specifies the no of clients that can server can listen to
  
  //accepting connection
  int client_socket= accept(server_socket ,NULL,NULL);
  printf("%d",client_socket);
  float server_response[1];

 	recv(client_socket,& server_response,sizeof(server_response),0);
 	client_socket= accept(server_socket ,NULL,NULL);
  	message[0] = pow(server_response[0],1.5);
   printf("%d",client_socket);
  	sleep(2);
  	send(client_socket,message,sizeof(message),0);
 
  return 0;
  
}
