# Public Transport Management System

## Overview

This project is a comprehensive, console-based application developed in C++ for the management and simulation of a city's public transportation network. [cite_start]It provides a detailed framework for handling various types of public transport vehicles, their routes, and the depots they are housed in. The system supports multiple transport networks (e.g., surface transport like STB and an underground metro system) and allows for data persistence by loading from and saving to text files. [cite: 6]

The application is built using advanced C++ features, demonstrating a deep understanding of Object-Oriented Programming principles, the Standard Template Library (STL), design patterns, and exception handling.

---

## Features

- **System and Line Management**: Create and manage distinct transport systems (e.g., `STB`, `Metrorex`). [cite: 26, 23] Within each system, define, add, or remove transport lines and their respective stations.
- **Vehicle Fleet Management**: Manage depots that house various public transport vehicles. [cite: 2, 25] Add new vehicles of different types (`Bus`, `Tram`, `Trolleybus`, `Metro`) to a depot.
- **Vehicle Configuration**: Modify vehicle properties, such as assigning a vehicle to a different line.
- **Polymorphic Vehicle Interaction**: Utilize type-specific functionalities for vehicles, such as managing the low-floor feature for a `Bus` or the overhead wire connection for a `Trolleybus`.
- **Live Ride Simulation**: Initiate a real-time simulation for any vehicle. The simulation shows the vehicle moving between stations on its assigned line, calculating travel times, and updating passenger capacity dynamically. The simulation can be stopped at any time by the user.
- **Data Persistence**: All system, line, and depot data is loaded from `.txt` files on startup and can be saved back, preserving the state of the application between sessions. [cite: 6]
- **Interactive Console Menu**: A robust, user-friendly menu built on the Singleton pattern allows the user to navigate through all the available management and simulation options. [cite: 7]

---

## Technical Details & Implementation

This project leverages several key C++ concepts to achieve its functionality.

### Object-Oriented Design

The core of the application is a hierarchy of classes that model the real-world components of a transport system.

- **Core Classes**: `Station`, `Line`, and `System` form the backbone of the transport network. A `System` contains a collection of `Line` objects, and each `Line` contains a sequence of `Station` objects.

- **Abstract Base Class**: `PublicTransport` is an abstract class that defines the common interface for all vehicles. It uses `virtual` functions to ensure polymorphic behavior.

- **Concrete Vehicle Classes**:
    - `Metro`, `Bus`, and `Tram` are concrete classes that inherit from `PublicTransport`. Each class adds properties and behaviors specific to that vehicle type.
    - **Multiple Inheritance**: The `Trolleybus` class inherits from both `Bus` and `Tram` to demonstrate a "diamond inheritance" scenario. [cite_start]This fulfills the project requirement of inheritance to a "grandchild" level (`Parent` -> `Child` -> `Grandchild`). [cite: 13] The ambiguity from inheriting common base members is resolved by explicitly specifying which base class members to use (e.g., `Bus::actual_capacity`).

### Design Patterns

- [cite_start]**Singleton Pattern**: The `Singleton` class is used to create and manage the main menu. [cite: 7] This ensures a single, global point of access for user interactions and orchestrates the application's lifecycle, including loading and saving data.

### Data Structures (STL)

[cite_start]The project utilizes several STL containers to efficiently manage data, as required: [cite: 5]
- `std::vector`: Used within the `System` class to store a dynamic array of `Line` objects and within the `Singleton` to manage the collections of systems and depots.
- `std::deque`: Chosen over `std::list` to represent the sequence of stations in a `Line`, as it provides efficient insertions and deletions at both the beginning and end of the route.
- `std::map`: Used in the `Depot` class to store and manage the fleet of vehicles. It maps a vehicle's unique name (`string`) to a pointer of type `PublicTransport*`, allowing for fast lookups.

### Polymorphism, Upcasting, and Downcasting

[cite_start]The project heavily demonstrates polymorphism as per the requirements. [cite: 12]

- **Upcasting**: All vehicle objects (`Metro`, `Bus`, etc.) are stored in the `Depot`'s map as `PublicTransport*` pointers. This is an implicit upcast and allows the collection to manage different vehicle types polymorphically.

- **Downcasting**: `dynamic_cast` is used extensively to safely downcast a `PublicTransport*` pointer back to its specific derived type. This is necessary for:
    - **Serialization**: When saving the depot state to a file, `dynamic_cast` determines the object's true type to write the correct identifier and data.
    - **Type-Specific Methods**: To invoke methods unique to a derived class, such as `switchLowFloor()` for a `Bus` or `changePlugStatus()` for a `Trolleybus`.

### Template Programming

- [cite_start]**Template Class**: The `Object<T>` class is a generic wrapper used to associate a `System` or `Depot` object with its corresponding data file path. [cite: 8] This template abstracts the logic for loading and saving different types of objects from files.

- [cite_start]**Template Method**: The `createAndRead<T>` method within the `Singleton` class is a template function that simplifies the polymorphic creation of new vehicles of any type (`Metro`, `Bus`, etc.), avoiding code duplication. [cite: 9]

### Exception Handling

[cite_start]The application implements robust exception handling to manage potential runtime errors gracefully, fulfilling the requirement to handle at least three different exception types. [cite: 11, 10] The code catches and reports various exceptions, including:
- `std::runtime_error`: For file I/O issues (e.g., file not found, read/write errors).
- `std::invalid_argument`: For invalid data entered by the user (e.g., a negative distance for a station).
- `std::logic_error`: For incorrect user choices in the menu or invalid vehicle types.
- `std::ios_base::failure`: For stream input failures.
- `std::bad_alloc`: For memory allocation failures during object creation.

### Concurrency

The vehicle ride simulation (`setTren` method) uses `std::thread` to run a non-blocking input loop. This allows the simulation to proceed in real-time while simultaneously listening for a user command (`exit`) to terminate the simulation, providing a more interactive user experience.

---

## File Structure

- `GestiuneaSTP3.cpp`: The main C++ source file containing all class definitions and application logic.
- [cite_start]`config_systems.txt`: Configuration file that lists the filenames for the transport systems to be loaded. [cite: 1]
- [cite_start]`config_depots.txt`: Configuration file that lists the filenames for the depots to be loaded. [cite: 22]
- [cite_start]`system_*.txt`: Data files defining transport systems, their lines, and stations (e.g., `system_metro.txt`). [cite: 23]
- [cite_start]`depoul_*.txt`: Data files defining depots and their vehicle fleets (e.g., `depoul_militari.txt`). [cite: 2]

---

## How to Compile and Run

### Prerequisites

- A C++ compiler that supports the C++17 standard (e.g., `g++` or `clang++`).
- All `.txt` data and configuration files must be in the same directory as the executable.

### Compilation

To compile the project, use the following command:

```sh
clang++ -std=c++17 -o GestiuneaSTP GestiuneaSTP.cpp
```

### Execution

To run the application, execute the compiled binary from your terminal:

```sh
./GestiuneaSTP
```
