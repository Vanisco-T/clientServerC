#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(){

  char *ip = "192.168.0.105";
  int port = 5566;
  int server_sock, client_sock;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_size;
  char buffer[1024],buffer1[1024];
  int n,n1;
  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock < 0){
    perror("Socket error");
    exit(1);
  }
  printf("[+]TCP server socket created.\n");

  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (n < 0){
    perror("Bind error");
    exit(1);
  }
  printf("Bind to the port number: %d\n", port);

  listen(server_sock, 5);
  printf("Listening...\n");
    addr_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
    printf("Client connected.\n");
  while(1){
        bzero(buffer, 1024);
        n1=read(client_sock,buffer,1024);
        n1=n-1;
        printf("\nClient Send:%s\n",buffer);
        char int_str[n1];
        sprintf(int_str,"%d",n1);
        bzero(buffer1, 1024);
        int j;
        char ch;
        for(j=0;j<n1;j++){
          ch=buffer[j];
          buffer1[j]=toupper(ch);
        }
        strcat(buffer1,"With length: ");
        strcat(buffer1,int_str);
        bzero(buffer,1024);
        strcpy(buffer,buffer1);
        n=write(client_sock,buffer,1024);
        if(!strncmp("bye",buffer,3))
          break;
        
  }
  close(client_sock);
  printf("[+]Client disconnected.\n\n");

  return 0;
}