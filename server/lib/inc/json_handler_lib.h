#ifndef JSON_HANDLER_LIB_H
#define JSON_HANDLER_LIB_H

// Cross-platform export/import macros as required by Part З
#ifdef _WIN32
    #ifdef LIBRARY_EXPORT
        #define LIBRARY_API __declspec(dllexport)
    #else
        #define LIBRARY_API __declspec(dllimport)
    #endif
#else
    #ifdef LIBRARY_EXPORT
        #define LIBRARY_API __attribute__((visibility("default")))
    #else
        #define LIBRARY_API
    #endif
#endif

#include <string>
#include "../../../ext/nlohmann/json.hpp"

using json = nlohmann::json;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief JSON Handler Library class for server-side processing
 * @details Provides all JSON manipulation functions as a dynamic library
 *
 * This class implements Part З of the task requirements -
 * a dynamically linked library with export macros for cross-platform support
 */
class LIBRARY_API JSONHandlerLib {
private:
    json data;
    int idCounter;
    std::string dataFilePath;

public:
    /**
     * @brief Constructor - initializes the handler
     */
    JSONHandlerLib();

    /**
     * @brief Destructor
     */
    ~JSONHandlerLib();

    /**
     * @brief Load JSON data from file
     * @param filename Path to the JSON file to load
     * @return true if successful, false otherwise
     */
    bool Input(const std::string& filename);

    /**
     * @brief Get current JSON data as string
     * @return JSON data as formatted string
     */
    std::string GetPrintOutput() const;

    /**
     * @brief Execute task function (filter by rating)
     * @details Keeps 10% top and 10% bottom rated items
     * @return true if successful, false otherwise
     */
    bool Execute();

    /**
     * @brief Add new item to data
     * @param item JSON object to add
     * @return true if successful, false otherwise
     */
    bool AddItem(const json& item);

    /**
     * @brief Delete items by field value
     * @param field Field name to match
     * @param value Value to match
     * @return Number of items deleted
     */
    int Delete(const std::string& field, const std::string& value);

    /**
     * @brief Generate random item
     * @return Generated item as JSON
     */
    json Generate();

    /**
     * @brief Generate multiple items
     * @param count Number of items to generate
     * @return true if successful, false otherwise
     */
    bool GenerateEx(int count);

    /**
     * @brief Generate unique ID
     * @param name Name to use for ID generation
     * @return Generated ID string
     */
    std::string GenerateID(const std::string& name);

    /**
     * @brief Validate ID format
     * @param id ID to validate
     * @return true if valid, false otherwise
     */
    bool ValidateID(const std::string& id) const;

    /**
     * @brief Get item by ID
     * @param id ID to search for
     * @return JSON object if found, empty otherwise
     */
    json GetByID(const std::string& id) const;

    /**
     * @brief Save data to file
     * @param filename Path to save file
     * @return true if successful, false otherwise
     */
    bool SaveToFile(const std::string& filename) const;

    /**
     * @brief Get current data
     * @return Current JSON data
     */
    json GetData() const { return data; }

    /**
     * @brief Set data
     * @param newData New JSON data to set
     */
    void SetData(const json& newData) { data = newData; }
};

// C-style wrapper functions for maximum compatibility
LIBRARY_API JSONHandlerLib* CreateHandler();
LIBRARY_API void DestroyHandler(JSONHandlerLib* handler);
LIBRARY_API bool ProcessCommand(JSONHandlerLib* handler, const char* command, const char* params);

#ifdef __cplusplus
}
#endif

#endif // JSON_HANDLER_LIB_H