#include "../../socket_functions.h"
#include "../../socket_headers.h"

#define F_SIZE 4

void display(Frame frame, int frame_no) {
  printf("frame sequence number: %d\n", frame.sq_no);
  printf("frame number is %d \n", frame_no);
}

void main() {
  struct sockaddr_in server_address;
  int client_socket = create_socket(&server_address);
  printf("%d", server_address.sin_port);
  if (connect(client_socket, (struct sockaddr *)&server_address,
              sizeof(server_address)) == -1) {
    printf("Connection couldn't be established");
  }
  Frame frame_send[F_SIZE], frame_recv;
  // frame_send.frame_kind = 1;
  int frame_no = 0;

  frame_recv.ack = 1;
  for (int i = 0; i < F_SIZE; i++) {
    frame_send[i].frame_kind = 0;
    frame_send[i].sq_no = frame_no;
    frame_send[i].ack = 1;
    printf("Enter the data to send: ");
    fgets(frame_send[i].data, 1024, stdin);
    send(client_socket, &frame_send[i], sizeof(frame_send[i]), 0);
    frame_no++;
  }
  int i = frame_no;
  while (1) {
    printf("Inside");
    i %= F_SIZE;
    printf("value of i is %d\n", i);
    int size = recv(client_socket, &frame_recv, sizeof(frame_recv), 0);
    if (size > 0 && frame_recv.frame_kind == 0 && frame_recv.ack == 1) {
      display(frame_recv, frame_no);
      // frame_no++;
      frame_send[i].sq_no = (frame_no % 4);
      i++;
    } else if (frame_recv.frame_kind == 1) {
      break;
    } else {
      i -= F_SIZE;
      i++;
      continue;
    }
    if (frame_recv.ack == 1) {
      printf("Enter the data to send: ");
      fgets(frame_send[i].data, 1024, stdin);
      send(client_socket, &frame_send[i], sizeof(frame_send[i]), 0);
      frame_no++;
    } else {
    }
  }
  printf("Total number of frames send = %d\n", frame_no);
  close(client_socket);
}