#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <concepts>
#include <type_traits>  // For std::is_floating_point
#include <complex>
#include <algorithm>
#include <stdexcept>


#define DEBUG 0
#define RUN 1

class Numeric
{
    public:
    Numeric()
    {
        #if DEBUG == 1
        std::cout << "Numeric Class constructor is called" << std::endl;
        #endif // DEBUG

    }

    // Factory function to create derived objects
    template <typename T>
    static std::unique_ptr<Numeric> create(T value);

    virtual std::unique_ptr<Numeric> sumOperation(const Numeric& second ) = 0;
    virtual std::unique_ptr<Numeric> subtractOperation( const Numeric& second ) = 0;
    virtual std::unique_ptr<Numeric> multiplyOperation(const Numeric& second ) = 0;
    virtual std::unique_ptr<Numeric> divideOperation(const Numeric& second ) = 0;
    virtual bool lessThanOperation(const Numeric& second ) =0;
    virtual bool greaterThanOperation(const Numeric& second ) =0;
    virtual bool equalOperation(const Numeric& second ) =0;
    virtual std::string toString() const = 0;

    virtual ~Numeric()
    {
        #if DEBUG == 1
        std::cout << "Numeric Class destructor is called" << std::endl;
        #endif // DEBUG
    }

};


/* When to Use dynamic_cast?
When working with polymorphic base classes (Numeric has virtual functions).
When you need to convert a base class reference/pointer to a derived class safely.
When there’s a possibility that the object is not the expected type.*/

/**Templates do not allow me to use runtime polymorphism as easily.
 * So, if i want to have different types (e.g., integer vs. floating-point vs. complex)
 *  in the same container, a base class with polymorphism (like my current design) becomes more useful */

 //firstly a derived class with polymorphism
class IntNumeric : public Numeric
{
    public:
        int intValue;

    IntNumeric(int value) : intValue(value)
    {
        #if DEBUG == 1
        std::cout << "IntNumeric Class constructor with value: " << value << std::endl;
        #endif // DEBUG
    }

    // Helper function to cast a base class reference/pointer to a derived class
    
    std::unique_ptr<Numeric> sumOperation(const Numeric& second ) override
    {
        try
        {
            const IntNumeric& secondInt = dynamic_cast<const IntNumeric&>(second);
            return std::make_unique<IntNumeric>(intValue + secondInt.intValue);
        }
        catch (const std::bad_cast& e)
        {
            std::cerr << "Type mismatch: " << e.what() << std::endl;
            return nullptr; // Or handle differently
        }
        catch (const std::bad_alloc& e)
        {
            std::cerr << "Memory allocation failed: " << e.what() << std::endl;
            return nullptr; // Handle memory failure
        }
        
    }
    std::unique_ptr<Numeric> subtractOperation(const Numeric& second) override
    {
        try
        {
            const IntNumeric& secondInt = dynamic_cast<const IntNumeric&>(second);
            return std::make_unique<IntNumeric>(intValue - secondInt.intValue);
        }
        catch (const std::bad_cast& e)
        {
            std::cerr << "Type mismatch: " << e.what() << std::endl;
            return nullptr;
        }
        catch (const std::bad_alloc& e)
        {
            std::cerr << "Memory allocation failed: " << e.what() << std::endl;
            return nullptr;
        }
    }
    

    std::unique_ptr<Numeric> multiplyOperation(const Numeric& second) override
    {
        try
        {
            const IntNumeric& secondInt = dynamic_cast<const IntNumeric&>(second);
            return std::make_unique<IntNumeric>(intValue * secondInt.intValue);
        }
        catch (const std::bad_cast& e)
        {
            throw std::runtime_error("multiplyOperation: Type mismatch, expected IntNumeric.");
        }
        catch (const std::bad_alloc& e)
        {
            throw std::runtime_error("multiplyOperation: Memory allocation failed.");
        }
    }
    
    std::unique_ptr<Numeric> divideOperation(const Numeric& second) override
    {
        try
        {
            const IntNumeric& secondInt = dynamic_cast<const IntNumeric&>(second);
    
            if (secondInt.intValue == 0)
            {
                throw std::runtime_error("divideOperation: Division by zero is not allowed.");
            }
    
            return std::make_unique<IntNumeric>(intValue / secondInt.intValue);
        }
        catch (const std::bad_cast& e)
        {
            throw std::runtime_error("divideOperation: Type mismatch, expected IntNumeric.");
        }
        catch (const std::bad_alloc& e)
        {
            throw std::runtime_error("divideOperation: Memory allocation failed.");
        }
    }
    bool lessThanOperation(const Numeric& second ) override
    {
        const IntNumeric& secondInt= dynamic_cast<const IntNumeric&>(second);
        return (intValue < secondInt.intValue );
    }
    bool greaterThanOperation(const Numeric& second ) override
    {
        const IntNumeric& secondInt= dynamic_cast<const IntNumeric&>(second);
        return (intValue >  secondInt.intValue);
    }
    bool equalOperation(const Numeric& second ) override
    {
        const IntNumeric& secondInt= dynamic_cast<const IntNumeric&>(second);
        return (intValue ==  secondInt.intValue);
    }

