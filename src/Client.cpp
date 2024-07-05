#include "Client.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>

Client::Client(const std::string& host, int port) : host(host), port(port), client_fd(-1) {}

void Client::start() {
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr);

    if (connect(client_fd, (struct sockaddr*) & server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Failed to connect to server" << std::endl;
        return;
    }

    std::cout << "Connected to server " << host << ":" << port << std::endl;

    receiveCommands();

    close(client_fd);
}

void Client::receiveCommands() { // Получение команд от сервера
    uint8_t buffer[256];

    while (true) {
        ssize_t length = recv(client_fd, buffer, sizeof(buffer), 0);
        if (length <= 0) {
            std::cerr << "Disconnected from server" << std::endl;
            break;
        }

        uint8_t type = buffer[0]; // Извлечение типа команды и длины поля value
        uint16_t len = (buffer[1] << 8) | buffer[2];

        lamp.processCommand(type, len, buffer + 3);
    }
}