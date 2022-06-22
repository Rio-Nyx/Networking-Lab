#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUF 1024
#define PORT 8080

int main() {
  char message[] = "Connection Established Sucessfully";
  int server_socket = socket(AF_INET, SOCK_DGRAM, 0);

  // define the server address
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr =
      INADDR_ANY;  // communication bw the same machine

  // binding the socket with specified IP
  int bind_status = bind(server_socket, (struct sockaddr*)&server_address,
                         sizeof(server_address));
  if (bind_status < 0) printf("Port not binded to socket\n");

  // listening for connection
  // specifies the no of clients that can server can listen to
  // accepting connection
  char server_response[BUF];
  int len = sizeof(server_address);

  recvfrom(server_socket, &server_response, BUF, 0,
           (struct sockaddr*)&server_address, &len);
  printf("Message from client : %s\n", server_response);
  sendto(server_socket, message, strlen(message), 0,
         (struct sockaddr*)&server_address, len);
  return 0;
}
