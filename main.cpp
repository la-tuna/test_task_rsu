#include "Server.h"
#include "Client.h"

// Справка: порядок ввода аргументов из командной строки
void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [server|client] [host] [port]" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) { // Проверка количества аргументов
        printUsage(argv[0]);
        return 1;
    }

    // Чтение из аргументов командной строки
    std::string mode = argv[1];
    std::string host = (argc > 2) ? argv[2] : "127.0.0.1";
    int port = (argc > 3) ? std::stoi(argv[3]) : 9999;

    if (mode == "server") {
        Server server(host, port);
        server.start();
    }
    else if (mode == "client") {
        Client client(host, port);
        client.start();
    }
    else {
        printUsage(argv[0]);
        return 1;
    }

    return 0;
}