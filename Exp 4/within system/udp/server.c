#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF 1024
#define PORT 9876
#define N 5

int main() {
  int message[3];
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);

  // define the server address
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr =
      INADDR_ANY;  // communication bw the same machine

  // binding the socket with specified IP
  bind(server_socket, (struct sockaddr*)&server_address,
       sizeof(server_address));

  // accepting connection
  int server_response[N];
  int sum = 0;
  int MI = 100000;
  int MA = -10000;
  // recv(client_socket,& server_response,sizeof(server_response),0);
  int len = sizeof(server_address);

  recvfrom(server_socket, &server_response, BUF, 0,
           (struct sockaddr*)&server_address, &len);
  printf("recieved data from client\nPerforming Calculation...\n");
  for (int i = 0; i < N; i++) {
    if (server_response[i] > MA)
      MA = server_response[i];
    else if (server_response[i] < MI)
      MI = server_response[i];
    sum += server_response[i];
  }
  message[0] = MA;
  message[1] = MI;
  message[2] = sum;
  sleep(2);
  sendto(server_socket, message, strlen(message), 0,
         (struct sockaddr*)&server_address, len);
  printf("Result send to client\n");
  return 0;
}
