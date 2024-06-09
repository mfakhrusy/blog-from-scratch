#include <sys/socket.h>
#include <netdb.h>
#include <cstdlib>
#include <unistd.h> // Include the header file for the close function
#include <iostream>
#include <string>
#include <cstring>

int main() {
    int serverSocket;
    struct addrinfo hints, *servinfo;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Get the address information of the server
    int rv = getaddrinfo(NULL, "8080", &hints, &servinfo);

    if (rv != 0) {
        std::cerr << "Error getting address info: " << gai_strerror(rv) << std::endl;
        return 1;
    }

    serverSocket = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

    if (serverSocket == -1) {
        std::cerr << "Error creating socket" << strerror(errno) << std::endl;
        return 1;
    }

    bind(serverSocket, servinfo->ai_addr, servinfo->ai_addrlen);

    // Allow the socket to be reused immediately after the server is closed
    int yes = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    listen(serverSocket, 10);

    int clientSocket;
    struct sockaddr_storage clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);

    while(true) {
        std::cout << "Waiting for connection..." << std::endl;
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection" << strerror(errno) << std::endl;
            break;
        }
        std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title>My Title</title></head><body>Hello, World!</body></html>";
        std::cout << "Sending response..." << std::endl;
        send(clientSocket, httpResponse.c_str(), httpResponse.length(), 0);
        std::cout << "Response sent!" << std::endl;
        close(clientSocket);
    }
    
    return 0;
}
