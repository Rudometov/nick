#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>
#include <memory>
#include "../ext/nlohmann/json.hpp"

using json = nlohmann::json;

// Task 1: Literature structure
struct Literature {
    std::vector<std::string> Author;
    std::string Name;
    int Year;
    std::string Publisher;
    double Rating;
};

class JSONHandler {
private:
    json data;
    std::string dataFilePath;
    int idCounter;

public:
    JSONHandler();
    ~JSONHandler();

    // Part A: Input/Output functions
    void Input(const std::string& filename);
    void Print() const;

    // Part B: Data processing functions
    void Execute();
    void Add();
    void Delete(const std::string& field, const std::string& value);
    void Generate();

    // Part E: Extended functions
    std::string GenerateID(const std::string& name);
    bool ValidateID(const std::string& id) const;
    json GetByID(const std::string& id) const;

    // Part F: Batch generation
    void Generate_ex(int count);

    // Helper functions
    void SaveToFile(const std::string& filename) const;
    json GetData() const { return data; }
    void SetData(const json& newData) { data = newData; }
    void Clear() { data.clear(); }

    // Conversion functions
    void to_json(json& j, const Literature& lit) const;
    void from_json(const json& j, Literature& lit) const;
};

#endif // TASK_H