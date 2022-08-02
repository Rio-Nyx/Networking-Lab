#include "socket_headers.h"

int create_socket(struct sockaddr_in *server_address) {
  int network_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (network_socket < 0) {
    printf("socket creation failed\n");
    exit(EXIT_FAILURE);
  }

  server_address->sin_family = AF_INET;
  server_address->sin_port = htons(PORT);
  server_address->sin_addr.s_addr = INADDR_ANY;
  return network_socket;
}

typedef struct frame {
  int frame_kind;  // ACK/FRAME:0, FIN:1
  int sq_no;       // Two sq_no : 1 and 0
  int ack;
  char data[1024];
} Frame;

int bind_listen_client(int server_socket, struct sockaddr_in *server_address) {
  if (bind(server_socket, (struct sockaddr *)server_address,
           sizeof(*server_address)) < 0) {
    printf("Socket cannot be binded to port\n");
    exit(EXIT_FAILURE);
  }
  listen(server_socket, 3);
  int client_socket = accept(server_socket, 0, 0);
  return client_socket;
}