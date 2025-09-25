#include "../inc/task.h"
#include <iostream>
#include <string>

void printMenu() {
    std::cout << "\n====== JSON Handler Menu ======" << std::endl;
    std::cout << "1. Load data from file (Input)" << std::endl;
    std::cout << "2. Display current data (Print)" << std::endl;
    std::cout << "3. Execute task function" << std::endl;
    std::cout << "4. Add new item" << std::endl;
    std::cout << "5. Delete item by field" << std::endl;
    std::cout << "6. Generate random item" << std::endl;
    std::cout << "7. Generate multiple items (Generate_ex)" << std::endl;
    std::cout << "8. Save data to file" << std::endl;
    std::cout << "9. Validate ID" << std::endl;
    std::cout << "10. Get item by ID" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "Enter your choice: ";
}

int main() {
    JSONHandler handler;
    int choice;
    std::string filename, field, value, id;
    int count;

    std::cout << "Welcome to JSON Data Handler!" << std::endl;

    while (true) {
        printMenu();
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        switch (choice) {
            case 1:
                std::cout << "Enter filename to load (default: data.json): ";
                std::cin >> filename;
                if (filename.empty()) filename = "data.json";
                handler.Input(filename);
                break;

            case 2:
                handler.Print();
                break;

            case 3:
                handler.Execute();
                break;

            case 4:
                handler.Add();
                break;

            case 5:
                std::cout << "Enter field name: ";
                std::cin >> field;
                std::cout << "Enter value to match: ";
                std::cin >> value;
                handler.Delete(field, value);
                break;

            case 6:
                handler.Generate();
                break;

            case 7:
                std::cout << "Enter number of items to generate: ";
                std::cin >> count;
                if (count > 0) {
                    handler.Generate_ex(count);
                } else {
                    std::cout << "Invalid count." << std::endl;
                }
                break;

            case 8:
                std::cout << "Enter filename to save (default: output.json): ";
                std::cin >> filename;
                if (filename.empty()) filename = "output.json";
                handler.SaveToFile(filename);
                break;

            case 9:
                std::cout << "Enter ID to validate: ";
                std::cin >> id;
                if (handler.ValidateID(id)) {
                    std::cout << "ID '" << id << "' is valid." << std::endl;
                } else {
                    std::cout << "ID '" << id << "' is invalid." << std::endl;
                }
                break;

            case 10:
                std::cout << "Enter ID to search: ";
                std::cin >> id;
                {
                    json item = handler.GetByID(id);
                    if (!item.empty()) {
                        std::cout << "Found item:" << std::endl;
                        std::cout << std::setw(4) << item << std::endl;
                    } else {
                        std::cout << "Item with ID '" << id << "' not found." << std::endl;
                    }
                }
                break;

            case 0:
                std::cout << "Exiting... Goodbye!" << std::endl;
                return 0;

            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    }

    return 0;
}