#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define port 8080

void main() {
  int server_fd, client_fd;
  int opt = 1;

  char msg[50];

  struct sockaddr_in server_addr, client_addr;

  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd < 0) {
    printf("Error: socket creation\n");
    exit(EXIT_FAILURE);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (connect(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    printf("Error: connect\n");
    exit(EXIT_FAILURE);
  }
  char url[100];
  printf("Enter url: ");
  scanf("%s", url);
  send(server_fd, url, sizeof(url), 0);
  char message[1000];
  recv(server_fd, message, 1000 * sizeof(char), 0);

  printf("Message from client is : %s\n", message);

  close(server_fd);
}
