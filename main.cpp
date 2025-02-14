#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <concepts>
#include <type_traits>  // For std::is_floating_point
#include <complex>


class Numeric
{
    public:
    Numeric()
    {
        std::cout << "Numeric Class constructor is called" << std::endl;
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
        std::cout << "Numeric Class destructor is called" << std::endl;
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
        std::cout << "IntNumeric Class constructor with value: " << value << std::endl;
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
        std::cout << "IntNumeric Class destructor with value: " << intValue << std::endl;
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
        std::cout << "FloatNumeric class constructor is called with value: " << floatValue << std::endl;
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
        std::cout << "FloatNumeric Class destructor with value: " << floatValue << std::endl;
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
        std::cout<<"ComplexNumeric Constructor is calledwith value: " << complexNum << std::endl;
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
        std::cout << "ComplexNumeric Class destructor with Real value: " << complexNum.real()
        << " And for Img value: "<< complexNum.imag() <<std::endl;
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
        std::cout << "CharNumeric class constructor is calledwith value: " << charValue << std::endl;
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
        std::cout << "CharNumeric Class destructor with value: " << charValue << std::endl;
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
        // Test IntNumeric
        auto num1 = Numeric::create(5);        // IntNumeric
        auto num2 = Numeric::create(10);       // IntNumeric
        auto numZero = Numeric::create(0);     // IntNumeric
        auto numNegative = Numeric::create(-5); // IntNumeric

        // Sum
        std::cout << "Sum (Int): " << num1->sumOperation(*num2)->toString() << std::endl;

        // Subtraction
        std::cout << "Subtraction (Int): " << num1->subtractOperation(*num2)->toString() << std::endl;

        // Multiplication
        std::cout << "Multiplication (Int): " << num1->multiplyOperation(*num2)->toString() << std::endl;

        // Division
        std::cout << "Division (Int): " << num1->divideOperation(*num2)->toString() << std::endl;

        // Division by zero (should throw an exception)
        try {
            std::cout << "Division by zero (Int): " << num1->divideOperation(*numZero)->toString() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Division by zero exception (Int): " << e.what() << std::endl;
        }

        // Comparison
        std::cout << "Is num1 less than num2? " << (num1->lessThanOperation(*num2) ? "Yes" : "No") << std::endl;
        std::cout << "Is num1 greater than num2? " << (num1->greaterThanOperation(*num2) ? "Yes" : "No") << std::endl;
        std::cout << "Is num1 equal to num2? " << (num1->equalOperation(*num2) ? "Yes" : "No") << std::endl;

        // Test FloatNumeric with float
        auto num3 = Numeric::create(5.5f);     // FloatNumeric<float>
        auto num4 = Numeric::create(10.5f);    // FloatNumeric<float>
        auto numFloatZero = Numeric::create(0.0f); // FloatNumeric<float>
        auto numFloatNegative = Numeric::create(-5.5f); // FloatNumeric<float>

        // Sum
        std::cout << "Sum (Float): " << num3->sumOperation(*num4)->toString() << std::endl;

        // Subtraction
        std::cout << "Subtraction (Float): " << num3->subtractOperation(*num4)->toString() << std::endl;

        // Multiplication
        std::cout << "Multiplication (Float): " << num3->multiplyOperation(*num4)->toString() << std::endl;

        // Division
        std::cout << "Division (Float): " << num3->divideOperation(*num4)->toString() << std::endl;

        // Division by zero (should throw an exception)
        try {
            std::cout << "Division by zero (Float): " << num3->divideOperation(*numFloatZero)->toString() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Division by zero exception (Float): " << e.what() << std::endl;
        }

        // Comparison
        std::cout << "Is num3 less than num4? " << (num3->lessThanOperation(*num4) ? "Yes" : "No") << std::endl;
        std::cout << "Is num3 greater than num4? " << (num3->greaterThanOperation(*num4) ? "Yes" : "No") << std::endl;
        std::cout << "Is num3 equal to num4? " << (num3->equalOperation(*num4) ? "Yes" : "No") << std::endl;

        // Test FloatNumeric with double
        auto num5 = Numeric::create(5.5);      // FloatNumeric<double>
        auto num6 = Numeric::create(10.5);     // FloatNumeric<double>
        auto numDoubleZero = Numeric::create(0.0); // FloatNumeric<double>
        auto numDoubleNegative = Numeric::create(-5.5); // FloatNumeric<double>

        // Sum
        std::cout << "Sum (Double): " << num5->sumOperation(*num6)->toString() << std::endl;

        // Subtraction
        std::cout << "Subtraction (Double): " << num5->subtractOperation(*num6)->toString() << std::endl;

        // Multiplication
        std::cout << "Multiplication (Double): " << num5->multiplyOperation(*num6)->toString() << std::endl;

        // Division
        std::cout << "Division (Double): " << num5->divideOperation(*num6)->toString() << std::endl;

        // Division by zero (should throw an exception)
        try {
            std::cout << "Division by zero (Double): " << num5->divideOperation(*numDoubleZero)->toString() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Division by zero exception (Double): " << e.what() << std::endl;
        }

        // Comparison
        std::cout << "Is num5 less than num6? " << (num5->lessThanOperation(*num6) ? "Yes" : "No") << std::endl;
        std::cout << "Is num5 greater than num6? " << (num5->greaterThanOperation(*num6) ? "Yes" : "No") << std::endl;
        std::cout << "Is num5 equal to num6? " << (num5->equalOperation(*num6) ? "Yes" : "No") << std::endl;

        // Test ComplexNumeric
        auto num7 = Numeric::create(std::complex<float>(3.0f, 4.0f)); // ComplexNumeric<float>
        auto num8 = Numeric::create(std::complex<float>(5.0f, 6.0f)); // ComplexNumeric<float>
        auto numComplexZero = Numeric::create(std::complex<float>(0.0f, 0.0f)); // ComplexNumeric<float>

        // Sum
        std::cout << "Sum (Complex): " << num7->sumOperation(*num8)->toString() << std::endl;

        // Subtraction
        std::cout << "Subtraction (Complex): " << num7->subtractOperation(*num8)->toString() << std::endl;

        // Multiplication
        std::cout << "Multiplication (Complex): " << num7->multiplyOperation(*num8)->toString() << std::endl;

        // Division
        std::cout << "Division (Complex): " << num7->divideOperation(*num8)->toString() << std::endl;

        // Division by zero (should throw an exception)
        try {
            std::cout << "Division by zero (Complex): " << num7->divideOperation(*numComplexZero)->toString() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Division by zero exception (Complex): " << e.what() << std::endl;
        }

        // Comparison
        std::cout << "Is num7 less than num8? " << (num7->lessThanOperation(*num8) ? "Yes" : "No") << std::endl;
        std::cout << "Is num7 greater than num8? " << (num7->greaterThanOperation(*num8) ? "Yes" : "No") << std::endl;
        std::cout << "Is num7 equal to num8? " << (num7->equalOperation(*num8) ? "Yes" : "No") << std::endl;

        // Test charNumeric
        auto num9 = Numeric::create('a'); // charNumeric<char>
        auto num10 = Numeric::create('b'); // charNumeric<char>

        // Sum
        std::cout << "Sum (Char): " << num9->sumOperation(*num10)->toString() << std::endl;

        // Subtraction
        std::cout << "Subtraction (Char): " << num9->subtractOperation(*num10)->toString() << std::endl;

        // Multiplication (should throw an exception)
        try {
            std::cout << "Multiplication (Char): " << num9->multiplyOperation(*num10)->toString() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Multiplication exception (Char): " << e.what() << std::endl;
        }

        // Division (should throw an exception)
        try {
            std::cout << "Division (Char): " << num9->divideOperation(*num10)->toString() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Division exception (Char): " << e.what() << std::endl;
        }

        // Comparison
        std::cout << "Is num9 less than num10? " << (num9->lessThanOperation(*num10) ? "Yes" : "No") << std::endl;
        std::cout << "Is num9 greater than num10? " << (num9->greaterThanOperation(*num10) ? "Yes" : "No") << std::endl;
        std::cout << "Is num9 equal to num10? " << (num9->equalOperation(*num10) ? "Yes" : "No") << std::endl;

        // Test unsupported type (should throw an exception)
        try {
            auto numUnsupported = Numeric::create("UnsupportedType"); // Unsupported type
        } catch (const std::exception& e) {
            std::cout << "Unsupported type exception: " << e.what() << std::endl;
        }

    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
}