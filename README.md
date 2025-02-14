# Generic Numeric Data Type

This repository contains a C++ implementation of a generic numeric data type system that supports various numeric types, including integers, floating-point numbers, complex numbers, and characters. The system is designed to be extensible and type-safe, leveraging modern C++ features such as templates, polymorphism, and concepts.

## Features

- **Polymorphic Numeric Base Class**: The `Numeric` class serves as a base class for all numeric types, providing a common interface for arithmetic operations and comparisons.
- **Derived Numeric Types**:
  - `IntNumeric`: Represents integer values.
  - `FloatNumeric`: Represents floating-point values (float, double).
  - `ComplexNumeric`: Represents complex numbers.
  - `charNumeric`: Represents character values.
- **Factory Pattern**: A factory function `Numeric::create` is provided to create instances of the appropriate numeric type based on the input value.
- **Type Safety**: The system ensures type safety by using `dynamic_cast` to verify types at runtime and throwing exceptions for invalid operations.
- **Exception Handling**: Comprehensive exception handling is implemented to manage errors such as type mismatches, division by zero, and memory allocation failures.
- **Debugging Support**: Debugging output can be enabled using the `DEBUG` macro to trace constructor and destructor calls.

---

## Classes and Their Functionality

### 1. **Base Class: `Numeric`**
This is an abstract base class that defines the interface for all numeric types. It includes pure virtual functions for arithmetic operations (`sumOperation`, `subtractOperation`, etc.) and comparisons (`lessThanOperation`, `greaterThanOperation`, etc.).

```cpp
class Numeric {
public:
    virtual ~Numeric() = default;
    virtual std::unique_ptr<Numeric> sumOperation(const Numeric& second) = 0;
    virtual std::unique_ptr<Numeric> subtractOperation(const Numeric& second) = 0;
    virtual std::unique_ptr<Numeric> multiplyOperation(const Numeric& second) = 0;
    virtual std::unique_ptr<Numeric> divideOperation(const Numeric& second) = 0;
    virtual bool lessThanOperation(const Numeric& second) = 0;
    virtual bool greaterThanOperation(const Numeric& second) = 0;
    virtual bool equalOperation(const Numeric& second) = 0;
    virtual std::string toString() const = 0;
};
```

### 2. **Derived Class: `IntNumeric`**
Represents integer values. Implements all virtual functions from the `Numeric` base class.

```cpp
class IntNumeric : public Numeric {
public:
    int intValue;
    IntNumeric(int value) : intValue(value) {}

    std::unique_ptr<Numeric> sumOperation(const Numeric& second) override {
        const IntNumeric& secondInt = dynamic_cast<const IntNumeric&>(second);
        return std::make_unique<IntNumeric>(intValue + secondInt.intValue);
    }
    // Other operations implemented similarly...
};
```

### 3. **Derived Class: `FloatNumeric` (Template)**
Represents floating-point values (float, double). Uses C++ concepts to restrict the template to floating-point types.

```cpp
template <FloatingPoint T>
class FloatNumeric : public Numeric {
public:
    T floatValue;
    FloatNumeric(T value) : floatValue(value) {}

    std::unique_ptr<Numeric> sumOperation(const Numeric& second) override {
        const FloatNumeric<T>& secondFloat = dynamic_cast<const FloatNumeric<T>&>(second);
        return std::make_unique<FloatNumeric<T>>(floatValue + secondFloat.floatValue);
    }
    // Other operations implemented similarly...
};
```

### 4. **Derived Class: `ComplexNumeric` (Template)**
Represents complex numbers. Uses `std::complex<T>` for storage and implements arithmetic operations.

```cpp
template <typename T>
class ComplexNumeric : public Numeric {
public:
    std::complex<T> complexNum;
    ComplexNumeric(std::complex<T> num) : complexNum(num) {}

    std::unique_ptr<Numeric> sumOperation(const Numeric& second) override {
        const ComplexNumeric<T>& secondComplex = dynamic_cast<const ComplexNumeric<T>&>(second);
        return std::make_unique<ComplexNumeric<T>>(complexNum + secondComplex.complexNum);
    }
    // Other operations implemented similarly...
};
```