    std::string toString() const override
    {
        return std::to_string(intValue); // from std library 
    }

    ~IntNumeric()
    {
        #if DEBUG == 1
        std::cout << "IntNumeric Class destructor with value: " << intValue << std::endl;
        #endif // DEBUG
    }
};

//secondly, derived class with template parameter for floating and double
/*We could use concepts (modern c++23) here to limit this class to
only using float and double with using: std::is_floating_point<T>::value*/
template <class T>
concept FloatingPoint = std::is_floating_point_v<T>;

template <FloatingPoint T>
class FloatNumeric : public Numeric
{
    public:
        T floatValue;

    FloatNumeric(T float_val): floatValue(float_val)
    {
        #if DEBUG == 1
        std::cout << "FloatNumeric class constructor is called with value: " << floatValue << std::endl;
        #endif // DEBUG
    }

    std::unique_ptr<Numeric> sumOperation(const Numeric& second) override
    {
        try
        {
            const FloatNumeric<T>& secondFloat = dynamic_cast<const FloatNumeric<T>&>(second);
            return std::make_unique<FloatNumeric<T>>(floatValue + secondFloat.floatValue);
        }
        catch (const std::bad_cast& e)
        {
            throw std::runtime_error("sumOperation: Type mismatch, expected FloatNumeric<T>.");
        }
        catch (const std::bad_alloc& e)
        {
            throw std::runtime_error("sumOperation: Memory allocation failed.");
        }
    }

    std::unique_ptr<Numeric> subtractOperation(const Numeric& second) override
    {
        try
        {
            const FloatNumeric<T>& secondFloat = dynamic_cast<const FloatNumeric<T>&>(second);
            return std::make_unique<FloatNumeric<T>>(floatValue - secondFloat.floatValue);
        }
        catch (const std::bad_cast& e)
        {
            throw std::runtime_error("subtractOperation: Type mismatch, expected FloatNumeric<T>.");
        }
        catch (const std::bad_alloc& e)
        {
            throw std::runtime_error("subtractOperation: Memory allocation failed.");
        }
    }

    std::unique_ptr<Numeric> multiplyOperation(const Numeric& second) override
    {
        try
        {
            const FloatNumeric<T>& secondFloat = dynamic_cast<const FloatNumeric<T>&>(second);
            return std::make_unique<FloatNumeric<T>>(floatValue * secondFloat.floatValue);
        }
        catch (const std::bad_cast& e)
        {
            throw std::runtime_error("multiplyOperation: Type mismatch, expected FloatNumeric<T>.");
        }
        catch (const std::bad_alloc& e)
        {
            throw std::runtime_error("multiplyOperation: Memory allocation failed.");
        }
    }

    std::unique_ptr<Numeric> divideOperation(const Numeric& second) override
    {
        try
        {
            const FloatNumeric<T>& secondFloat = dynamic_cast<const FloatNumeric<T>&>(second);

            if (secondFloat.floatValue == 0)
            {
                throw std::runtime_error("divideOperation: Division by zero is not allowed.");
            }

            return std::make_unique<FloatNumeric<T>>(floatValue / secondFloat.floatValue);
        }
        catch (const std::bad_cast& e)
        {
            throw std::runtime_error("divideOperation: Type mismatch, expected FloatNumeric<T>.");
        }
        catch (const std::bad_alloc& e)
        {
            throw std::runtime_error("divideOperation: Memory allocation failed.");
        }
    }


    bool lessThanOperation(const Numeric& second ) override
    {
        const FloatNumeric<T>& secondFloat= dynamic_cast<const FloatNumeric<T>&>(second);
        return (floatValue < secondFloat.floatValue );
    }

    bool greaterThanOperation(const Numeric& second ) override
    {
        const FloatNumeric<T>& secondFloat= dynamic_cast<const FloatNumeric<T>&>(second);
        return (floatValue >  secondFloat.floatValue);
    }

    bool equalOperation(const Numeric& second ) override
    {
        const FloatNumeric<T>& secondFloat= dynamic_cast<const FloatNumeric<T>&>(second);
        return (floatValue ==  secondFloat.floatValue);
    }

