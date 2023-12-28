#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUF_SIZE 1200
typedef struct Data{
    int sockfd;
    char *file_down;
}Data;


void *download_file(void *arg);
void traitement(char *filename,char *ip,char *port);
int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <IP> <port> <file download>\n", argv[0]);
        exit(1);
    }
    traitement(argv[3],argv[1],argv[2]);
    return 0;
}

void *download_file(void *arg) {
    Data *args = (Data *)arg;

    char filename[BUF_SIZE];
    sprintf(filename, "downloaded%s",args->file_down);

    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    char buf[BUF_SIZE];
    int bytes_received;
    printf("en cours de telechargement.");
    while ((bytes_received = recv(args->sockfd, buf, BUF_SIZE, 0)) > 0) {
        fwrite(buf, sizeof(char), bytes_received, fp);
        printf("=");
    }
    printf("=>\n");
    fclose(fp);
    printf("File downloaded successfully.\n");

    pthread_exit(NULL);
}

void traitement(char *filename,char *ip,char *port){

     int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(atoi(port));

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(1);
    }
    Data data={0};
    data.file_down=filename;
    data.sockfd=sock;

    if (send(sock, filename, strlen(filename)+1, 0) == -1) {
        perror("send");
        exit(1);
    }

    pthread_t tid;
    pthread_create(&tid, NULL, download_file, (void *)&data);
    pthread_join(tid, NULL);

    close(sock);
}