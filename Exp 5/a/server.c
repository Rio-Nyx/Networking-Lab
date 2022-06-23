#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 9876
#define BUF 1024

int main() {
  char message[BUF];
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
  if (bind(server_socket, (struct sockaddr*)&server_address,
           sizeof(server_address)) < 0) {
    printf("Socket cannot be binded to port\n");
    exit(EXIT_FAILURE);
  }

  // listening for connection
  listen(server_socket,
         3);  // specifies the no of clients that can server can listen to

  // accepting connection
  int client_socket = accept(server_socket, NULL, NULL);
  char server_response[BUF];

  recv(client_socket, &server_response, sizeof(server_response), 0);
  printf("recieved string from client1 %s\n", server_response);
  int client2 = accept(server_socket, NULL, NULL);

  int len = strlen(server_response);
  printf("recieved string length : %d\n", len);
  for (int i = 0; i < len; i++) {
    message[len - i - 1] = server_response[i];
  }
  sleep(2);
  send(client2, message, sizeof(message), 0);
  close(client2);
  close(client_socket);
  close(server_socket);
  return 0;
}
