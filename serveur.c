#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#define PORT 1234
#define MAX_BUFFER_SIZE 1024

void share_data(){

    int server_fd, new_socket, valread;
    struct sockaddr_in address,client_addr;
    int addrlen = sizeof(address);
    char buffer[MAX_BUFFER_SIZE] = {0};

    // création du socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    // configuration de l'adresse et du port du serveur
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // attachement du socket à l'adresse et au port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("Erreur lors de l'attachement du socket à l'adresse et au port");
        exit(EXIT_FAILURE);
    }

    // mise en écoute du socket
    if (listen(server_fd, 3) < 0) {
        perror("Erreur lors de la mise en écoute du socket");
        exit(EXIT_FAILURE);
    }

    printf("Le serveur est prêt à recevoir des demandes de téléchargement...\n");

    // acceptation d'une connexion entrante
    if ((new_socket = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen))<0) {
        perror("Erreur lors de l'acceptation d'une connexion entrante");
        exit(EXIT_FAILURE);
    }

    printf("Connexion établie avec le client %d:%d\n", client_addr.sin_addr.s_addr, client_addr.sin_port);
    char filename[300];
    // réception du nom du fichier demandé
    valread = read(new_socket, filename, MAX_BUFFER_SIZE);
    if (valread < 0) {
        perror("Erreur lors de la réception des données");
        exit(EXIT_FAILURE);
    }
    
    // ouverture du fichier demandé
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        char* message = "Erreur : le fichier demandé est introuvable.";
        send(new_socket, message, strlen(message), 0);
        printf("Le fichier %s est introuvable.\n", filename);
        close(new_socket);
        exit(EXIT_FAILURE);
    }

    // envoi du contenu du fichier au client
    int bytes_read;
    while ((bytes_read = read(fd, buffer, MAX_BUFFER_SIZE)) > 0) {
        send(new_socket, buffer, bytes_read, 0);
    }

    printf("Le fichier %s a été envoyé avec succès.\n", filename);

    close(fd);
    close(new_socket);
    close(server_fd);


}



int main() {
    
    share_data();
    return 0;
}