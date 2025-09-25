#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <memory>
#include "../../ext/httplib.h"
#include "../../ext/nlohmann/json.hpp"

using json = nlohmann::json;

struct SendMessage {
    std::string Command;
    std::vector<std::string> Param;
};

struct ReceiveMessage {
    std::string Command;
    int Result;
};

class JSONClient {
private:
    std::unique_ptr<httplib::Client> client;
    std::string serverHost;
    int serverPort;

public:
    JSONClient(const std::string& host = "127.0.0.1", int port = 8080);
    ~JSONClient();

    bool Connect();
    bool SendCommand(const SendMessage& msg, ReceiveMessage& response);
    void StopServer();

    // Interface functions
    void Input();
    void Print();
    void Execute();
    void Add();
    void Delete();
    void Generate();
    void GenerateEx();

    void RunInteractive();

private:
    json SerializeSendMessage(const SendMessage& msg) const;
    ReceiveMessage DeserializeReceiveMessage(const json& j) const;
};

#endif // CLIENT_H