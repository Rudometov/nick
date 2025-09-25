#include "../inc/task.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <regex>
#include <iomanip>
#include <chrono>

JSONHandler::JSONHandler() : idCounter(1) {
    dataFilePath = "data.json";
}

JSONHandler::~JSONHandler() {}

void JSONHandler::Input(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return;
    }

    try {
        file >> data;
        std::cout << "Successfully loaded data from " << filename << std::endl;
    } catch (const json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    }

    file.close();
}

void JSONHandler::Print() const {
    if (data.empty()) {
        std::cout << "No data to display." << std::endl;
        return;
    }

    std::cout << std::setw(4) << data << std::endl;
}

void JSONHandler::Execute() {
    // Task 1: Sort by rating and keep top 10% and bottom 10%
    if (!data.is_array() || data.empty()) {
        std::cerr << "Error: Invalid or empty data" << std::endl;
        return;
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
    std::cout << "Executed: Kept 10% books with highest and lowest ratings" << std::endl;
}

void JSONHandler::Add() {
    json newItem;

    std::cout << "Adding new Literature item:" << std::endl;

    // Get Authors
    std::vector<std::string> authors;
    std::string author;
    std::cout << "Enter authors (empty line to finish):" << std::endl;
    std::cin.ignore();
    while (std::getline(std::cin, author) && !author.empty()) {
        authors.push_back(author);
    }
    newItem["Author"] = authors;

    // Get Name
    std::cout << "Enter book name: ";
    std::string name;
    std::getline(std::cin, name);
    newItem["Name"] = name;

    // Get Year
    std::cout << "Enter publication year: ";
    int year;
    std::cin >> year;
    newItem["Year"] = year;

    // Get Publisher
    std::cout << "Enter publisher: ";
    std::cin.ignore();
    std::string publisher;
    std::getline(std::cin, publisher);
    newItem["Publisher"] = publisher;

    // Get Rating
    std::cout << "Enter rating (0.0-10.0): ";
    double rating;
    std::cin >> rating;
    newItem["Rating"] = rating;

    // Generate and add ID
    newItem["ID"] = GenerateID(name);

    if (!data.is_array()) {
        data = json::array();
    }

    data.push_back(newItem);
    std::cout << "Item added successfully with ID: " << newItem["ID"] << std::endl;
}

void JSONHandler::Delete(const std::string& field, const std::string& value) {
    if (!data.is_array()) {
        std::cerr << "Error: Data is not an array" << std::endl;
        return;
    }

    auto it = std::remove_if(data.begin(), data.end(), [&](const json& item) {
        if (!item.contains(field)) return false;

        if (item[field].is_string()) {
            return item[field].get<std::string>() == value;
        } else if (item[field].is_number_integer()) {
            try {
                return item[field].get<int>() == std::stoi(value);
            } catch(...) {
                return false;
            }
        } else if (item[field].is_number_float()) {
            try {
                return std::abs(item[field].get<double>() - std::stod(value)) < 0.001;
            } catch(...) {
                return false;
            }
        }
        return false;
    });

    if (it != data.end()) {
        data.erase(it, data.end());
        std::cout << "Deleted items where " << field << " = " << value << std::endl;
    } else {
        std::cout << "No items found with " << field << " = " << value << std::endl;
    }
}

void JSONHandler::Generate() {
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

    if (!data.is_array()) {
        data = json::array();
    }

    data.push_back(newItem);
    std::cout << "Generated item with ID: " << newItem["ID"] << std::endl;
}

std::string JSONHandler::GenerateID(const std::string& name) {
    std::stringstream ss;
    ss << "LIT" << std::setfill('0') << std::setw(3) << idCounter++;

    // Add first 3 letters of name (or less if name is shorter)
    std::string namePrefix = name.substr(0, std::min(size_t(3), name.length()));
    std::transform(namePrefix.begin(), namePrefix.end(), namePrefix.begin(), ::toupper);
    ss << namePrefix;

    return ss.str();
}

bool JSONHandler::ValidateID(const std::string& id) const {
    // Pattern: LIT + 3 digits + 3 letters
    std::regex pattern("^LIT\\d{3}[A-Z]{1,3}$");
    return std::regex_match(id, pattern);
}

json JSONHandler::GetByID(const std::string& id) const {
    if (!ValidateID(id)) {
        std::cerr << "Invalid ID format: " << id << std::endl;
        return json();
    }

    for (const auto& item : data) {
        if (item.contains("ID") && item["ID"] == id) {
            return item;
        }
    }

    return json();
}

void JSONHandler::Generate_ex(int count) {
    std::cout << "Generating " << count << " items..." << std::endl;

    for (int i = 0; i < count; ++i) {
        Generate();
    }

    std::cout << "Successfully generated " << count << " items" << std::endl;
}

void JSONHandler::SaveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << " for writing" << std::endl;
        return;
    }

    file << std::setw(4) << data << std::endl;
    file.close();
    std::cout << "Data saved to " << filename << std::endl;
}

void JSONHandler::to_json(json& j, const Literature& lit) const {
    j = json{
        {"Author", lit.Author},
        {"Name", lit.Name},
        {"Year", lit.Year},
        {"Publisher", lit.Publisher},
        {"Rating", lit.Rating}
    };
}

void JSONHandler::from_json(const json& j, Literature& lit) const {
    if (j.contains("Author")) j.at("Author").get_to(lit.Author);
    if (j.contains("Name")) j.at("Name").get_to(lit.Name);
    if (j.contains("Year")) j.at("Year").get_to(lit.Year);
    if (j.contains("Publisher")) j.at("Publisher").get_to(lit.Publisher);
    if (j.contains("Rating")) j.at("Rating").get_to(lit.Rating);
}