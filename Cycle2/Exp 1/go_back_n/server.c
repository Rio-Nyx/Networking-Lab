#include "../../socket_functions.h"
#include "../../socket_headers.h"

#define F_SIZE 4

void display(Frame frame) {
  printf("frame sequence number: %d\n", frame.sq_no);
  printf("frame content is %s \n", frame.data);
}

void main() {
  struct sockaddr_in server_address;
  int server_socket = create_socket(&server_address);
  printf("%d", server_address.sin_port);
  int client_socket = bind_listen_client(server_socket, &server_address);

  Frame frame_send, frame_recv[F_SIZE];
  // frame_send.frame_kind = 1;
  int frame_no = 0;
  frame_send.sq_no = frame_no;
  frame_send.ack = 1;
  int size[F_SIZE];
  for (int i = 0; i < F_SIZE; i++) {
    size[i] = recv(client_socket, &frame_recv[i], sizeof(frame_recv[i]), 0);
  }
  int i = 0;
  while (1) {
    printf("Inside");
    i %= F_SIZE;
    if (size[i] > 0 && frame_recv[i].frame_kind == 0) {
      frame_send.ack = 1;
      frame_no++;
      frame_send.sq_no = (frame_recv[i].sq_no + 1) % 4;
      send(client_socket, &frame_send, sizeof(frame_send), 0);
      size[i] = recv(client_socket, &frame_recv[i], sizeof(frame_recv[i]), 0);

      display(frame_recv[i]);

    } else if (size[i] <= 0) {
      frame_send.ack = 0;
      send(client_socket, &frame_send, sizeof(frame_send), 0);

      size[i] = recv(client_socket, &frame_recv[i], sizeof(frame_recv[i]), 0);

    } else {
      frame_send.frame_kind = 1;
      frame_send.ack = 1;
      frame_send.sq_no = (frame_recv[i].sq_no + 1) % 2;
      send(client_socket, &frame_send, sizeof(frame_send), 0);
    }
  }
  close(client_socket);
  close(server_socket);
  printf("Number of frames send = %d\n", frame_no);
}