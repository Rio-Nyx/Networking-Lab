#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8000

int main() {
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    printf("Error: socket\n");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr = inet_addr("0.0.0.0");

  int bind_status = bind(server_socket, (struct sockaddr*)&server_address,
                         sizeof(server_address));
  if (bind_status < 0) {
    printf("Error: bind\n");
    exit(EXIT_FAILURE);
  }

  listen(server_socket, 10);

  int pids[4];
  int chopstick[4];

  int message[2];
  int i = 0;

  for (int i = 0; i < 4; i++) chopstick[i] = 0;
  while (1) {
    if (i >= 4) {
      break;
    }
    int client_socket1 = accept(server_socket, NULL, NULL);
    recv(client_socket1, &message, sizeof(message), 0);
    printf("The message for 1 is %d\n ", message[0]);

    pids[i] = message[0];

    i++;
    printf("%d\n", i);
  }

  int pid_low = pids[0];

  for (int i = 1; i < 4; i++) {
    if (pid_low > pids[i]) pid_low = pids[i];
    printf("%d \n\n", pids[i]);
  }
  printf("The samllest pid value %d\n", pid_low);
  while (1) {
    int client_socket1 = accept(server_socket, NULL, NULL);
    recv(client_socket1, &message, sizeof(message), 0);

    int philospher_num = message[0] - pid_low;

    if (message[1] == 1) {
      if (chopstick[philospher_num] == 0 &&
          chopstick[(philospher_num + 1) % 4] == 0) {
        printf("philosopher %d has obtained the chopstick \n", philospher_num);
        chopstick[philospher_num] = 1;
        chopstick[(philospher_num + 1) % 4] = 1;
      }

      else {
        printf("%d cant obtain the chopstick\n", philospher_num);
      }

    } else if (message[1] == 0) {
      if (chopstick[philospher_num] == 1 &&
          chopstick[(philospher_num + 1) % 4] == 1) {
        printf("philosopher %d has put down the chopstick \n", philospher_num);
        chopstick[philospher_num] = 0;
        chopstick[(philospher_num + 1) % 4] = 0;
      } else {
        chopstick[philospher_num] = 0;
        chopstick[(philospher_num + 1) % 4] = 0;
      }
    }
  }
  close(server_socket);
}
