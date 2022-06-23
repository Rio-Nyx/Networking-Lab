#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define PORT 9876
#define BUF 1024

int main() {
  int network_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (network_socket < 0) {
    printf("socket creation failed\n");
    exit(EXIT_FAILURE);
  }
  // define the server address
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(9876);
  server_address.sin_addr.s_addr =
      INADDR_ANY;  // communication between the same machine

  int status = connect(network_socket, (struct sockaddr*)&server_address,
                       sizeof(server_address));
  // gives the status of the connection
  if (status == -1) {
    printf("Connection couldn't be established");
  }

  int server_response[1];
  recv(network_socket, &server_response, sizeof(server_response), 0);
  printf(" The received message is :square = %d\n", server_response[0]);

  return 0;
}
