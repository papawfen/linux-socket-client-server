#include "server.h"
#include <algorithm>

TCPServer::TCPServer() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Ошибка при создании сокета\n";
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Ошибка при привязке сокета\n";
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 10) == -1) {
        std::cerr << "Ошибка при прослушивании сокета\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Сервер запущен. Ожидание подключений...\n";
}

void TCPServer::handleClient(int client_socket) {
    client_sockets.push_back(client_socket);

    while (true) {
        char buffer[BUFFER_SIZE];
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            std::cerr << "Ошибка при чтении данных\n";
            close(client_socket);
            client_sockets.erase(std::remove(client_sockets.begin(), client_sockets.end(), client_socket), client_sockets.end());
            return;
        }

        std::cout << "Получено от клиента: " << buffer << std::endl;
        if (strcmp(buffer, "connections\n") == 0) {
            int num_connections = getCurrentConnections();
            std::string response = "Текущее количество подключений: " + std::to_string(num_connections) + "\n";
            send(client_socket, response.c_str(), response.size(), 0);
        } else {
            countLettersAndSend(client_socket, buffer);
            broadcastMessage(buffer, client_socket);
        }
    }
}

void TCPServer::countLettersAndSend(int client_socket, const char* buffer) {
    std::map<char, int> count;
    int len = strlen(buffer);
    for (int i = 0; i < len; ++i) {
        char ch = buffer[i];
        if (isalpha(ch)) {
            count[ch]++;
        }
    }
    std::string resp;
    for (auto i : count) {
        resp += i.first + " " + std::to_string(i.second) + '\n';
    }
    send(client_socket, resp.c_str(), resp.length(), 0);
}

void TCPServer::broadcastMessage(const std::string& message, int sender_socket) {
    for (int client_socket : client_sockets) {
        if (client_socket != sender_socket) {
            send(client_socket, message.c_str(), message.size(), 0);
        }
    }
}

int TCPServer::getCurrentConnections() {
    return client_sockets.size();
}

void TCPServer::start() {
    while (true) {
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
        if (client_socket == -1) {
            std::cerr << "Ошибка при принятии подключения\n";
            continue;
        }

        std::cout << "Подключение принято\n";
        handleClient(client_socket);
    }
}

TCPServer::~TCPServer() {
    close(server_socket);
}