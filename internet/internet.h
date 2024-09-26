#ifndef INTERNET_H
#define INTERNET_H

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <array>
#include <stdexcept>
#include <memory>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

void startServer(int port = 8080) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    std::string ip = exec("hostname -I").substr(0, ip.size() - 2);
    if (inet_pton(AF_INET, ip.c_str(), &address.sin_addr) <= 0) {
        std::cout << "Invalid address / Address not supported" << std::endl;
        exit(EXIT_FAILURE); 
    }
    address.sin_port = htons(port);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    char server_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &address.sin_addr, server_ip, sizeof(server_ip));
    std::cout << "Server is running on IP: " << server_ip << " and port: " << port << std::endl;

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    read(new_socket, buffer, 1024);
    std::cout << "Message from client: " << buffer << std::endl;
    close(new_socket);
    close(server_fd);
}

void startClient(const char* ipAddress, int port) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    std::string input;
    std::cout << "message: ";
    std::getline(std::cin, input);
    const char* message = input.c_str();
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Socket creation error" << std::endl;
        return;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ipAddress, &serv_addr.sin_addr) <= 0) {
        std::cout << "Invalid address / Address not supported" << std::endl;
        return;
    }
    
    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Connection failed" << std::endl;
        return;
    }
    send(sock, message, strlen(message), 0);
    std::cout << "Message sent to server" << std::endl;
    close(sock);
}

#endif