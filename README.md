# JSON Data Handler System

A comprehensive C++ application for JSON data manipulation with client-server architecture, implementing all requirements from task document S02.E04.Build2025.03.01.0.

## 📋 Project Overview

This project implements a complete JSON processing system for managing Literature data structures with the following capabilities:
- Standalone JSON data manipulation
- Client-server architecture with HTTP REST API
- Batch data generation with unique ID system
- Advanced filtering and sorting operations

## 🎯 Task Requirements Implementation

This project fully implements all parts specified in `task-4-2-2025.pdf`:

### Part A: Input and Output (Часть А: Ввод и вывод данных)
- ✅ `Input()` - Load JSON data from files
- ✅ `Print()` - Display formatted JSON to console

### Part B: Data Processing (Часть Б: Обработка данных)
- ✅ `Execute()` - Task-specific function (keeps 10% top and bottom rated books)
- ✅ `Add()` - Interactive addition of new Literature items
- ✅ `Delete()` - Remove items by field value
- ✅ `Generate()` - Generate random Literature items

### Part C: CMake Build System (Часть В: Файл сборки CMakeLists)
- ✅ Hierarchical CMake structure with subdirectories
- ✅ Automatic data file copying to build directory
- ✅ Cross-platform configuration

### Part D: Client-Server Architecture (Часть Г: HTTPLib)
- ✅ HTTP server with JSON command processing
- ✅ HTTP client with interactive menu
- ✅ Structured Send/Receive message format

### Part E: ID Generation (Часть Д: Идентификатор объекта)
- ✅ Unique ID generation (format: `LIT###XXX`)
- ✅ Regex-based ID validation
- ✅ ID search functionality

### Part F: Batch Generation (Часть Е: Генератор данных)
- ✅ `Generate_ex()` - Generate multiple items at once
- ✅ Configurable batch size

## 📁 Project Structure

```
task/
├── README.md                # This file
├── CMakeLists.txt          # Root CMake configuration
├── inc/                    # Header files
│   └── task.h             # Main JSONHandler class
├── src/                    # Source files
│   ├── main.cpp           # Standalone application
│   └── task.cpp           # JSONHandler implementation
├── client/                 # Client implementation
│   ├── CMakeLists.txt
│   ├── inc/
│   │   └── client.h
│   └── src/
│       ├── client.cpp
│       └── main_client.cpp
├── server/                 # Server implementation
│   ├── CMakeLists.txt
│   ├── inc/
│   │   └── server.h
│   └── src/
│       ├── server.cpp
│       └── main_server.cpp
├── data/                   # Data files
│   └── data.json          # Sample Literature data
├── ext/                    # External libraries
│   ├── nlohmann/
│   │   └── json.hpp       # JSON processing library
│   └── httplib.h          # HTTP client/server library
└── build/                  # Build output (generated)
    └── bin/               # Executables
```

## 🛠️ Prerequisites

- **C++ Compiler**: C++17 compatible (GCC, Clang, MSVC)
- **CMake**: Version 3.10 or higher
- **Operating System**: Linux, macOS, or Windows

### Installing Prerequisites on macOS
```bash
brew install cmake
```

