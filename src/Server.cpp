#include "Server.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>

Server::Server(const std::string& host, int port) : host(host), port(port), server_fd(-1) {}

void Server::start() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr);

    if (bind(server_fd, (struct sockaddr*) & server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Failed to bind socket" << std::endl;
        return;
    }

    if (listen(server_fd, 5) == -1) {
        std::cerr << "Failed to listen on socket" << std::endl;
        return;
    }

    std::cout << "Server started on " << host << ":" << port << std::endl;

    // Прием клиентских подключений
    std::thread(&Server::acceptClients, this).detach();

    // Ожидание команд от пользователя для отправки клиентам
    while (true) {
        std::string command;
        std::cout << "Enter command (on, off, color R G B): ";
        std::getline(std::cin, command);

        // Отправка команд клиентам
        if (command == "on") {
            uint8_t cmd[] = { 0x12, 0x00, 0x00 };
            sendCommandToClients(cmd, sizeof(cmd));
        }
        else if (command == "off") {
            uint8_t cmd[] = { 0x13, 0x00, 0x00 };
            sendCommandToClients(cmd, sizeof(cmd));
        }
        else if (command.rfind("color", 0) == 0) {
            int r, g, b;
            sscanf(command.c_str(), "color %d %d %d", &r, &g, &b);
            uint8_t cmd[] = { 0x20, 0x00, 0x03, static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b) };
            sendCommandToClients(cmd, sizeof(cmd));
        }
        else {
            std::cout << "Unknown command" << std::endl;
        }
    }
}

void Server::stop() {
    if (server_fd != -1) {
        close(server_fd);
    }
}

// Обработка подключения клиента
void Server::handleClient(int client_fd) {
    client_fds.push_back(client_fd);
    lamps.emplace_back();

    uint8_t buffer[256];

    while (true) { // Прием данных от клиента
        ssize_t length = recv(client_fd, buffer, sizeof(buffer), 0);
        if (length <= 0) {
            close(client_fd); // Закрытие при ошибке или прекращении передачи данных
            break;
        }
        processClientData(client_fd, buffer, length);
    }
}

// Обработка принятых данных от клиента
void Server::processClientData(int client_fd, uint8_t* buffer, ssize_t length) { 
    while (length > 0) {
        uint8_t type = buffer[0];
        uint16_t len = (buffer[1] << 8) | buffer[2];

        if (length >= len + 3) { // Проверка на корректность всей команды (type + length + value)
            sendCommandToLamps(type, len, buffer + 3);

            buffer += len + 3; // Смещение буфера на следующую команду
            length -= len + 3;
        }
        else {
            break;
        }
    }
}

void Server::sendCommandToLamps(uint8_t type, uint16_t length, const uint8_t* value) {
    for (Lamp& lamp : lamps) {
        lamp.processCommand(type, length, value);
    }
}

// Отправка команды всем подключенным клиентам
void Server::sendCommandToClients(const uint8_t* command, size_t length) {
    for (int client_fd : client_fds) {
        send(client_fd, command, length, 0);
    }
}

// Прием клиентских подключений
void Server::acceptClients() {
    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd == -1) {
            std::cerr << "Failed to accept client" << std::endl;
            continue;
        }
        std::thread(&Server::handleClient, this, client_fd).detach();
    }
}