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

## SumOperation Function Explanation

## Overview
The `sumOperation` function is a member of a class (likely `FloatNumeric<T>`) that overrides a virtual function from a base class. It performs an addition operation between the current object (`*this`) and another `Numeric` type object (`second`). The function handles different types of `Numeric` objects dynamically and ensures the correct arithmetic operations are applied.

## Function Logic
### 1. Checking if `second` is a `FloatNumeric<T>`
```cpp
try {
    const FloatNumeric<T>& secondFloat = dynamic_cast<const FloatNumeric<T>&>(second);
    return std::make_unique<FloatNumeric<T>>(floatValue + secondFloat.floatValue);
}
```
- **If `second` is a `FloatNumeric<T>`**, it is safely cast and added directly.
- If `second` is **not** a `FloatNumeric<T>`, a `std::bad_cast` exception is thrown.

### 2. Checking if `second` is a `ComplexNumeric<T>`
```cpp
catch (const std::bad_cast&) {
    if (typeid(second) == typeid(ComplexNumeric<T>)) {
        const ComplexNumeric<T>& secondComplex = dynamic_cast<const ComplexNumeric<T>&>(second);
        return std::make_unique<ComplexNumeric<T>>(std::complex<T>(floatValue + secondComplex.complexNum.real(), secondComplex.complexNum.imag()));
    }
```
- If `second` is a `ComplexNumeric<T>`, it is cast correctly.
- The real part of `secondComplex` is added to `floatValue`, while the imaginary part remains unchanged.

### 3. Handling Other Numeric Types
```cpp
    else {
        auto converted = second.convertTo(typeid(*this));
        const FloatNumeric<T>& secondFloat = dynamic_cast<const FloatNumeric<T>&>(*converted);
        return std::make_unique<FloatNumeric<T>>(floatValue + secondFloat.floatValue);
    }
```
- If `second` is neither `FloatNumeric<T>` nor `ComplexNumeric<T>`, it is converted to `FloatNumeric<T>`.
- Once converted, the addition is performed normally.

### 4. Handling Memory Allocation Failures
```cpp
catch (const std::bad_alloc& e) {
    throw std::runtime_error("sumOperation: Memory allocation failed.");
}
```
- If `std::make_unique<>` fails due to memory allocation issues, it throws a `std::runtime_error`.

## Summary
1. **Tries to cast `second` to `FloatNumeric<T>`**:
   - If successful, adds values directly.
2. **If `second` is `ComplexNumeric<T>`**:
   - Adds to the real part and keeps the imaginary part unchanged.
3. **If `second` is an unknown `Numeric` type**:
   - Converts it to `FloatNumeric<T>` and performs the addition.
4. **Handles memory allocation errors**.

This function ensures flexible and type-safe addition operations across different `Numeric` types.


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


