#include "../inc/client.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::string host = "127.0.0.1";
    int port = 8080;

    // Parse command line arguments if provided
    if (argc > 1) {
        host = argv[1];
    }
    if (argc > 2) {
        port = std::stoi(argv[2]);
    }

    JSONClient client(host, port);
    client.RunInteractive();

    return 0;
}