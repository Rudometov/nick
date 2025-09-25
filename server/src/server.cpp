#include "../inc/server.h"
#include <iostream>
#include <thread>
#include <sstream>

JSONServer::JSONServer(int port) : serverPort(port), shouldStop(false) {
    server = std::make_unique<httplib::Server>();
    handler = std::make_unique<JSONHandler>();
}

JSONServer::~JSONServer() {
    Stop();
}

void JSONServer::SetupRoutes() {
    // Ping endpoint for connection check
    server->Get("/ping", [this](const httplib::Request& req, httplib::Response& res) {
        HandlePing(req, res);
    });

    // Main command endpoint
    server->Post("/command", [this](const httplib::Request& req, httplib::Response& res) {
        HandleCommand(req, res);
    });
}

void JSONServer::HandlePing(const httplib::Request&, httplib::Response& res) {
    res.set_content("pong", "text/plain");
    res.status = 200;
}

void JSONServer::HandleCommand(const httplib::Request& req, httplib::Response& res) {
    try {
        json request = json::parse(req.body);
        json response = ProcessCommand(request);

        res.set_content(response.dump(), "application/json");
        res.status = 200;
    } catch (const json::parse_error& e) {
        json error = CreateResponse("error", 0);
        res.set_content(error.dump(), "application/json");
        res.status = 400;
    } catch (const std::exception& e) {
        json error = CreateResponse("error", 0);
        res.set_content(error.dump(), "application/json");
        res.status = 500;
    }
}

json JSONServer::CreateResponse(const std::string& command, int result) const {
    json response;
    response["Command"] = command;
    response["Result"] = result;
    return response;
}

json JSONServer::ProcessCommand(const json& request) {
    if (!request.contains("Command")) {
        return CreateResponse("error", 0);
    }

    std::string command = request["Command"].get<std::string>();
    std::vector<std::string> params;

    if (request.contains("Param") && request["Param"].is_array()) {
        for (const auto& p : request["Param"]) {
            if (p.is_string()) {
                params.push_back(p.get<std::string>());
            }
        }
    }

    int result = 0;

    try {
        if (command == "input") {
            if (!params.empty()) {
                handler->Input(params[0]);
                result = 1;
            }
        } else if (command == "print") {
            handler->Print();
            result = 1;
        } else if (command == "execute") {
            handler->Execute();
            result = 1;
        } else if (command == "add") {
            // Parse parameters for add
            json newItem;
            std::vector<std::string> authors;

            for (const auto& param : params) {
                size_t colonPos = param.find(':');
                if (colonPos != std::string::npos) {
                    std::string key = param.substr(0, colonPos);
                    std::string value = param.substr(colonPos + 1);

                    if (key == "author") {
                        authors.push_back(value);
                    } else if (key == "name") {
                        newItem["Name"] = value;
                    } else if (key == "year") {
                        newItem["Year"] = std::stoi(value);
                    } else if (key == "publisher") {
                        newItem["Publisher"] = value;
                    } else if (key == "rating") {
                        newItem["Rating"] = std::stod(value);
                    }
                }
            }

            if (!authors.empty()) {
                newItem["Author"] = authors;
            }

            // Add ID
            if (newItem.contains("Name")) {
                newItem["ID"] = handler->GenerateID(newItem["Name"].get<std::string>());
            }

            // Add to data
            json currentData = handler->GetData();
            if (!currentData.is_array()) {
                currentData = json::array();
            }
            currentData.push_back(newItem);
            handler->SetData(currentData);

            result = 1;
        } else if (command == "delete") {
            if (params.size() >= 2) {
                handler->Delete(params[0], params[1]);
                result = 1;
            }
        } else if (command == "generate") {
            handler->Generate();
            result = 1;
        } else if (command == "generate_ex") {
            if (!params.empty()) {
                int count = std::stoi(params[0]);
                handler->Generate_ex(count);
                result = 1;
            }
        } else if (command == "stop") {
            shouldStop = true;
            result = 1;
        }

        std::cout << "Processed command: " << command << " (Result: " << result << ")" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error processing command: " << e.what() << std::endl;
        result = 0;
    }

    return CreateResponse(command, result);
}

void JSONServer::Start() {
    SetupRoutes();

    std::cout << "JSON Server starting on port " << serverPort << "..." << std::endl;

    // Start server in a separate thread
    std::thread serverThread([this]() {
        server->listen("0.0.0.0", serverPort);
    });

    std::cout << "Server is running. Press Ctrl+C to stop..." << std::endl;

    // Monitor for stop signal
    while (!shouldStop.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    Stop();

    if (serverThread.joinable()) {
        serverThread.join();
    }
}

void JSONServer::Stop() {
    if (server) {
        std::cout << "Stopping server..." << std::endl;
        server->stop();
    }
}