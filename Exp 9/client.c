// concurrent time server

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8884
#define BUF_SIZE 1024
#define IP_ADDR "0.0.0.0"

void main() {
  int client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket < 0) {
    printf("Error: socket\n");
    exit(EXIT_FAILURE);
  }
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr = inet_addr(IP_ADDR);

  int connect_status = connect(client_socket, (struct sockaddr*)&server_address,
                               sizeof(server_address));
  if (connect_status < 0) {
    printf("Error: connect\n");
    exit(EXIT_FAILURE);
  }

  char operation[100];
  char time[100];

  while (1) {
    strcpy(operation, "");
    strcpy(time, "");
    scanf("%s", operation);
    if (strcmp(operation, "stop") == 0) {
      break;
    } else {
      send(client_socket, operation, sizeof(operation), 0);
      recv(client_socket, time, sizeof(time), 0);
      printf("current server time is %s\n", time);
    }
  }
}