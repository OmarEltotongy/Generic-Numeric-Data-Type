#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <concepts>
#include <type_traits>  // For std::is_floating_point
#include <complex>

#include <algorithm>

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
    private:
        int intValue;
    public:
    IntNumeric(int value) : intValue(value)
    {
        #if DEBUG == 1
        std::cout << "IntNumeric Class constructor with value: " << value << std::endl;
        #endif // DEBUG
    }

    // Helper function to cast a base class reference/pointer to a derived class
    
    std::unique_ptr<Numeric> sumOperation(const Numeric& second ) override
    {
        const IntNumeric& secondInt= dynamic_cast<const IntNumeric&>(second);
        return std::make_unique<IntNumeric>(intValue + secondInt.intValue);
        
    }
    std::unique_ptr<Numeric> subtractOperation(const Numeric& second ) override
    {
        const IntNumeric& secondInt= dynamic_cast<const IntNumeric&>(second);
        return std::make_unique<IntNumeric>(intValue - secondInt.intValue);
    }
    std::unique_ptr<Numeric> multiplyOperation(const Numeric& second ) override
    {
        const IntNumeric& secondInt= dynamic_cast<const IntNumeric&>(second);
        return std::make_unique<IntNumeric>(intValue * secondInt.intValue);
    }
    std::unique_ptr<Numeric> divideOperation(const Numeric& second ) override
    {
        const IntNumeric& secondInt= dynamic_cast<const IntNumeric&>(second);
        if(secondInt.intValue == 0)
        {
            throw std::runtime_error("Division by zero is not allowed.");
        }
        return std::make_unique<IntNumeric>(intValue / secondInt.intValue);
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
    private:
    T floatValue;
    public:
    
    FloatNumeric(T float_val): floatValue(float_val)
    {
        #if DEBUG == 1
        std::cout << "FloatNumeric class constructor is called with value: " << floatValue << std::endl;
        #endif // DEBUG
    }

    std::unique_ptr<Numeric> sumOperation(const Numeric& second ) override
    {
        const FloatNumeric<T>& secondFloat= dynamic_cast<const FloatNumeric<T>&>(second);
        return std::make_unique<FloatNumeric<T>>(floatValue + secondFloat.floatValue);
        
    }
    std::unique_ptr<Numeric> subtractOperation(const Numeric& second ) override
    {
        const FloatNumeric<T>& secondFloat= dynamic_cast<const FloatNumeric<T>&>(second);
        return std::make_unique<FloatNumeric<T>>(floatValue - secondFloat.floatValue);
    }
    std::unique_ptr<Numeric> multiplyOperation(const Numeric& second ) override
    {
        const FloatNumeric<T>& secondFloat= dynamic_cast<const FloatNumeric<T>&>(second);
        return std::make_unique<FloatNumeric<T>>(floatValue * secondFloat.floatValue);
    }
    std::unique_ptr<Numeric> divideOperation(const Numeric& second ) override
    {
        const FloatNumeric<T>& secondFloat= dynamic_cast<const FloatNumeric<T>&>(second);
        if(secondFloat.floatValue == 0)
        {
            throw std::runtime_error("Division by zero is not allowed.");
        }
        return std::make_unique<FloatNumeric<T>>(floatValue / secondFloat.floatValue);
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
    private:
    std::complex<T> complexNum;

    public:
    ComplexNumeric(std::complex<T> num)
    : complexNum(num) 
    {
        #if DEBUG == 1
        std::cout<<"ComplexNumeric Constructor is calledwith value: " << complexNum << std::endl;
        #endif // DEBUG
    }

    std::unique_ptr<Numeric> sumOperation(const Numeric& second)override
    {
        const ComplexNumeric<T>& complexValue =  dynamic_cast<const ComplexNumeric<T>&>(second);

        std::complex<T> result(complexNum + complexValue.complexNum);
        return std::make_unique<ComplexNumeric<T>>(result);
    }
    
    std::unique_ptr<Numeric> subtractOperation(const Numeric& second)
    {
        const ComplexNumeric<T>& complexValue =  dynamic_cast<const ComplexNumeric<T>&>(second);
        
        std::complex<T> result (complexNum - complexValue.complexNum);
        return std::make_unique<ComplexNumeric<T>>(result);
    }

    std::unique_ptr<Numeric> multiplyOperation(const Numeric& second)override
    {
        const ComplexNumeric<T>& complexValue =  dynamic_cast<const ComplexNumeric<T>&>(second);

        std::complex<T> result(complexNum * complexValue.complexNum);
        return std::make_unique<ComplexNumeric<T>>(result);
    }

    std::unique_ptr<Numeric> divideOperation(const Numeric& second)override
    {
        const ComplexNumeric<T>& complexValue =  dynamic_cast<const ComplexNumeric<T>&>(second);

        std::complex<T> result(complexNum / complexValue.complexNum);

        return std::make_unique<ComplexNumeric<T>>(result);
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
    private:
    T charValue;
    public:
    
    charNumeric(T char_val): charValue(char_val)
    {
        #if DEBUG == 1
        std::cout << "CharNumeric class constructor is calledwith value: " << charValue << std::endl;
        #endif // DEBUG
    }
    
    std::unique_ptr<Numeric> sumOperation(const Numeric& second ) override
    {
        const charNumeric<T>& secondChar= dynamic_cast<charNumeric<T>&>(second);
        return toascii(std::make_unique<charNumeric<T>>(charValue + secondChar.charValue));
    }

    std::unique_ptr<Numeric> subtractOperation(const Numeric& second ) override
    {
        const charNumeric<T>& secondChar= dynamic_cast<charNumeric<T>&>(second);
        return toascii(std::make_unique<charNumeric<T>>(charValue - secondChar.charValue));
    }
    std::unique_ptr<Numeric> multiplyOperation(const Numeric& second ) override
    {
        throw std::runtime_error(" multiply operation is not supported for characters");
        return nullptr;
    } 
    std::unique_ptr<Numeric> divideOperation(const Numeric& second ) override
    {
        throw std::runtime_error(" multiply operation is not supported for characters");
        return nullptr;
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
        // Create a vector to store Numeric objects
        std::vector<std::unique_ptr<Numeric>> numericVector;

        // Add different numeric types to the vector
        numericVector.push_back(Numeric::create(5));                     // IntNumeric
        numericVector.push_back(Numeric::create(10));                    // IntNumeric
        numericVector.push_back(Numeric::create(0));                     // IntNumeric
        numericVector.push_back(Numeric::create(-5));                    // IntNumeric
        numericVector.push_back(Numeric::create(5.5f));                  // FloatNumeric<float>
        numericVector.push_back(Numeric::create(10.5f));                 // FloatNumeric<float>
        numericVector.push_back(Numeric::create(0.0f));                  // FloatNumeric<float>
        numericVector.push_back(Numeric::create(-5.5f));                 // FloatNumeric<float>
        numericVector.push_back(Numeric::create(5.5));                   // FloatNumeric<double>
        numericVector.push_back(Numeric::create(10.5));                  // FloatNumeric<double>
        numericVector.push_back(Numeric::create(0.0));                   // FloatNumeric<double>
        numericVector.push_back(Numeric::create(-5.5));                  // FloatNumeric<double>
        numericVector.push_back(Numeric::create(std::complex<float>(3.0f, 4.0f))); // ComplexNumeric<float>
        numericVector.push_back(Numeric::create(std::complex<float>(5.0f, 6.0f))); // ComplexNumeric<float>
        numericVector.push_back(Numeric::create(std::complex<float>(0.0f, 0.0f))); // ComplexNumeric<float>
        numericVector.push_back(Numeric::create('a'));                   // CharNumeric<char>

        // Perform operations on the vector elements
        std::cout << "Vector Contents:" << std::endl;
        for (const auto& num : numericVector) {
            std::cout << num->toString() << std::endl;
        }

        // Perform arithmetic operations on selected elements
        std::cout << "Arithmetic Operations:" << std::endl;
        for (size_t i = 0; i < numericVector.size() - 1; ++i) {
            std::cout << "Sum: " << numericVector[i]->sumOperation(*numericVector[i + 1])->toString() << std::endl;
            std::cout << "Subtraction: " << numericVector[i]->subtractOperation(*numericVector[i + 1])->toString() << std::endl;
            std::cout << "Multiplication: " << numericVector[i]->multiplyOperation(*numericVector[i + 1])->toString() << std::endl;
            try {
                std::cout << "Division: " << numericVector[i]->divideOperation(*numericVector[i + 1])->toString() << std::endl;
            } catch (const std::exception& e) {
                std::cout << "Division exception: " << e.what() << std::endl;
            }
        }

        // Perform comparison operations
        std::cout << "Comparison Operations:" << std::endl;
        for (size_t i = 0; i < numericVector.size() - 1; ++i) {
            std::cout << "Is element " << i << " less than element " << i + 1 << "? "
                    << (numericVector[i]->lessThanOperation(*numericVector[i + 1]) ? "Yes" : "No") << std::endl;
            std::cout << "Is element " << i << " greater than element " << i + 1 << "? "
                    << (numericVector[i]->greaterThanOperation(*numericVector[i + 1]) ? "Yes" : "No") << std::endl;
            std::cout << "Is element " << i << " equal to element " << i + 1 << "? "
                    << (numericVector[i]->equalOperation(*numericVector[i + 1]) ? "Yes" : "No") << std::endl;
        }

        // Sorting the vector based on lessThanOperation
        std::sort(numericVector.begin(), numericVector.end(),
                [](const std::unique_ptr<Numeric>& a, const std::unique_ptr<Numeric>& b) {
                    return a->lessThanOperation(*b);
                });

        std::cout << "Sorted Vector Contents:" << std::endl;
        for (const auto& num : numericVector) {
            std::cout << num->toString() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
