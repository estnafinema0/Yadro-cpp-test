# Computer Club Management System

This is a C++ object-oriented simulation of a computer club management system. The project is organized into clear modules that handle different parts of the system. The project includes unit tests for each module (using Google Test) as well as integration tests.

---

## Table of Contents

- [Computer Club Management System](#computer-club-management-system)
  - [Table of Contents](#table-of-contents)
  - [Overview](#overview)
  - [Setup and Build](#setup-and-build)
    - [Install Prerequisites](#install-prerequisites)
    - [Build Main Application](#build-main-application)
  - [Run the Application](#run-the-application)
  - [Testing](#testing)
    - [Unit Tests](#unit-tests)
    - [Integration Tests](#integration-tests)
  - [Usage Example](#usage-example)

---

## Overview

A full description is available in Russian in the root directory as [Original_task.pdf](Yadro/Original_task.pdf).

The project features a clear modular structure. Each module has its own unit tests (written with Google Test). Integration tests for the whole project are also provided and include a bash script for automated execution.

The program processes a text file containing a club configuration and a sequence of events (client arrivals, seating, waiting, departures), applies business rules (such as not allowing a client to wait when tables are free and automatically seating waiting clients when a table is freed), and generates a detailed report that includes error events and revenue summaries for each table.

Key modules include:

- **Parser Module:** Reads and validates input from a text file.
- **Time Module:** Converts string representations (in the format "HH:MM") into integer minutes (using `std::optional` for error handling).
- **Club Module:** Implements the club's business logic for client management, seating, waiting, error reporting, and revenue calculation.
- **Main:** Lauches the parsing and processing of events and outputs a final report.
- **Unit Tests:** Written for [each module](Yadro/unit-tests) using Google Test.
- **Integration Tests:** Five input and expected output files are located in the [tests folder](Yadro/tests), with a bash script for automated execution.

---

## Setup and Build

### Install Prerequisites

The project is designed for a C++20 compiler. For testing, GNU Make and Google Test are also required.

To install Google Test, run:
```bash
sudo apt update
sudo apt install libgtest-dev
```
Then build and install the libraries:
```bash
cd /usr/src/gtest
sudo cmake .
sudo make
sudo cp lib/*.a /usr/lib
```

### Build Main Application

In the `Yadro/project` folder, run:
```bash
make
```
This command compiles the project and produces an executable named `main`.

To clean all object files, run:
```bash
make clean
```

---

## Run the Application

You can run the main application from the `Yadro/project/` directory.  
The application requires a single argument: the input file (a test file with club instructions):

```bash
./main input_file.txt
```

For example, to run an integration test:
```bash
./main ../tests/inputs/test1.in.txt
```

The output will be printed to the console.

---

## Testing

### Unit Tests

Unit tests are organized into three groups `Yadro/unit-tests/time/`, `Yadro/unit-tests/parser/`, `Yadro/unit-tests/club/` for one module each.

1. **Time tests** validate correct parsing and formatting of time strings.
2. **Parser tests** checks that configuration lines and event lines are parsed correctly and that errors are detected as specified. If any errors detected the program stops (see the instruction). 
3. **Club tests** do the business logic for client management, seating, waiting queue handling, and end-of-day processing.


Choose the module to check and in the chosen folder (`club`, `parser` and `time`) run:

```bash
make
./run_tests
```

### Integration Tests

The script iterates over all input files, runs the application, and compares the produced output with the expected output. Differences (if any happened) are displayed in the termanal.

To make test script executable in the `Yadro/tests/` folder, run: 
```bash
chmod +x test_script.sh
```
Run the test script:
```bash
./test_script.sh
```

---

## Usage Example

Example of test file 1 (`test1.in.txt`):
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

You will see the following output in the terminal:
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

---

Thanks for checking!

---