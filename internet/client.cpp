#include "internet.h"

int main() {
    std::string input;
    std::cout << "ip addr: ";
    std::getline(std::cin, input);
    const char* ip = input.c_str();
    startClient(ip, 8080);
}