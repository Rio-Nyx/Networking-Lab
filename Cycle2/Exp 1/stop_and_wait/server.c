#include "../../socket_headers.h"

typedef struct frame {
  int frame_kind;  // ACK/FRAME:0, FIN:1
  int sq_no;       // Two sq_no : 1 and 0
  int ack;
  char data[1024];
} Frame;

int create_socket(struct sockaddr_in *server_address) {
  int network_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (network_socket < 0) {
    printf("socket creation failed\n");
    exit(EXIT_FAILURE);
  }

  // define the server address
  server_address->sin_family = AF_INET;
  server_address->sin_port = htons(PORT);
  server_address->sin_addr.s_addr = INADDR_ANY;
  return network_socket;
}

int bind_listen_client(int server_socket, struct sockaddr_in *server_address) {
  if (bind(server_socket, (struct sockaddr *)server_address,
           sizeof(*server_address)) < 0) {
    printf("Socket cannot be binded to port\n");
    exit(EXIT_FAILURE);
  }
  listen(server_socket, 3);
  int client_socket = accept(server_socket, NULL, NULL);
  return client_socket;
}

void display(Frame frame) {
  printf("frame sequence number: %d\n", frame.sq_no);
  printf("frame content is %s \n", frame.data);
}

void main() {
  struct sockaddr_in server_address;
  int server_socket = create_socket(&server_address);
  printf("%d", server_address.sin_port);
  int client_socket = bind_listen_client(server_socket, &server_address);

  Frame frame_send, frame_recv;
  // frame_send.frame_kind = 1;
  int frame_no = 0;
  frame_send.sq_no = frame_no;
  frame_send.ack = 0;
  while (1) {
    int size = recv(client_socket, &frame_recv, sizeof(frame_recv), 0);
    if (size > 0 && frame_recv.frame_kind == 0) {
      frame_send.ack = 1;
      frame_no++;
      frame_send.sq_no = (frame_recv.sq_no + 1) % 2;
      send(client_socket, &frame_send, sizeof(frame_send), 0);
      display(frame_recv);

    } else if (size <= 0) {
      frame_send.ack = 0;
      send(client_socket, &frame_send, sizeof(frame_send), 0);
    } else {
      frame_send.frame_kind = 1;
      frame_send.ack = 1;
      frame_send.sq_no = (frame_recv.sq_no + 1) % 2;
      send(client_socket, &frame_send, sizeof(frame_send), 0);
    }
  }
  close(client_socket);
  close(server_socket);
  printf("Number of frames send = %d\n", frame_no);
}