    std::string toString() const override
    {
        return std::to_string(floatValue); // from std library 
    }

    ~FloatNumeric()
    {
        #if DEBUG == 1
        std::cout << "FloatNumeric Class destructor with value: " << floatValue << std::endl;
        #endif // DEBUG
    }
    
};

template <typename T>
class ComplexNumeric : public Numeric
{

    public:
    std::complex<T> complexNum;

    ComplexNumeric(std::complex<T> num)
    : complexNum(num) 
    {
        #if DEBUG == 1
        std::cout<<"ComplexNumeric Constructor is calledwith value: " << complexNum << std::endl;
        #endif // DEBUG
    }

    std::unique_ptr<Numeric> sumOperation(const Numeric& second) override
    {
        try
        {
            const ComplexNumeric<T>& complexValue = dynamic_cast<const ComplexNumeric<T>&>(second);
            std::complex<T> result = complexNum + complexValue.complexNum;
            return std::make_unique<ComplexNumeric<T>>(result);
        }
        catch (const std::bad_cast& e)
        {
            throw std::runtime_error("sumOperation: Type mismatch, expected ComplexNumeric<T>.");
        }
        catch (const std::bad_alloc& e)
        {
            throw std::runtime_error("sumOperation: Memory allocation failed.");
        }
    }
    
    std::unique_ptr<Numeric> subtractOperation(const Numeric& second) override
    {
        try
        {
            const ComplexNumeric<T>& complexValue = dynamic_cast<const ComplexNumeric<T>&>(second);
            std::complex<T> result = complexNum - complexValue.complexNum;
            return std::make_unique<ComplexNumeric<T>>(result);
        }
        catch (const std::bad_cast& e)
        {
            throw std::runtime_error("subtractOperation: Type mismatch, expected ComplexNumeric<T>.");
        }
        catch (const std::bad_alloc& e)
        {
            throw std::runtime_error("subtractOperation: Memory allocation failed.");
        }
    }
    
    std::unique_ptr<Numeric> multiplyOperation(const Numeric& second) override
    {
        try
        {
            const ComplexNumeric<T>& complexValue = dynamic_cast<const ComplexNumeric<T>&>(second);
            std::complex<T> result = complexNum * complexValue.complexNum;
            return std::make_unique<ComplexNumeric<T>>(result);
        }
        catch (const std::bad_cast& e)
        {
            throw std::runtime_error("multiplyOperation: Type mismatch, expected ComplexNumeric<T>.");
        }
        catch (const std::bad_alloc& e)
        {
            throw std::runtime_error("multiplyOperation: Memory allocation failed.");
        }
    }
    
    std::unique_ptr<Numeric> divideOperation(const Numeric& second) override
    {
        try
        {
            const ComplexNumeric<T>& complexValue = dynamic_cast<const ComplexNumeric<T>&>(second);
    
            if (std::abs(complexValue.complexNum) == 0)
            {
                throw std::runtime_error("divideOperation: Division by zero is not allowed.");
            }
    
            std::complex<T> result = complexNum / complexValue.complexNum;
            return std::make_unique<ComplexNumeric<T>>(result);
        }
        catch (const std::bad_cast& e)
        {
            throw std::runtime_error("divideOperation: Type mismatch, expected ComplexNumeric<T>.");
        }
        catch (const std::bad_alloc& e)
        {
            throw std::runtime_error("divideOperation: Memory allocation failed.");
        }
    }
    
    /*I will compare using Lexicographical comparison*/

    /*I could use std::lexicographical_compare as it is generally used for
    *comparing sequences (like arrays, vectors, or strings),
    *not individual numbers. Since complex numbers have only two components
    *(real and imaginary), a direct comparison using simple if statements is
    * clearer and more efficient*/
    bool lessThanOperation(const Numeric& second ) override
    {
        const ComplexNumeric& complexValue =  dynamic_cast<const ComplexNumeric<T>&>(second);
        if (complexNum.real() == complexValue.complexNum.real())
            return complexNum.imag() < complexValue.complexNum.imag();
        return complexNum.real() < complexValue.complexNum.real();

    }
    bool greaterThanOperation(const Numeric& second ) override
    {
        const ComplexNumeric& complexValue =  dynamic_cast<const ComplexNumeric<T>&>(second);
        if (complexNum.real() == complexValue.complexNum.real())
            return complexNum.imag() > complexValue.complexNum.imag();
        return complexNum.real() > complexValue.complexNum.real();

    }
    bool equalOperation(const Numeric& second ) override
    {
        const ComplexNumeric& complexValue =  dynamic_cast< const ComplexNumeric<T>&>(second);
        return (complexNum == complexValue.complexNum);
    }