### 5. **Derived Class: `charNumeric` (Template)**
Represents character values. Supports addition and subtraction but throws exceptions for unsupported operations like multiplication and division.

```cpp
template <charTemp T>
class charNumeric : public Numeric {
public:
    T charValue;
    charNumeric(T value) : charValue(value) {}

    std::unique_ptr<Numeric> sumOperation(const Numeric& second) override {
        const charNumeric<T>& secondChar = dynamic_cast<const charNumeric<T>&>(second);
        return std::make_unique<charNumeric<T>>(toascii(charValue + secondChar.charValue));
    }
    // Other operations implemented similarly...
};
```

---

## Main Function Explained

The `main` function demonstrates the usage of the numeric types. It creates instances of different numeric types, performs arithmetic operations, sorts the values, and handles exceptions.

### Key Steps in the `main` Function:

1. **Create Vectors for Each Numeric Type**:
   ```cpp
   std::vector<std::unique_ptr<Numeric>> intNumbers;
   std::vector<std::unique_ptr<Numeric>> floatNumbers;
   std::vector<std::unique_ptr<Numeric>> doubleNumbers;
   std::vector<std::unique_ptr<Numeric>> complexNumbers;
   std::vector<std::unique_ptr<Numeric>> charNumbers;
   ```

2. **Populate Vectors with Numeric Values**:
   ```cpp
   intNumbers.push_back(Numeric::create(5));
   floatNumbers.push_back(Numeric::create(3.14f));
   complexNumbers.push_back(Numeric::create(std::complex<double>(1, 2)));
   charNumbers.push_back(Numeric::create('A'));
   ```

3. **Perform Arithmetic Operations**:
   ```cpp
   auto result = intNumbers[0]->sumOperation(*intNumbers[1]);
   std::cout << "Sum: " << result->toString() << std::endl;
   ```

4. **Sort Numeric Values**:
   ```cpp
   std::sort(intNumbers.begin(), intNumbers.end(), [](const auto &a, const auto &b) {
       return a->lessThanOperation(*b);
   });
   ```

5. **Handle Exceptions**:
   ```cpp
   try {
       auto zero = Numeric::create(0);
       auto number = Numeric::create(10);
       auto result = number->divideOperation(*zero);
   } catch (const std::runtime_error &e) {
       std::cerr << "Exception: " << e.what() << std::endl;
   }
   ```

---

## Example Output

### Integer Operations:
```
Integer Addition Results:
5 + 10 = 15
10 + 2 = 12
2 + -7 = -5

Sorted Integer Values:
-7 2 5 10
```

### Floating-Point Operations:
```
Float Addition Results:
3.14 + 2.71 = 5.85
2.71 + 1.41 = 4.12
1.41 + -0.99 = 0.42

Sorted Float Values:
-0.99 1.41 2.71 3.14
```

### Complex Number Operations:
```
Complex Addition Results:
(1.00 + 2.00i) + (3.00 + 4.00i) = (4.00 + 6.00i)
(3.00 + 4.00i) + (0.00 + -1.00i) = (3.00 + 3.00i)
(0.00 + -1.00i) + (-2.00 + -3.00i) = (-2.00 + -4.00i)

Sorted Complex Values:
(-2.00 + -3.00i) (0.00 + -1.00i) (1.00 + 2.00i) (3.00 + 4.00i)
```

### Exception Handling:
```
Exception caught: Division by zero -> divideOperation: Division by zero is not allowed.
Exception caught: Type mismatch -> std::bad_cast
```

---

## Building and Running

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/OmarEltotongy/Generic-Numeric-Data-Type.git
   cd Generic-Numeric-Data-Type
   ```

2. **Compile the Code**:
   ```bash
   g++ -std=c++20 -o numeric_main main.cpp
   ```

3. **Run the Program**:
   ```bash
   ./numeric_main
   ```

---
Feel free to explore the code, experiment with different numeric types, and extend the system to support additional types or operations. If you have any questions or feedback, don't hesitate to reach out!