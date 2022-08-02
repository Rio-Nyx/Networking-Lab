#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8000

void create_connection() {
  int network_socket;
  network_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (network_socket < 0) {
    printf("Error: socket\n");
    exit(EXIT_FAILURE);
  }
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr = inet_addr("0.0.0.0");

  int connection_status =
      connect(network_socket, (struct sockaddr *)&server_address,
              sizeof(server_address));

  if (connection_status < 0) {
    printf("Error: connection\n");
    exit(EXIT_FAILURE);
  }

  int message[2];
  message[0] = getpid();
  message[1] = -1;

  printf("\n\n%d\n\n", message[0]);
  send(network_socket, message, sizeof(message), 0);
}

void getchopstick() {
  int network_socket;
  network_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (network_socket < 0) {
    printf("Error: socket\n");
    exit(EXIT_FAILURE);
  }
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr = inet_addr("0.0.0.0");

  int connection_status =
      connect(network_socket, (struct sockaddr *)&server_address,
              sizeof(server_address));

  if (connection_status == -1) {
    printf("Error: connection\n");
    exit(EXIT_FAILURE);
  }

  int message[2];
  message[0] = getpid();
  message[1] = 1;
  send(network_socket, message, sizeof(message), 0);

  close(network_socket);
}

void putchopstick() {
  int network_socket;
  network_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (network_socket < 0) {
    printf("Error: socket\n");
    exit(EXIT_FAILURE);
  }
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr = inet_addr("0.0.0.0");

  int connection_status =
      connect(network_socket, (struct sockaddr *)&server_address,
              sizeof(server_address));

  if (connection_status < 0) {
    printf("Error: connect\n");
    exit(EXIT_FAILURE);
  }

  int message[2];
  message[0] = getpid();
  message[1] = 0;
  send(network_socket, message, sizeof(message), 0);
  close(network_socket);
}

int main() {
  fork();
  fork();

  create_connection();

  getchopstick();
  sleep(3);
  putchopstick();
  sleep(3);
  getchopstick();
  sleep(3);
}
