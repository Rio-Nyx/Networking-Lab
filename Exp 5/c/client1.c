#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include <netinet/in.h>
#include<string.h>
#include<sys/socket.h>

int main(){

  int network_socket=socket(AF_INET,SOCK_STREAM,0);
  //define the server address
  struct sockaddr_in server_address;
  server_address.sin_family=AF_INET;
  server_address.sin_port=htons(9876);
  server_address.sin_addr.s_addr=INADDR_ANY;  //communication between the same machine

  int status=connect(network_socket,(struct sockaddr*)&server_address,sizeof(server_address));
  //gives the status of the connection
  if(status==-1){
     printf("Connection couldn't be established");
  }

 	 float server_response[1];
  
	float message[] = {10};
  	send(network_socket,message,sizeof(message),0);

  	
  return 0;
}
