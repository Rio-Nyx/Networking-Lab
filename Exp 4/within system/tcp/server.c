#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 9876
#define N 5

int main() {
  int message[3];
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
    printf("Socket cannot be bind to port\n");
    exit(EXIT_FAILURE);
  }

  // listening for connection
  listen(server_socket,
         3);  // specifies the no of clients that can server can listen to

  // accepting connection
  int client_socket = accept(server_socket, NULL, NULL);
  int server_response[N];

  recv(client_socket, &server_response, sizeof(server_response), 0);
  int sum = 0;
  int MI = server_response[0];
  int MA = server_response[0];
  for (int i = 0; i < N; i++) {
    if (server_response[i] > MA)
      MA = server_response[i];
    else if (server_response[i] < MI)
      MI = server_response[i];
    sum += server_response[i];
  }
  sum = sum / N;
  message[0] = MA;
  message[1] = MI;
  message[2] = sum;
  sleep(2);
  send(client_socket, message, sizeof(message), 0);

  return 0;
}
