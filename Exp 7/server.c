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

static int server_socket;
int clientfd[100] = {-1};
static int clientCount = 0;

void *recieveMsg(void *arg){
  int sockfd = *(int *)arg;
  char message[1024];
  while(1){
    recv(sockfd,message,1024,0);
    int i;
    for(i=0;i<clientCount;i++){
      send(clientfd[i],message,sizeof(message),0);
    }  
    printf("%d is count\n",i);
  }
  strcpy(message,"");
}

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *acceptClient(void *arg){
  while(1){
    clientfd[clientCount] = accept(server_socket,NULL,NULL);
    if(clientfd[clientCount]>0){
    printf("Accepted client %d\n",clientCount);
      pthread_t t ;
                  int c=clientfd[clientCount];

      pthread_create(&t,NULL,recieveMsg,(void *)&c);
      pthread_mutex_lock(&lock);
      clientCount++;
      pthread_mutex_unlock(&lock);
    }
  }
}

void main(){
  server_socket = socket(AF_INET,SOCK_STREAM,0);
  if(server_socket<0){
    printf("Error: socket\n");
    exit(EXIT_FAILURE);
  }
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr = INADDR_ANY;
  
  int bind_status = bind(server_socket,(struct sockaddr*)&server_address,sizeof(server_address));
  if(bind_status<0){
    printf("Error: bind\n");
    exit(EXIT_FAILURE);
  }
  
  listen(server_socket,3);
  pthread_t tid;
  pthread_create(&tid,NULL,acceptClient,NULL);
  pthread_join(tid,NULL);
  
  close(server_socket);
}
