#include <iostream>
#include <string>
#include <memory>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

constexpr int PORT = 8080;
const int BUFFER_SIZE = 1024;

int main() 
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
{
    perror("Socket error.");
    exit(EXIT_FAILURE);
}
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        perror("Bind error.");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 3) < 0)
    {
        perror("Listen.");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port." << PORT << std::endl;
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0)
    {
        perror("Accept.");
        exit(EXIT_FAILURE);
    }

    ssize_t valread = read(new_socket, buffer, BUFFER_SIZE);
    std::cout << "Received: " << buffer << std::endl;
    send(new_socket, buffer, valread, 0);
    std::cout << "Echoed back to client." << std::endl;
    close(new_socket);
    close(server_fd);
    return 0;
}

