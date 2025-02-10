# Yadro Computer Club Management System

Yadro is a command-line based simulation of a computer club management system. The project handles client arrivals, seating assignments, waiting queue management, departure events, error handling, and revenue reporting. It is designed using modern C++ (up to C++20) and leverages standard library features (including `std::optional` and `std::string_view`) to ensure robust input parsing and error handling.

---

## Table of Contents

- [Yadro Computer Club Management System](#yadro-computer-club-management-system)
  - [Table of Contents](#table-of-contents)
  - [Overview](#overview)
  - [Features](#features)
  - [Project Structure](#project-structure)
  - [Environment Setup and Build Instructions](#environment-setup-and-build-instructions)
  - [Running the Application](#running-the-application)
  - [Testing](#testing)
    - [Unit Tests](#unit-tests)
      - [How to Run Unit Tests](#how-to-run-unit-tests)
    - [Integration Tests](#integration-tests)
      - [Running Integration Tests](#running-integration-tests)
  - [Global Project Features and Design Details](#global-project-features-and-design-details)
  - [Usage Example](#usage-example)
  - [Dependencies](#dependencies)
  - [Debugging and Configuration](#debugging-and-configuration)
  - [License](#license)

---

## Overview

The Yadro Computer Club Management System is designed to simulate the operations of a computer club. It processes a text file containing a club configuration and a sequence of events (client arrivals, seating, waiting, departures), applies business rules (e.g., not allowing a client to wait when tables are free, automatically seating waiting clients when a table is freed), and generates a detailed report including error events and revenue summaries for each table.

Key components include:

- **Parser Module:** Reads and validates input from a text file.
- **Time Module:** Converts between string representations (in the format "HH:MM") and integer minute counts using `std::optional` for error handling.
- **Club Module:** Implements business logic for client management, seating, waiting, error reporting, and revenue calculation.
- **Main:** Orchestrates the parsing and processing of events and outputs a final report.

---

## Features

- **Input Parsing:**  
  - Reads configuration (number of tables, opening and closing times, hourly cost) and event lines.
  - Uses robust parsing routines with error handling via `std::optional`.
  
- **Client Management:**  
  - Processes client arrival (ID 1), seating (ID 2), waiting (ID 3), and departure (ID 4) events.
  - Generates error events (ID 13) for invalid operations (e.g., arriving outside of working hours, trying to sit at a busy table, etc.).
  - Handles automatic re-seating of waiting clients when a table is freed—except during end-of-day processing.

- **Revenue Reporting:**  
  - Calculates revenue per table based on rounded-up client usage time.
  - Outputs a final report showing table occupancy durations and revenues.

- **Test-Driven Development:**  
  - Comprehensive unit tests for individual modules (Time, Parser, Club) are provided.
  - Integration tests (input/output files) and a test script ensure end-to-end correctness.

- **Modern C++ Practices:**  
  - Uses `std::optional` for error-prone conversions.
  - Leverages `std::string_view` for efficient parsing.
  - Organizes code within the `Yadro` namespace.

---

## Project Structure

```
Yadro/
├── project/
│   ├── Club.hpp            # Club module interface
│   ├── Club.cpp            # Club module implementation
│   ├── Parser.hpp          # Parser interface
│   ├── Parser.cpp          # Parser implementation
│   ├── ParserHelpers.hpp   # Parsing helper functions
│   ├── Time.hpp            # Time conversion functions (in Yadro::Time namespace)
│   ├── Time.cpp            # (Optional) Implementation file (all functions are inline)
│   ├── Utils.hpp           # Utility functions in Yadro::Util namespace
│   ├── main.cpp            # Main application entry point
│   └── Makefile            # Build instructions for the project
├── tests/
│   ├── inputs/             # Integration test input files (*.in.txt)
│   │   ├── test1.in.txt
│   │   ├── test2.in.txt
│   │   ├── test3.in.txt
│   │   ├── test4.in.txt
│   │   └── test5.in.txt
│   ├── outputs/            # Expected output files (*.out.txt)
│   │   ├── test1.out.txt
│   │   ├── test2.out.txt
│   │   ├── test3.out.txt
│   │   ├── test4.out.txt
│   │   └── test5.out.txt
│   └── test_script.sh      # Shell script to run integration tests
└── unit-tests/
    ├── club/               # Unit tests for the Club module
    │   ├── club_test.cpp
    │   └── Makefile
    ├── parser/             # Unit tests for the Parser module
    │   ├── parser_test.cpp
    │   └── Makefile
    └── time/               # Unit tests for the Time module
        ├── time_test.cpp
        └── Makefile
```

---

## Environment Setup and Build Instructions

1. **Prerequisites:**
   - A C++20-compatible compiler (e.g., GCC 10+ or Clang 10+).
   - GNU Make.
   - For unit testing, [GoogleTest](https://github.com/google/googletest) is required (the Makefiles for unit tests link against `-lgtest -lgtest_main`).

2. **Building the Main Application:**
   - Navigate to the `Yadro/project/` directory.
   - Run:
     ```bash
     make
     ```
   - This command compiles the project and produces an executable named `main`.

3. **Making Test Scripts Executable:**
   - Navigate to the `Yadro/tests/` directory.
   - Run:
     ```bash
     chmod +x test_script.sh
     ```

---

## Running the Application

Once built, you can run the main application from the `Yadro/project/` directory. The application expects a single argument specifying the input file:

```bash
./main path/to/your/input_file.txt
```

For example, to run an integration test:
```bash
./main ../tests/inputs/test1.in.txt
```

The output will be printed to the console.

---

## Testing

### Unit Tests

Unit tests are organized into three groups:
- **Time Module:** Located in `Yadro/unit-tests/time/`
  - Tests (`time_test.cpp`) validate correct parsing and formatting of time strings.
- **Parser Module:** Located in `Yadro/unit-tests/parser/`
  - Tests (`parser_test.cpp`) ensure that configuration lines and event lines are parsed correctly and that errors are detected as specified.
- **Club Module:** Located in `Yadro/unit-tests/club/`
  - Tests (`club_test.cpp`) cover business logic for client management, seating, waiting queue handling, and end-of-day processing.
  
#### How to Run Unit Tests
1. Navigate to each unit test directory (e.g., `Yadro/unit-tests/club/`).
2. Run:
   ```bash
   make
   ./run_tests
   ```
3. Repeat for the other modules (`parser` and `time`).

### Integration Tests

Integration tests use input/output file pairs located in the `Yadro/tests/inputs/` and `Yadro/tests/outputs/` directories.

#### Running Integration Tests
1. Navigate to the `Yadro/tests/` directory.
2. Run the test script:
   ```bash
   ./test_script.sh
   ```
3. The script iterates over all input files, runs the application, and compares the produced output with the expected output. Differences (if any) are displayed.

---

## Global Project Features and Design Details

- **Robust Parsing:**  
  The project uses modern C++ features such as `std::optional` and `std::string_view` to robustly parse and validate time strings and integer values. Invalid formats are handled gracefully with clear error messages.

- **Business Logic:**  
  The `Club` module implements all business rules regarding client arrivals, seating, waiting, departure, and end-of-day processing. Special events (such as error events with ID 13 and automatic departure events with ID 11) are generated as per the requirements.

- **Separation of Concerns:**  
  Parsing logic is separated into dedicated helper modules (`ParserHelpers.hpp` and `Utils.hpp`), and all code is organized under the `Yadro` namespace to avoid global namespace pollution.

- **Testing Infrastructure:**  
  Both unit tests and integration tests are provided to ensure that each module works as intended and that the entire system meets the specification. A Bash script automates the integration testing process.

---

## Usage Example

Assume you have an input file `test1.in.txt` with the following content:
```
3
09:00 19:00
10
08:48 1 client1
09:41 1 client1
09:48 1 client2
09:52 3 client1
09:54 2 client1 1
10:25 2 client2 2
10:58 1 client3
10:59 2 client3 3
11:30 1 client4
11:35 2 client4 2
11:45 3 client4
12:33 4 client1
12:43 4 client2
15:52 4 client4
```

When you run:
```bash
./main test1.in.txt
```

The application will output (for example):
```
09:00
08:48 1 client1
08:48 13 NotOpenYet
09:41 1 client1
09:48 1 client2
09:52 3 client1
09:52 13 ICanWaitNoLonger!
09:54 2 client1 1
10:25 2 client2 2
10:58 1 client3
10:59 2 client3 3
11:30 1 client4
11:35 2 client4 2
11:35 13 PlaceIsBusy
11:45 3 client4
12:33 4 client1
12:33 12 client4 1
12:43 4 client2
15:52 4 client4
19:00 11 client3
19:00
1 70 05:58
2 30 02:18
3 90 08:01
```

This output includes the opening time, all processed events (including error events), the closing time, and a final report of table usage and revenue.

---

## Dependencies

- **Compiler:** A modern C++ compiler supporting C++20 (e.g., GCC 10+, Clang 10+).
- **GoogleTest:** Required for unit testing (installed via your package manager or built from source).
- **Make:** GNU Make is used for building both the project and the tests.

No external libraries are used in the main project code (only the standard C++ library).

---

## Debugging and Configuration

- **Logging and Output:**  
  All events processed by the system are logged to the console as specified.  
- **Configuration File:**  
  The application accepts a single input file containing configuration parameters and events.  
- **Modifying Build Options:**  
  To adjust compiler flags or build settings, edit the `Makefile` located in the `Yadro/project/` directory.

---

## License

*(Include your license information here.)*

---

This README provides a comprehensive guide for developers and users of the Yadro Computer Club Management System, ensuring that even newcomers can easily understand how to build, test, and use the project.