#include "../inc/json_handler_lib.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <regex>
#include <iomanip>
#include <chrono>

JSONHandlerLib::JSONHandlerLib() : idCounter(1) {
    dataFilePath = "data.json";
}

JSONHandlerLib::~JSONHandlerLib() {}

bool JSONHandlerLib::Input(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    try {
        file >> data;
        file.close();
        return true;
    } catch (const json::parse_error&) {
        file.close();
        return false;
    }
}

std::string JSONHandlerLib::GetPrintOutput() const {
    if (data.empty()) {
        return "No data to display.";
    }
    return data.dump(4);
}

bool JSONHandlerLib::Execute() {
    if (!data.is_array() || data.empty()) {
        return false;
    }

    std::vector<json> items;
    for (const auto& item : data) {
        items.push_back(item);
    }

    // Sort by Rating field
    std::sort(items.begin(), items.end(), [](const json& a, const json& b) {
        if (!a.contains("Rating") || !b.contains("Rating")) return false;
        return a["Rating"].get<double>() < b["Rating"].get<double>();
    });

    // Calculate 10% of items
    size_t tenPercent = std::max(size_t(1), items.size() / 10);

    json result = json::array();

    // Add bottom 10% (lowest ratings)
    for (size_t i = 0; i < tenPercent && i < items.size(); ++i) {
        result.push_back(items[i]);
    }

    // Add top 10% (highest ratings)
    size_t startIdx = items.size() > tenPercent ? items.size() - tenPercent : 0;
    for (size_t i = startIdx; i < items.size(); ++i) {
        if (std::find(result.begin(), result.end(), items[i]) == result.end()) {
            result.push_back(items[i]);
        }
    }

    data = result;
    return true;
}

bool JSONHandlerLib::AddItem(const json& item) {
    if (!data.is_array()) {
        data = json::array();
    }
    data.push_back(item);
    return true;
}

int JSONHandlerLib::Delete(const std::string& field, const std::string& value) {
    if (!data.is_array()) {
        return 0;
    }

    int deleted = 0;
    auto it = std::remove_if(data.begin(), data.end(), [&](const json& item) {
        if (!item.contains(field)) return false;

        bool shouldDelete = false;
        if (item[field].is_string()) {
            shouldDelete = item[field].get<std::string>() == value;
        } else if (item[field].is_number_integer()) {
            try {
                shouldDelete = item[field].get<int>() == std::stoi(value);
            } catch(...) {}
        } else if (item[field].is_number_float()) {
            try {
                shouldDelete = std::abs(item[field].get<double>() - std::stod(value)) < 0.001;
            } catch(...) {}
        }

        if (shouldDelete) deleted++;
        return shouldDelete;
    });

    if (it != data.end()) {
        data.erase(it, data.end());
    }

    return deleted;
}

json JSONHandlerLib::Generate() {
    json newItem;

    // Sample data for random generation
    std::vector<std::string> sampleAuthors = {
        "Leo Tolstoy", "Fyodor Dostoevsky", "Anton Chekhov",
        "Alexander Pushkin", "Nikolai Gogol", "Ivan Turgenev",
        "Vladimir Nabokov", "Mikhail Bulgakov"
    };

    std::vector<std::string> sampleNames = {
        "War and Peace", "Crime and Punishment", "Anna Karenina",
        "The Brothers Karamazov", "Dead Souls", "Fathers and Sons",
        "The Master and Margarita", "Eugene Onegin"
    };

    std::vector<std::string> samplePublishers = {
        "Penguin Classics", "Oxford University Press", "Vintage Books",
        "Modern Library", "Everyman's Library", "Dover Publications"
    };

    std::random_device rd;
    std::mt19937 gen(rd());

    // Generate random authors (1-3)
    std::uniform_int_distribution<> authorCountDist(1, 3);
    std::uniform_int_distribution<> authorDist(0, sampleAuthors.size() - 1);
    int authorCount = authorCountDist(gen);
    std::vector<std::string> authors;
    for (int i = 0; i < authorCount; ++i) {
        authors.push_back(sampleAuthors[authorDist(gen)]);
    }
    newItem["Author"] = authors;

    // Generate random name
    std::uniform_int_distribution<> nameDist(0, sampleNames.size() - 1);
    std::string name = sampleNames[nameDist(gen)];
    newItem["Name"] = name;

    // Generate random year (1800-2024)
    std::uniform_int_distribution<> yearDist(1800, 2024);
    newItem["Year"] = yearDist(gen);

    // Generate random publisher
    std::uniform_int_distribution<> publisherDist(0, samplePublishers.size() - 1);
    newItem["Publisher"] = samplePublishers[publisherDist(gen)];

    // Generate random rating (0.0-10.0)
    std::uniform_real_distribution<> ratingDist(0.0, 10.0);
    newItem["Rating"] = std::round(ratingDist(gen) * 10.0) / 10.0;

    // Generate ID
    newItem["ID"] = GenerateID(name);

    return newItem;
}

bool JSONHandlerLib::GenerateEx(int count) {
    for (int i = 0; i < count; ++i) {
        json item = Generate();
        AddItem(item);
    }
    return true;
}

std::string JSONHandlerLib::GenerateID(const std::string& name) {
    std::stringstream ss;
    ss << "LIT" << std::setfill('0') << std::setw(3) << idCounter++;

    // Add first 3 letters of name (or less if name is shorter)
    std::string namePrefix = name.substr(0, std::min(size_t(3), name.length()));
    std::transform(namePrefix.begin(), namePrefix.end(), namePrefix.begin(), ::toupper);
    ss << namePrefix;

    return ss.str();
}

bool JSONHandlerLib::ValidateID(const std::string& id) const {
    // Pattern: LIT + 3 digits + 1-3 letters
    std::regex pattern("^LIT\\d{3}[A-Z]{1,3}$");
    return std::regex_match(id, pattern);
}

json JSONHandlerLib::GetByID(const std::string& id) const {
    if (!ValidateID(id)) {
        return json();
    }

    for (const auto& item : data) {
        if (item.contains("ID") && item["ID"] == id) {
            return item;
        }
    }

    return json();
}

bool JSONHandlerLib::SaveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    file << std::setw(4) << data << std::endl;
    file.close();
    return true;
}

// C-style wrapper functions
JSONHandlerLib* CreateHandler() {
    return new JSONHandlerLib();
}

void DestroyHandler(JSONHandlerLib* handler) {
    delete handler;
}

bool ProcessCommand(JSONHandlerLib* handler, const char* command, const char* params) {
    if (!handler || !command) return false;

    std::string cmd(command);

    if (cmd == "input" && params) {
        return handler->Input(std::string(params));
    } else if (cmd == "execute") {
        return handler->Execute();
    } else if (cmd == "generate") {
        json item = handler->Generate();
        return handler->AddItem(item);
    }

    return false;
}