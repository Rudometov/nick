#include "../inc/server.h"
#include <iostream>
#include <csignal>

std::unique_ptr<JSONServer> globalServer;

void SignalHandler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        std::cout << "\nReceived termination signal. Shutting down server..." << std::endl;
        if (globalServer) {
            globalServer->Stop();
        }
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    int port = 8080;

    // Parse command line arguments if provided
    if (argc > 1) {
        port = std::stoi(argv[1]);
    }

    // Set up signal handlers
    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);

    // Create and start server
    globalServer = std::make_unique<JSONServer>(port);
    globalServer->Start();

    return 0;
}