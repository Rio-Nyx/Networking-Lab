#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PORT 8080
#define BUF 1024

int main() {
  char message[] = "Connection Established Sucessfully";
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    printf("socket creation failed\n");
    exit(EXIT_FAILURE);
  }

  // define the server address
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr =
      INADDR_ANY;  // communication bw the same machine

  // binding the socket with specified IP
  int bind_status = bind(server_socket, (struct sockaddr*)&server_address,
                         sizeof(server_address));

  if (bind_status < 0) printf("Socket not binded to port\n");

  // listening for connection
  listen(server_socket,
         3);  // specifies the no of clients that can server can listen to

  // accepting connection
  int client_socket = accept(server_socket, NULL, NULL);
  char server_response[BUF];
  recv(client_socket, &server_response, sizeof(server_response), 0);
  printf("message recieved from client is %s\n", server_response);
  return 0;
}
