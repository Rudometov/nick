#include "../inc/client.h"
#include <iostream>
#include <sstream>

JSONClient::JSONClient(const std::string& host, int port)
    : serverHost(host), serverPort(port) {
    client = std::make_unique<httplib::Client>(serverHost, serverPort);
    client->set_connection_timeout(5, 0);
    client->set_read_timeout(5, 0);
}

JSONClient::~JSONClient() {}

bool JSONClient::Connect() {
    try {
        auto res = client->Get("/ping");
        return res && res->status == 200;
    } catch (...) {
        return false;
    }
}

json JSONClient::SerializeSendMessage(const SendMessage& msg) const {
    json j;
    j["Command"] = msg.Command;
    j["Param"] = msg.Param;
    return j;
}

ReceiveMessage JSONClient::DeserializeReceiveMessage(const json& j) const {
    ReceiveMessage msg;
    if (j.contains("Command")) {
        msg.Command = j["Command"].get<std::string>();
    }
    if (j.contains("Result")) {
        msg.Result = j["Result"].get<int>();
    }
    return msg;
}

bool JSONClient::SendCommand(const SendMessage& msg, ReceiveMessage& response) {
    try {
        json requestJson = SerializeSendMessage(msg);
        std::string body = requestJson.dump();

        auto res = client->Post("/command", body, "application/json");

        if (res && res->status == 200) {
            json responseJson = json::parse(res->body);
            response = DeserializeReceiveMessage(responseJson);
            return true;
        }

        return false;
    } catch (const std::exception& e) {
        std::cerr << "Error sending command: " << e.what() << std::endl;
        return false;
    }
}

void JSONClient::StopServer() {
    SendMessage msg;
    msg.Command = "stop";
    ReceiveMessage response;

    if (SendCommand(msg, response)) {
        std::cout << "Server stop command sent." << std::endl;
    } else {
        std::cerr << "Failed to send stop command to server." << std::endl;
    }
}

void JSONClient::Input() {
    std::string filename;
    std::cout << "Enter filename to load: ";
    std::cin >> filename;

    SendMessage msg;
    msg.Command = "input";
    msg.Param.push_back(filename);
    ReceiveMessage response;

    if (SendCommand(msg, response)) {
        if (response.Result == 1) {
            std::cout << "Data loaded successfully." << std::endl;
        } else {
            std::cout << "Failed to load data." << std::endl;
        }
    } else {
        std::cerr << "Failed to communicate with server." << std::endl;
    }
}

void JSONClient::Print() {
    SendMessage msg;
    msg.Command = "print";
    ReceiveMessage response;

    if (SendCommand(msg, response)) {
        std::cout << "Print command executed." << std::endl;
    } else {
        std::cerr << "Failed to communicate with server." << std::endl;
    }
}

void JSONClient::Execute() {
    SendMessage msg;
    msg.Command = "execute";
    ReceiveMessage response;

    if (SendCommand(msg, response)) {
        if (response.Result == 1) {
            std::cout << "Task executed successfully." << std::endl;
        } else {
            std::cout << "Task execution failed." << std::endl;
        }
    } else {
        std::cerr << "Failed to communicate with server." << std::endl;
    }
}

void JSONClient::Add() {
    SendMessage msg;
    msg.Command = "add";

    std::cin.ignore();

    // Get Authors
    std::cout << "Enter authors (empty line to finish):" << std::endl;
    std::string author;
    while (std::getline(std::cin, author) && !author.empty()) {
        msg.Param.push_back("author:" + author);
    }

    // Get Name
    std::cout << "Enter book name: ";
    std::string name;
    std::getline(std::cin, name);
    msg.Param.push_back("name:" + name);

    // Get Year
    std::cout << "Enter publication year: ";
    std::string year;
    std::getline(std::cin, year);
    msg.Param.push_back("year:" + year);

    // Get Publisher
    std::cout << "Enter publisher: ";
    std::string publisher;
    std::getline(std::cin, publisher);
    msg.Param.push_back("publisher:" + publisher);

    // Get Rating
    std::cout << "Enter rating (0.0-10.0): ";
    std::string rating;
    std::getline(std::cin, rating);
    msg.Param.push_back("rating:" + rating);

    ReceiveMessage response;
    if (SendCommand(msg, response)) {
        if (response.Result == 1) {
            std::cout << "Item added successfully." << std::endl;
        } else {
            std::cout << "Failed to add item." << std::endl;
        }
    } else {
        std::cerr << "Failed to communicate with server." << std::endl;
    }
}

