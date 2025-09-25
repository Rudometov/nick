/**
 * @file task.h
 * @brief JSON Handler for Literature data processing
 * @author Claude Assistant
 * @date 2024
 *
 * @details This file contains the main JSONHandler class for processing
 * Literature data in JSON format according to task requirements S02.E04.Build2025.03.01.0
 */

#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>
#include <memory>
#include "../ext/nlohmann/json.hpp"

using json = nlohmann::json;

/**
 * @struct Literature
 * @brief Data structure for literature items
 *
 * @details Task 1 data structure containing information about books/literature
 * with author, title, year, publisher, and rating fields
 */
struct Literature {
    std::vector<std::string> Author;  ///< List of authors
    std::string Name;                  ///< Title of the book
    int Year;                          ///< Publication year
    std::string Publisher;             ///< Publisher name
    double Rating;                     ///< Rating value (0.0-10.0)
};

/**
 * @class JSONHandler
 * @brief Main class for JSON data manipulation
 *
 * @details Implements all required functions for Parts A-F of the task:
 * - Input/Output operations (Part A)
 * - Data processing functions (Part B)
 * - ID generation and validation (Part E)
 * - Batch operations (Part F)
 */
class JSONHandler {
private:
    json data;
    std::string dataFilePath;
    int idCounter;

public:
    /**
     * @brief Default constructor
     */
    JSONHandler();

    /**
     * @brief Destructor
     */
    ~JSONHandler();

    // Part A: Input/Output functions
    /**
     * @brief Load JSON data from file (Part A)
     * @param filename Path to the JSON file to load
     * @throws None (prints error message on failure)
     */
    void Input(const std::string& filename);

    /**
     * @brief Print JSON data to console (Part A)
     * @details Outputs formatted JSON to stdout
     */
    void Print() const;

    // Part B: Data processing functions
    /**
     * @brief Execute task-specific function (Part B)
     * @details Filters data keeping 10% top and 10% bottom rated items
     */
    void Execute();

    /**
     * @brief Interactively add new Literature item (Part B)
     * @details Prompts user for input via console
     */
    void Add();

    /**
     * @brief Delete items by field value (Part B)
     * @param field Field name to match
     * @param value Value to match for deletion
     */
    void Delete(const std::string& field, const std::string& value);

    /**
     * @brief Generate random Literature item (Part B)
     * @details Creates item with random data from predefined lists
     */
    void Generate();

    // Part E: Extended functions
    /**
     * @brief Generate unique ID for item (Part E)
     * @param name Name to use for ID suffix
     * @return Generated ID in format LIT###XXX
     */
    std::string GenerateID(const std::string& name);

    /**
     * @brief Validate ID format (Part E)
     * @param id ID string to validate
     * @return true if ID matches pattern LIT[0-9]{3}[A-Z]{1,3}
     */
    bool ValidateID(const std::string& id) const;

    /**
     * @brief Get item by ID (Part E)
     * @param id ID to search for
     * @return JSON object if found, empty JSON otherwise
     */
    json GetByID(const std::string& id) const;

    // Part F: Batch generation
    /**
     * @brief Generate multiple random items (Part F)
     * @param count Number of items to generate
     */
    void Generate_ex(int count);

    // Helper functions
    /**
     * @brief Save current data to file
     * @param filename Output file path
     */
    void SaveToFile(const std::string& filename) const;

    /**
     * @brief Get current JSON data
     * @return Copy of internal JSON data
     */
    json GetData() const { return data; }

    /**
     * @brief Set JSON data
     * @param newData New JSON data to replace current
     */
    void SetData(const json& newData) { data = newData; }

    /**
     * @brief Clear all data
     */
    void Clear() { data.clear(); }

    // Conversion functions
    /**
     * @brief Convert Literature to JSON
     * @param j Output JSON object
     * @param lit Input Literature structure
     */
    void to_json(json& j, const Literature& lit) const;

    /**
     * @brief Convert JSON to Literature
     * @param j Input JSON object
     * @param lit Output Literature structure
     */
    void from_json(const json& j, Literature& lit) const;
};

#endif // TASK_H