#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <map>
#include <vector>
#include <string>

constexpr int BUFFER_SIZE = 1024;
constexpr int port = 8888;

class TCPServer {

public:
    TCPServer();
    ~TCPServer();
    void start();

private:
    int server_socket;
    std::vector<int> client_sockets;

    void handleClient(int client_socket);
    void countLettersAndSend(int client_socket, const char* buffer);
    void broadcastMessage(const std::string& message, int sender_socket);
    int getCurrentConnections();
};

#endif // TCPSERVER_H
