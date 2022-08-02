#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 8884
#define BUF_SIZE 1024
#define IP_ADDR "0.0.0.0"
static int client_socket ;
void *readMsg(void *arg){
  while(1){
    char msg[100]; fflush(stdout);
    int s = recv(client_socket,msg,100,0);
    if(strlen(msg)>0)
    printf("msg %d recieved %s\n",s,msg);
      strcpy(msg,"");      fflush(stdout);

  }
}

void *sendMsg(void *arg){
  char msg[100];
  int started = 0;
  while(1){            fflush(stdin);  fflush(stdout);

    strcpy(msg,"");
    scanf("%s",msg);
    if(started==0){if(strcmp(msg,"start")==0){started=1;printf("started\n");}}
    else{
      if(strcmp(msg,"stop")==0)
        exit(0);
      else{send(client_socket,msg,100,0);printf("message send\n");}
    }
  }
}

void main(){
  client_socket = socket(AF_INET,SOCK_STREAM,0);
  if(client_socket<0){
    printf("Error: socket\n");
    exit(EXIT_FAILURE);
  }
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr = inet_addr(IP_ADDR);
  
  int connect_status = connect(client_socket,(struct sockaddr*)&server_address,sizeof(server_address));
  if(connect_status<0){
    printf("Error: connect\n");
    exit(EXIT_FAILURE);
  }
  
  pthread_t tid1, tid2;
  char message[BUF_SIZE];
  pthread_create(&tid1,NULL,sendMsg,NULL);
  pthread_create(&tid2,NULL,readMsg,NULL);
  
  pthread_join(tid1,NULL);
  pthread_join(tid2,NULL);
  
  
  close(client_socket);
}
