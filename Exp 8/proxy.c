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
  FILE *fp;

  char url[50];

  struct sockaddr_in server_addr, client_addr;

  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd < 0) {
    printf("Error: socket creation\n");
    exit(EXIT_FAILURE);
  }
  printf("Started proxy server \n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    printf("Error: bind\n");
    exit(EXIT_FAILURE);
  }

  listen(server_fd, 3);

  int size = sizeof(struct sockaddr_in);

  client_fd =
      accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&size);

  if (client_fd < 0) {
    printf("Error: accept\n");
    exit(EXIT_FAILURE);
  }

  recv(client_fd, url, 1000 * sizeof(char), 0);
  printf("request recieved from client\n");
  char buffer[1000];
  strcpy(buffer, "");
  strcat(buffer, "wget -qO - ");
  strcat(buffer, url);
  fp = popen(buffer, "r");
  printf("retrieving data from %s\n", url);

  char message[1000];
  fseek(fp, 0, SEEK_SET);

  fread(message, 1000, 1, fp);
  printf("sending data to client\n");
  send(client_fd, message, sizeof(message), 0);
  printf("data send\n");
  close(client_fd);
  close(server_fd);
}