### Installing Prerequisites on Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install cmake build-essential
```

## 🔨 Building the Project

1. Clone or extract the project to your local machine

2. Navigate to the project directory:
```bash
cd task
```

3. Create and enter the build directory:
```bash
mkdir build
cd build
```

4. Configure the project with CMake:
```bash
cmake ..
```

5. Build the project:
```bash
cmake --build .
# or simply
make
```

After successful build, you will find three executables in `build/bin/`:
- `JSONHandler` - Standalone application
- `JSONServer` - HTTP server
- `JSONClient` - HTTP client

## 🚀 Usage

### Standalone Mode

Run the standalone application:
```bash
cd build/bin
./JSONHandler
```

Menu options:
1. **Load data from file** - Input JSON data from a file
2. **Display current data** - Print loaded data
3. **Execute task function** - Apply 10% top/bottom filter
4. **Add new item** - Interactively add a Literature item
5. **Delete item by field** - Remove items matching criteria
6. **Generate random item** - Create one random item
7. **Generate multiple items** - Batch generation
8. **Save data to file** - Export current data
9. **Validate ID** - Check if an ID is valid
10. **Get item by ID** - Search by ID
0. **Exit** - Close application

### Client-Server Mode

1. **Start the server** (in one terminal):
```bash
cd build/bin
./JSONServer [port]
# Default port is 8080
./JSONServer 8080
```

2. **Start the client** (in another terminal):
```bash
cd build/bin
./JSONClient [host] [port]
# Default is localhost:8080
./JSONClient 127.0.0.1 8080
```

The client provides a similar menu interface that communicates with the server via HTTP.

### Example Workflow

1. Load sample data:
```
Choice: 1
Filename: data.json
```

2. Execute task (filter by ratings):
```
Choice: 3
```

3. Save filtered results:
```
Choice: 8
Filename: output.json
```

## 📊 Data Structure

The system manages Literature items with the following structure:
```json
{
  "Author": ["Author Name"],
  "Name": "Book Title",
  "Year": 2024,
  "Publisher": "Publisher Name",
  "Rating": 8.5,
  "ID": "LIT001BOO"
}
```

## 🧪 Testing

Comprehensive testing has been performed covering all requirements:

### Test Coverage

| Component | Test Type | Status |
|-----------|-----------|--------|
| Input/Output | File loading, JSON parsing | ✅ Passed |
| Execute Function | Rating filter (10% top/bottom) | ✅ Passed |
| Add/Delete | CRUD operations | ✅ Passed |
| Generate | Random data creation | ✅ Passed |
| ID System | Generation & validation | ✅ Passed |
| Batch Generation | Multiple item creation | ✅ Passed |
| Client-Server | HTTP communication | ✅ Passed |
| CMake Build | All platforms | ✅ Passed |

### Test Commands

#### Test Input/Output
```bash
echo -e "1\ndata.json\n2\n0" | ./JSONHandler
```

#### Test Execute Function
```bash
echo -e "1\ndata.json\n3\n8\noutput.json\n0" | ./JSONHandler
```

#### Test Generate
```bash
echo -e "6\n8\ngenerated.json\n0" | ./JSONHandler
```

#### Test Batch Generation
```bash
echo -e "7\n5\n8\nbatch.json\n0" | ./JSONHandler
```

#### Test ID Validation
```bash
echo -e "9\nLIT001ABC\n9\nINVALID\n0" | ./JSONHandler
```

#### Test Client-Server
```bash
# Terminal 1: Start server
./JSONServer 8080

# Terminal 2: Test with curl
curl -X GET http://127.0.0.1:8080/ping
curl -X POST http://127.0.0.1:8080/command \
  -H "Content-Type: application/json" \
  -d '{"Command":"generate"}'
```

### Test Results

All tests pass successfully. Detailed test results are available in `TEST_RESULTS.md`.

## 📚 External Libraries

- **[nlohmann/json](https://github.com/nlohmann/json)** - JSON for Modern C++
- **[cpp-httplib](https://github.com/yhirose/cpp-httplib)** - C++ header-only HTTP/HTTPS library

## 🔧 Configuration

### Server Configuration
- Default port: 8080
- Alternative port: 8081
- Host: 127.0.0.1 (localhost)

### ID Format
- Pattern: `LIT[0-9]{3}[A-Z]{1,3}`
- Example: `LIT001WAR`, `LIT002CRI`

## 📝 Task Document Reference

This implementation follows all requirements specified in:
- **Document**: S02.E04.Build2025.03.01.0
- **Task**: Задача 2. JSON
- **Variant**: Task 1 - Literature data structure

Key requirements implemented:
1. Cross-platform C++ application
2. JSON data manipulation using nlohmann library
3. HTTP client-server architecture using cpp-httplib
4. CMake build system
5. Specific task function (10% rating filter)
6. ID generation and validation system
7. Batch data generation

## 🤝 Contributing

This project was created as a complete implementation of the specified task requirements. All features have been thoroughly tested and verified.

## 📄 License

This project is created for educational purposes as part of the specified task requirements.

## ✅ Completion Status

**All requirements from the task document have been successfully implemented and tested.**

For detailed test results, see `TEST_RESULTS.md`.