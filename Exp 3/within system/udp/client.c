#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF 1024
#define PORT 8080

int main() {
  int network_socket = socket(AF_INET, SOCK_DGRAM, 0);

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
    printf("Connection couldn't be established\n");
    exit(EXIT_FAILURE);
  }

  char server_response[BUF];
  char message[] = "client message";
  int len = sizeof(server_address);

  sendto(network_socket, message, strlen(message), 0,
         (struct sockaddr*)&server_address, len);
  recvfrom(network_socket, server_response, BUF, 0,
           (struct sockaddr*)&server_address, &len);
  printf("The received message from server is : %s \n", server_response);

  return 0;
}
