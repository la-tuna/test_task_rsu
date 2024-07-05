#ifndef SERVER_H
#define SERVER_H

#include "Lamp.h"

#include <vector>
#include <thread>
#include <sys/types.h>

class Server {
public:
    Server(const std::string& host, int port);
    void start();
    void stop();

private:
    std::string host;
    int port;
    int server_fd;
    std::vector<int> client_fds;
    std::vector<Lamp> lamps;

    void handleClient(int client_fd);
    void processClientData(int client_fd, uint8_t* buffer, ssize_t length);
    void sendCommandToLamps(uint8_t type, uint16_t length, const uint8_t* value);
    void sendCommandToClients(const uint8_t* command, size_t length);
    void acceptClients();
};

#endif