    std::string toString() const override
    { 
        return "(" + std::to_string(complexNum.real()) + " + " + 
        std::to_string(complexNum.imag()) + "i)";
    }

    ~ComplexNumeric()
    {
        #if DEBUG == 1
        std::cout << "ComplexNumeric Class destructor with Real value: " << complexNum.real()
        << " And for Img value: "<< complexNum.imag() <<std::endl;
        #endif // DEBUG
    }

};

template <class T>
concept charTemp = std::is_same_v<T, char> || std::is_same_v<T, wchar_t> || std::is_same_v<T, char16_t> || std::is_same_v<T, char32_t>;

template <charTemp T>
class charNumeric : public Numeric
{
    public:
    T charValue;

    charNumeric(T char_val): charValue(char_val)
    {
        #if DEBUG == 1
        std::cout << "CharNumeric class constructor is calledwith value: " << charValue << std::endl;
        #endif // DEBUG
    }
    
    std::unique_ptr<Numeric> sumOperation(const Numeric& second) override
    {
        try
        {
            const charNumeric<T>& secondChar = dynamic_cast<const charNumeric<T>&>(second);
            return std::make_unique<charNumeric<T>>(toascii(charValue + secondChar.charValue));
        }
        catch (const std::bad_cast& e)
        {
            throw std::runtime_error("sumOperation: Type mismatch, expected charNumeric<T>.");
        }
        catch (const std::bad_alloc& e)
        {
            throw std::runtime_error("sumOperation: Memory allocation failed.");
        }
    }

    std::unique_ptr<Numeric> subtractOperation(const Numeric& second) override
    {
        try
        {
            const charNumeric<T>& secondChar = dynamic_cast<const charNumeric<T>&>(second);
            return std::make_unique<charNumeric<T>>(toascii(charValue - secondChar.charValue));
        }
        catch (const std::bad_cast& e)
        {
            throw std::runtime_error("subtractOperation: Type mismatch, expected charNumeric<T>.");
        }
        catch (const std::bad_alloc& e)
        {
            throw std::runtime_error("subtractOperation: Memory allocation failed.");
        }
    }

    std::unique_ptr<Numeric> multiplyOperation(const Numeric& second) override
    {
        throw std::runtime_error("multiplyOperation: Operation not supported for characters.");
    }

    std::unique_ptr<Numeric> divideOperation(const Numeric& second) override
    {
        throw std::runtime_error("divideOperation: Operation not supported for characters.");
    }

    
    bool lessThanOperation(const Numeric& second ) override
    {
        const charNumeric<T>& secondChar= dynamic_cast<charNumeric<T>&>(second);
        return (charValue < secondChar.charValue );
    }
    bool greaterThanOperation(const Numeric& second ) override
    {
        const charNumeric<T>& secondChar= dynamic_cast<charNumeric<T>&>(second);
        return (charValue >  secondChar.charValue);
    }
    bool equalOperation(const Numeric& second ) override
    {
        const charNumeric<T>& secondChar= dynamic_cast<charNumeric<T>&>(second);
        return (charValue ==  secondChar.charValue);
    }
    std::string toString() const override
    {
        return std::string(1, charValue);
    }
    ~charNumeric()
    {
        #if DEBUG == 1
        std::cout << "CharNumeric Class destructor with value: " << charValue << std::endl;
        #endif // DEBUG
    }
};

// Factory function implementation
template <typename T>
std::unique_ptr<Numeric> Numeric::create(T value) {
    if constexpr (std::is_integral_v<T>) {
        return std::make_unique<IntNumeric>(value);
    } else if constexpr (std::is_floating_point_v<T>) {
        return std::make_unique<FloatNumeric<T>>(value);
    } else if constexpr (std::is_same_v<T, std::complex<float>> || std::is_same_v<T, std::complex<double>>) {
        return std::make_unique<ComplexNumeric<typename T::value_type>>(value);
    } else if constexpr (std::is_same_v<T, char> || std::is_same_v<T, wchar_t> || std::is_same_v<T, char16_t> || std::is_same_v<T, char32_t>) {
        return std::make_unique<charNumeric<T>>(value);
    } else {
        throw std::runtime_error("Unsupported type");
    }
}


