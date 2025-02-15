# Numeric Operations Library

## Overview
This project provides a **Numeric** class hierarchy that supports various numeric types (integers, floating-point numbers, complex numbers, and characters) while performing arithmetic and comparison operations in a type-safe manner.

The implementation follows object-oriented design principles and uses **polymorphism** and **smart pointers** to manage different numeric types dynamically.

## Features
- Supports **int**, **float**, **double**, **complex numbers**, and **char** as numeric types.
- Implements arithmetic operations: **addition, subtraction, multiplication, and division**.
- Supports comparison operations: **less than, greater than, and equality**.
- Uses **std::unique_ptr** to manage polymorphic objects.
- Provides a **factory method** (`Numeric::create`) to simplify object creation.
- Sorts numeric elements using the `lessThanOperation` function.

## Project Requirements Followed
- **Encapsulation & Modularity**: The project separates declaration and implementation files.
- **Memory Safety**: Utilizes smart pointers (`std::unique_ptr`) to avoid memory leaks.
- **Type-Safety**: Uses **C++ templates** and **polymorphism** to handle different numeric types.
- **Error Handling**: Implements exception handling for invalid operations (e.g., division by zero).

## Installation
1. Clone the repository:
   ```sh
   git clone git@github.com:OmarEltotongy/Generic-Numeric-Data-Type.git
   cd numeric-operations
   ```
2. Compile the project using a C++17 or later compiler:
   ```sh
   g++ -std=c++17 -o numeric_operations main.cpp src/Numeric.cpp -Iinclude
   ```
3. Run the program:
   ```sh
   ./numeric_operations
   ```

## Usage
The program initializes a vector with different numeric types, performs arithmetic and comparison operations, and sorts the results.

### Example Output
```sh
Initial values in the vector:
10
5.5
3.14159
(1,2)
A

Operations for element 0 (10):
With element 1 (5.5):
Addition: 10 + 5.5 = 15.5
Subtraction: 10 - 5.5 = 4.5
Multiplication: 10 * 5.5 = 55
Division: 10 / 5.5 = 1.81818
Comparison (Less Than): 10 < 5.5 = false
Comparison (Greater Than): 10 > 5.5 = true
Comparison (Equal): 10 == 5.5 = false

Sorted values in the vector (ascending order):
A
(1,2)
3.14159
5.5
10
```

## Code Structure
```
📂 numeric-operations/
│── 📂 include/
│   ├── Numeric.hpp     # Base class definition
│── 📂 src/
│   ├── Numeric.cpp     # Implementation of Numeric class
│── main.cpp            # Entry point and execution logic
│── README.md           # Documentation (this file)
```

## Future Enhancements
- Implementing **operator overloading** for better usability.
- Extending support for additional numeric types.
- Providing serialization support to store and load numeric objects.


