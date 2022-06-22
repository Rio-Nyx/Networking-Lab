#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define PORT 8080
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
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr =
      INADDR_ANY;  // communication between the same machine

  int status = connect(network_socket, (struct sockaddr*)&server_address,
                       sizeof(server_address));

  // gives the status of the connection
  if (status == -1) {
    printf("Connection couldn't be established");
    exit(EXIT_FAILURE);
  }
  char server_response[BUF];
  recv(network_socket, &server_response, sizeof(server_response), 0);
  printf(" The received message is : %s \n", server_response);
  return 0;
}
