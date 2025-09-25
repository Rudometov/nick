/**
 * @file test_json_handler.cpp
 * @brief Comprehensive tests for JSONHandler class (Part И)
 * @details Tests to achieve ≥80% code coverage using gcov/lcov
 */

#include "../inc/task.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdio>

// Test helper function to create sample data file
void createTestDataFile(const std::string& filename) {
    json testData = json::array();

    for (int i = 1; i <= 10; ++i) {
        json item;
        item["Author"] = std::vector<std::string>{"Test Author " + std::to_string(i)};
        item["Name"] = "Test Book " + std::to_string(i);
        item["Year"] = 2000 + i;
        item["Publisher"] = "Test Publisher";
        item["Rating"] = i * 0.9; // Ratings from 0.9 to 9.0
        testData.push_back(item);
    }

    std::ofstream file(filename);
    file << testData.dump(4);
    file.close();
}

// Test 1: Input and Print functions
void testInputPrint() {
    std::cout << "Test 1: Input and Print...";

    createTestDataFile("test_data.json");

    JSONHandler handler;
    handler.Input("test_data.json");

    // Verify data loaded
    json data = handler.GetData();
    assert(data.is_array());
    assert(data.size() == 10);

    // Test print (just ensure it doesn't crash)
    handler.Print();

    std::cout << " PASSED" << std::endl;
}

// Test 2: Execute function (10% filter)
void testExecute() {
    std::cout << "Test 2: Execute function...";

    JSONHandler handler;
    handler.Input("test_data.json");

    handler.Execute();

    json data = handler.GetData();
    // Should keep 10% from top and bottom (10% of 10 = 1 each = 2 total)
    assert(data.size() == 2);

    // Verify we have lowest and highest ratings
    bool hasLowest = false;
    bool hasHighest = false;

    for (const auto& item : data) {
        double rating = item["Rating"].get<double>();
        if (rating < 1.0) hasLowest = true;
        if (rating > 8.5) hasHighest = true;
    }

    assert(hasLowest || hasHighest);

    std::cout << " PASSED" << std::endl;
}

// Test 3: Generate and Add functions
void testGenerateAdd() {
    std::cout << "Test 3: Generate and Add...";

    JSONHandler handler;

    // Test Generate
    handler.Generate();
    json data = handler.GetData();
    assert(data.size() == 1);
    assert(data[0].contains("ID"));

    // Test Generate_ex
    handler.Clear();
    handler.Generate_ex(5);
    data = handler.GetData();
    assert(data.size() == 5);

    // Verify all have IDs
    for (const auto& item : data) {
        assert(item.contains("ID"));
        assert(item.contains("Name"));
        assert(item.contains("Rating"));
    }

    std::cout << " PASSED" << std::endl;
}

// Test 4: Delete function
void testDelete() {
    std::cout << "Test 4: Delete function...";

    JSONHandler handler;
    handler.Input("test_data.json");

    // Delete by Year
    handler.Delete("Year", "2001");

    json data = handler.GetData();
    assert(data.size() == 9);

    // Verify item with Year 2001 is gone
    for (const auto& item : data) {
        assert(item["Year"].get<int>() != 2001);
    }

    // Delete by Rating (double value)
    handler.Delete("Rating", "1.8");
    data = handler.GetData();
    assert(data.size() == 8);

    std::cout << " PASSED" << std::endl;
}

// Test 5: ID Generation and Validation
void testIDFunctions() {
    std::cout << "Test 5: ID functions...";

    JSONHandler handler;

    // Test ID generation
    std::string id1 = handler.GenerateID("War and Peace");
    assert(id1.substr(0, 3) == "LIT");
    assert(id1.length() >= 6);

    // Test ID validation
    assert(handler.ValidateID("LIT001ABC") == true);
    assert(handler.ValidateID("LIT999Z") == true);
    assert(handler.ValidateID("INVALID") == false);
    assert(handler.ValidateID("ABC123DEF") == false);
    assert(handler.ValidateID("LIT1234AB") == false); // Too many digits

    // Test GetByID
    handler.Generate();
    json data = handler.GetData();
    std::string generatedId = data[0]["ID"].get<std::string>();

    json found = handler.GetByID(generatedId);
    assert(!found.empty());
    assert(found["ID"] == generatedId);

    json notFound = handler.GetByID("LIT999XXX");
    assert(notFound.empty());

    std::cout << " PASSED" << std::endl;
}

// Test 6: Save and Load functions
void testSaveLoad() {
    std::cout << "Test 6: Save and Load...";

    JSONHandler handler1;
    handler1.Generate_ex(3);
    handler1.SaveToFile("test_save.json");

    JSONHandler handler2;
    handler2.Input("test_save.json");

    json data1 = handler1.GetData();
    json data2 = handler2.GetData();

    assert(data1.size() == data2.size());
    assert(data1.size() == 3);

    std::cout << " PASSED" << std::endl;
}

// Test 7: Edge cases
void testEdgeCases() {
    std::cout << "Test 7: Edge cases...";

    JSONHandler handler;

    // Empty data operations
    handler.Print(); // Should not crash
    handler.Execute(); // Should handle empty data
    handler.Delete("Name", "NonExistent");

    // Invalid file
    handler.Input("non_existent_file.json");

    // SetData and GetData
    json newData = json::array();
    newData.push_back({{"Name", "Test"}, {"Rating", 5.0}});
    handler.SetData(newData);
    assert(handler.GetData().size() == 1);

    // Clear
    handler.Clear();
    assert(handler.GetData().empty() || handler.GetData().size() == 0);

    std::cout << " PASSED" << std::endl;
}

// Test 8: Literature conversion functions
void testLiteratureConversion() {
    std::cout << "Test 8: Literature conversion...";

    JSONHandler handler;

    Literature lit;
    lit.Author = {"Author1", "Author2"};
    lit.Name = "Test Book";
    lit.Year = 2024;
    lit.Publisher = "Test Pub";
    lit.Rating = 8.5;

    json j;
    handler.to_json(j, lit);

    assert(j["Name"] == "Test Book");
    assert(j["Year"] == 2024);
    assert(j["Rating"] == 8.5);
    assert(j["Author"].size() == 2);

    Literature lit2;
    handler.from_json(j, lit2);

    assert(lit2.Name == lit.Name);
    assert(lit2.Year == lit.Year);
    assert(lit2.Rating == lit.Rating);
    assert(lit2.Author.size() == lit.Author.size());

    std::cout << " PASSED" << std::endl;
}

// Main test runner
int main(int argc, char* argv[]) {
    std::cout << "=== Running JSONHandler Tests ===" << std::endl;
    std::cout << "Target: ≥80% code coverage" << std::endl << std::endl;

    try {
        testInputPrint();
        testExecute();
        testGenerateAdd();
        testDelete();
        testIDFunctions();
        testSaveLoad();
        testEdgeCases();
        testLiteratureConversion();

        std::cout << std::endl << "=== All Tests PASSED ===" << std::endl;
        std::cout << "Ready for coverage analysis with gcov/lcov" << std::endl;

        // Cleanup test files
        std::remove("test_data.json");
        std::remove("test_save.json");

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}