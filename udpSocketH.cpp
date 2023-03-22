#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // Create a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket\n";
        return 1;
    }

    // Bind the socket to a local address and port
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(12345);
    if (bind(sockfd, (sockaddr*) &addr, sizeof(addr)) < 0) {
        std::cerr << "Error binding socket\n";
        close(sockfd);
        return 1;
    }

    // Receive messages and print them to the console
    while (true) {
        char buffer[1024];
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int nbytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (sockaddr*) &client_addr, &client_len);
        if (nbytes < 0) {
            std::cerr << "Error receiving message\n";
            continue;
        }
        std::cout << "Received message from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << "\n";
        std::cout << "Message: " << std::string(buffer, nbytes) << "\n";
    }

    // Close the socket
    close(sockfd);

    return 0;
}
