#ifndef CLIENT_H
#define CLIENT_H

#include "Lamp.h"

class Client {
public:
    Client(const std::string& host, int port);
    void start();

private:
    std::string host;
    int port;
    int client_fd;
    Lamp lamp;

    void receiveCommands();
};

#endif