#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 9876

int main() {
  int network_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (network_socket < 0) {
    printf("socket creation failed\n");
    exit(EXIT_FAILURE);
  }

  // define the server address
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr =
      INADDR_ANY;  // communication between the same machine

  int status = connect(network_socket, (struct sockaddr*)&server_address,
                       sizeof(server_address));
  // gives the status of the connection
  if (status == -1) {
    printf("Connection couldn't be established");
  }

  char server_response[100];
  if (recv(network_socket, &server_response, sizeof(server_response), 0) <= 0) {
    printf("message cannot be send\n");
    exit(EXIT_FAILURE);
  }
  printf(" The received message is =%s\n", server_response);
  close(network_socket);

  return 0;
}
