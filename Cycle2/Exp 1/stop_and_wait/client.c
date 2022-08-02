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

  server_address->sin_family = AF_INET;
  server_address->sin_port = htons(PORT);
  server_address->sin_addr.s_addr = INADDR_ANY;
  return network_socket;
}

void display(Frame frame, int frame_no) {
  printf("got acknowledgement for frame %d\n", frame_no);
  printf("Next frame sequence number: %d\n", frame.sq_no);
}

void main() {
  struct sockaddr_in server_address;
  int client_socket = create_socket(&server_address);
  printf("%d", server_address.sin_port);
  if (connect(client_socket, (struct sockaddr *)&server_address,
              sizeof(server_address)) == -1) {
    printf("Connection couldn't be established");
  }
  Frame frame_send, frame_recv;
  // frame_send.frame_kind = 1;
  int frame_no = 0;
  frame_send.frame_kind = 0;
  frame_send.sq_no = frame_no;
  frame_send.ack = 0;
  frame_recv.ack = 1;
  while (1) {
    if (frame_recv.ack == 1) {
      printf("Enter the data to send: ");
      fgets(frame_send.data, 1024, stdin);
    }
    send(client_socket, &frame_send, sizeof(frame_send), 0);
    int size = recv(client_socket, &frame_recv, sizeof(frame_recv), 0);
    if (size > 0 && frame_recv.frame_kind == 0 && frame_recv.ack == 1) {
      display(frame_recv, frame_no);
      frame_no++;
      frame_send.sq_no = (frame_no % 2);
    } else if (frame_recv.frame_kind == 1) {
      break;
    } else {
      continue;
    }
  }
  printf("Total number of frames send = %d\n", frame_no);
  close(client_socket);
}