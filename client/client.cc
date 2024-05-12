#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

const int BUFFER_SIZE = 1024;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Использование: " << argv[0] << " <IP сервера> <порт сервера>\n";
        return EXIT_FAILURE;
    }

    const char* server_ip = argv[1];
    int server_port = std::atoi(argv[2]);

    while (true) {
        int client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == -1) {
            std::cerr << "Ошибка при создании сокета\n";
            return EXIT_FAILURE;
        }

        struct sockaddr_in server_address;
        memset(&server_address, 0, sizeof(server_address));
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = inet_addr(server_ip);
        server_address.sin_port = htons(server_port);

        if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
            std::cerr << "Ошибка при подключении к серверу\n";
            close(client_socket);
            sleep(3);
            continue;
        }

        std::cout << "Подключение к серверу " << server_ip << ":" << server_port << " установлено\n";

        while (true) {
            std::cout << "Введите команду для отправки на сервер ('exit' для завершения): ";
            std::string command;
            std::getline(std::cin, command);

            if (command == "exit") {
                close(client_socket);
                return EXIT_SUCCESS;
            }

            send(client_socket, command.c_str(), command.size(), 0);

            char buffer[BUFFER_SIZE];
            int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
            if (bytes_received <= 0) {
                std::cerr << "Ошибка при чтении ответа от сервера\n";
                close(client_socket);
                return EXIT_FAILURE;
            }

            buffer[bytes_received] = '\0';
            std::cout << buffer << std::endl;
            
        }

        close(client_socket);
    }

    return EXIT_SUCCESS;
}
