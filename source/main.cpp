#include <sys/socket.h>
#include <netdb.h>
#include <cstdlib>
#include <unistd.h> // Include the header file for the close function

int main() {
    int serverSocket;
    struct addrinfo hints, *servinfo;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Get the address information of the server
    getaddrinfo(NULL, "8080", &hints, &servinfo);

    serverSocket = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    bind(serverSocket, servinfo->ai_addr, servinfo->ai_addrlen);

    // Allow the socket to be reused immediately after the server is closed
    int yes = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    listen(serverSocket, 10);

    int clientSocket;
    struct sockaddr_storage clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);

    while(true) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
        send(clientSocket, "Hello, World!", 13, 0);
        close(clientSocket);
    }
    
    return 0;
}
