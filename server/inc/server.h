#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <memory>
#include <atomic>
#include "../../inc/task.h"
#include "../../ext/httplib.h"
#include "../../ext/nlohmann/json.hpp"

using json = nlohmann::json;

class JSONServer {
private:
    std::unique_ptr<httplib::Server> server;
    std::unique_ptr<JSONHandler> handler;
    std::atomic<bool> shouldStop;
    int serverPort;

public:
    JSONServer(int port = 8080);
    ~JSONServer();

    void Start();
    void Stop();

private:
    void SetupRoutes();
    void HandleCommand(const httplib::Request& req, httplib::Response& res);
    void HandlePing(const httplib::Request& req, httplib::Response& res);

    json ProcessCommand(const json& request);
    json CreateResponse(const std::string& command, int result) const;
};

#endif // SERVER_H