void JSONClient::Delete() {
    std::string field, value;
    std::cout << "Enter field name: ";
    std::cin >> field;
    std::cout << "Enter value to match: ";
    std::cin >> value;

    SendMessage msg;
    msg.Command = "delete";
    msg.Param.push_back(field);
    msg.Param.push_back(value);
    ReceiveMessage response;

    if (SendCommand(msg, response)) {
        if (response.Result == 1) {
            std::cout << "Item(s) deleted successfully." << std::endl;
        } else {
            std::cout << "No items found to delete." << std::endl;
        }
    } else {
        std::cerr << "Failed to communicate with server." << std::endl;
    }
}

void JSONClient::Generate() {
    SendMessage msg;
    msg.Command = "generate";
    ReceiveMessage response;

    if (SendCommand(msg, response)) {
        if (response.Result == 1) {
            std::cout << "Random item generated successfully." << std::endl;
        } else {
            std::cout << "Failed to generate item." << std::endl;
        }
    } else {
        std::cerr << "Failed to communicate with server." << std::endl;
    }
}

void JSONClient::GenerateEx() {
    int count;
    std::cout << "Enter number of items to generate: ";
    std::cin >> count;

    SendMessage msg;
    msg.Command = "generate_ex";
    msg.Param.push_back(std::to_string(count));
    ReceiveMessage response;

    if (SendCommand(msg, response)) {
        if (response.Result == 1) {
            std::cout << "Generated " << count << " items successfully." << std::endl;
        } else {
            std::cout << "Failed to generate items." << std::endl;
        }
    } else {
        std::cerr << "Failed to communicate with server." << std::endl;
    }
}

void JSONClient::RunInteractive() {
    std::cout << "JSON Client - Connecting to server at " << serverHost << ":" << serverPort << std::endl;

    if (!Connect()) {
        std::cerr << "Failed to connect to server. Make sure server is running." << std::endl;
        return;
    }

    std::cout << "Connected to server successfully!" << std::endl;

    int choice;
    while (true) {
        std::cout << "\n====== JSON Client Menu ======" << std::endl;
        std::cout << "1. Load data from file" << std::endl;
        std::cout << "2. Display data" << std::endl;
        std::cout << "3. Execute task" << std::endl;
        std::cout << "4. Add item" << std::endl;
        std::cout << "5. Delete item" << std::endl;
        std::cout << "6. Generate random item" << std::endl;
        std::cout << "7. Generate multiple items" << std::endl;
        std::cout << "8. Stop server" << std::endl;
        std::cout << "0. Exit client" << std::endl;
        std::cout << "==============================" << std::endl;
        std::cout << "Enter your choice: ";

        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input." << std::endl;
            continue;
        }

        switch (choice) {
            case 1:
                Input();
                break;
            case 2:
                Print();
                break;
            case 3:
                Execute();
                break;
            case 4:
                Add();
                break;
            case 5:
                Delete();
                break;
            case 6:
                Generate();
                break;
            case 7:
                GenerateEx();
                break;
            case 8:
                StopServer();
                std::cout << "Exiting client..." << std::endl;
                return;
            case 0:
                std::cout << "Exiting client..." << std::endl;
                return;
            default:
                std::cout << "Invalid choice." << std::endl;
                break;
        }
    }
}