int main() {
    try {
        // Separate vectors for each numeric type
        std::vector<std::unique_ptr<Numeric>> intNumbers;
        std::vector<std::unique_ptr<Numeric>> floatNumbers;
        std::vector<std::unique_ptr<Numeric>> doubleNumbers;
        std::vector<std::unique_ptr<Numeric>> complexNumbers;
        std::vector<std::unique_ptr<Numeric>> charNumbers;

        // Populate intNumbers vector
        intNumbers.push_back(Numeric::create(5));
        intNumbers.push_back(Numeric::create(10));
        intNumbers.push_back(Numeric::create(2));
        intNumbers.push_back(Numeric::create(-7)); // Edge case

        // Populate floatNumbers vector
        floatNumbers.push_back(Numeric::create(3.14f));
        floatNumbers.push_back(Numeric::create(2.71f));
        floatNumbers.push_back(Numeric::create(1.41f));
        floatNumbers.push_back(Numeric::create(-0.99f)); // Edge case

        // Populate doubleNumbers vector
        doubleNumbers.push_back(Numeric::create(9.81));
        doubleNumbers.push_back(Numeric::create(1.618));
        doubleNumbers.push_back(Numeric::create(2.9979));
        doubleNumbers.push_back(Numeric::create(-3.5)); // Edge case

        // Populate complexNumbers vector
        complexNumbers.push_back(Numeric::create(std::complex<double>(1, 2)));
        complexNumbers.push_back(Numeric::create(std::complex<double>(3, 4)));
        complexNumbers.push_back(Numeric::create(std::complex<double>(0, -1)));
        complexNumbers.push_back(Numeric::create(std::complex<double>(-2, -3))); // Edge case

        // Populate charNumbers vector
        charNumbers.push_back(Numeric::create('A'));
        charNumbers.push_back(Numeric::create('C'));
        charNumbers.push_back(Numeric::create('B'));
        charNumbers.push_back(Numeric::create('Z')); // Edge case

        // Function to perform addition and sorting
        auto processNumbers = [](auto &numbers, const std::string &type) {
            std::cout << "\n" << type << " Addition Results:\n";
            for (size_t i = 0; i < numbers.size() - 1; ++i) {
                try {
                    std::unique_ptr<Numeric> result = numbers[i]->sumOperation(*numbers[i + 1]);
                    std::cout << numbers[i]->toString() << " + " << numbers[i + 1]->toString()
                            << " = " << result->toString() << std::endl;
                } catch (const std::exception &e) {
                    std::cerr << "Exception caught during addition: " << e.what() << std::endl;
                }
            }
            std::sort(numbers.begin(), numbers.end(), [](const auto &a, const auto &b) {
                return a->lessThanOperation(*b);
            });
            std::cout << "\nSorted " << type << " Values:\n";
            for (const auto &num : numbers) {
                std::cout << num->toString() << " ";
            }
            std::cout << "\n";
        };

        // Process each type
        processNumbers(intNumbers, "Integer");
        processNumbers(floatNumbers, "Float");
        processNumbers(doubleNumbers, "Double");
        processNumbers(complexNumbers, "Complex");
        processNumbers(charNumbers, "Char");

        // **Exception Cases**
        try {
            // **Char Overflow Case**
            charNumbers.push_back(Numeric::create(CHAR_MAX));
            charNumbers.push_back(Numeric::create(CHAR_MAX));

            // Check for overflow before performing addition
            int sumCheck = static_cast<int>(CHAR_MAX) + static_cast<int>(CHAR_MAX);
            if (sumCheck > std::numeric_limits<unsigned char>::max()) {
                throw std::overflow_error("Char addition overflow detected.");
            }

            auto charOverflow = charNumbers[0]->sumOperation(*charNumbers[1]); 
            if (charOverflow) {
                std::cout << "Unexpected char addition result: " << charOverflow->toString() << std::endl;
            }
        } catch (const std::overflow_error &e) {
            std::cerr << "Exception caught: Char addition overflow -> " << e.what() << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Exception caught: General char addition error -> " << e.what() << std::endl;
        }

        try {
            // **Division by Zero Case**
            std::unique_ptr<Numeric> zero = Numeric::create(0);
            std::unique_ptr<Numeric> number = Numeric::create(10);
            auto result = number->divideOperation(*zero);
            std::cout << "Division result: " << result->toString() << std::endl;
        } catch (const std::runtime_error &e) {
            std::cerr << "Exception caught: Division by zero -> " << e.what() << std::endl;
        }

        try {
            // **Type Mismatch Case**
            auto typeMismatchResult = intNumbers[0]->sumOperation(*complexNumbers[0]); 
            std::cout << "Unexpected operation result: " << typeMismatchResult->toString() << std::endl;
        } catch (const std::bad_cast &e) {
            std::cerr << "Exception caught: Type mismatch -> " << e.what() << std::endl;
        }

    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "An unknown error occurred." << std::endl;
    }

    return 0;
}
