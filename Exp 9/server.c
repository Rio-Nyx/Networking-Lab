// concurrent time server

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define PORT 8884
#define BUF_SIZE 1024
#define IP_ADDR "0.0.0.0"

static int server_socket;
void *handleRequest(void *arg) {
  int sockfd = *(int *)arg;
  char message[100];
  char time_msg[100];
  while (1) {
    strcpy(message, "");
    recv(sockfd, message, sizeof(message), 0);
    if (strcmp(message, "GET_time") == 0) {
      time_t curtime;
      time(&curtime);
      strcpy(time_msg, ctime(&curtime));
      send(sockfd, time_msg, sizeof(time_msg), 0);
      printf("recieved message from %d\n", sockfd);
    }
  }
}
void *acceptClient(void *arg) {
  while (1) {
    int clientfd = accept(server_socket, NULL, NULL);
    if (clientfd > 0) {
      printf("Accepted client\n");
      pthread_t t;
      pthread_create(&t, NULL, handleRequest, (void *)&clientfd);
    }
  }
}
void main() {
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    printf("Error: socket\n");
    exit(EXIT_FAILURE);
  }
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr = INADDR_ANY;

  int bind_status = bind(server_socket, (struct sockaddr *)&server_address,
                         sizeof(server_address));
  if (bind_status < 0) {
    printf("Error: bind\n");
    exit(EXIT_FAILURE);
  }

  listen(server_socket, 10);
  pthread_t tid;
  pthread_create(&tid, NULL, acceptClient, NULL);
  pthread_join(tid, NULL);
  close(server_socket);
}