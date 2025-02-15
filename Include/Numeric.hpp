#ifndef __NUMERIC_HPP__
#define __NUMERIC_HPP__

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

// Forward declarations
class Numeric;
class IntNumeric;

template <class T>
concept FloatingPoint = std::is_floating_point_v<T>;

template <FloatingPoint T>
class FloatNumeric;

template <typename T>
class ComplexNumeric;

template <class T>
concept charTemp = std::is_same_v<T, char> || std::is_same_v<T, wchar_t> || std::is_same_v<T, char16_t> || std::is_same_v<T, char32_t>;

template <charTemp T>
class charNumeric;


/**
 * Template classes and their methods must be fully defined in the header file
 * (or made visible to the compiler in some other way) because the compiler
 *  needs to generate the code for each instantiation of the template.
 */

class Numeric
{
    public:
    Numeric();
    template <typename T>
    static std::unique_ptr<Numeric> create(T value)
    {
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

    // Convert the current Numeric object to a specific type
    virtual std::unique_ptr<Numeric> convertTo(const std::type_info& targetType) const = 0;
    virtual std::unique_ptr<Numeric> sumOperation(const Numeric& second ) = 0;
    virtual std::unique_ptr<Numeric> subtractOperation( const Numeric& second ) = 0;
    virtual std::unique_ptr<Numeric> multiplyOperation(const Numeric& second ) = 0;
    virtual std::unique_ptr<Numeric> divideOperation(const Numeric& second ) = 0;
    virtual bool lessThanOperation(const Numeric& second ) =0;
    virtual bool greaterThanOperation(const Numeric& second ) =0;
    virtual bool equalOperation(const Numeric& second ) =0;
    virtual std::string toString() const = 0;
    virtual ~Numeric();

};


class IntNumeric : public Numeric
{
    public:
        int intValue;

    IntNumeric(int value);
    
    std::unique_ptr<Numeric> convertTo(const std::type_info& targetType) const override;
    std::unique_ptr<Numeric> sumOperation(const Numeric& second ) override;
    std::unique_ptr<Numeric> subtractOperation(const Numeric& second) override;
    std::unique_ptr<Numeric> multiplyOperation(const Numeric& second) override;    
    std::unique_ptr<Numeric> divideOperation(const Numeric& second) override;
    bool lessThanOperation(const Numeric& second ) override;
    bool greaterThanOperation(const Numeric& second ) override;
    bool equalOperation(const Numeric& second ) override;
    std::string toString() const;
    ~IntNumeric();
};

//secondly, derived class with template parameter for floating and double
/*We could use concepts (modern c++23) here to limit this class to
only using float and double with using: std::is_floating_point<T>::value*/


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

    std::unique_ptr<Numeric> convertTo(const std::type_info& targetType) const {
        if (targetType == typeid(FloatNumeric<float>)) {
            return std::unique_ptr<Numeric>(std::make_unique<FloatNumeric<float>>(static_cast<float>(floatValue)));
        } else if (targetType == typeid(FloatNumeric<double>)) {
            return std::unique_ptr<Numeric>(std::make_unique<FloatNumeric<double>>(static_cast<double>(floatValue)));
        } else if (targetType == typeid(ComplexNumeric<float>)) {
            return std::unique_ptr<Numeric>(std::make_unique<ComplexNumeric<float>>(std::complex<float>(floatValue, 0)));
        } else if (targetType == typeid(ComplexNumeric<double>)) {
            return std::unique_ptr<Numeric>(std::make_unique<ComplexNumeric<double>>(std::complex<double>(floatValue, 0)));
        } else if (targetType == typeid(IntNumeric)) {
            return std::unique_ptr<Numeric>(std::make_unique<IntNumeric>(static_cast<int>(floatValue)));
        } else {
            throw std::runtime_error("Unsupported conversion");
        }
    }
    std::unique_ptr<Numeric> sumOperation(const Numeric& second) override {
        try {
            const FloatNumeric<T>& secondFloat = dynamic_cast<const FloatNumeric<T>&>(second);
            return std::make_unique<FloatNumeric<T>>(floatValue + secondFloat.floatValue);
        } catch (const std::bad_cast&) {
            // If the types don't match, check if the second operand is a complex number
            if (typeid(second) == typeid(ComplexNumeric<T>)) {
                const ComplexNumeric<T>& secondComplex = dynamic_cast<const ComplexNumeric<T>&>(second);
                return std::make_unique<ComplexNumeric<T>>(std::complex<T>(floatValue + secondComplex.complexNum.real(), secondComplex.complexNum.imag()));
            } else {
                // Convert the second operand to the current type and perform the operation
                auto converted = second.convertTo(typeid(*this));
                const FloatNumeric<T>& secondFloat = dynamic_cast<const FloatNumeric<T>&>(*converted);
                return std::make_unique<FloatNumeric<T>>(floatValue + secondFloat.floatValue);
            }
        } catch (const std::bad_alloc& e) {
            throw std::runtime_error("sumOperation: Memory allocation failed.");
        }
    }
    std::unique_ptr<Numeric> subtractOperation(const Numeric& second) {
        try {
            const FloatNumeric<T>& secondFloat = dynamic_cast<const FloatNumeric<T>&>(second);
            return std::make_unique<FloatNumeric<T>>(floatValue - secondFloat.floatValue);
        } catch (const std::bad_cast&) {
            // If the types don't match, convert the second operand to FloatNumeric<T>
            auto converted = second.convertTo(typeid(FloatNumeric<T>));
            const FloatNumeric<T>& secondFloat = dynamic_cast<const FloatNumeric<T>&>(*converted);
            return std::make_unique<FloatNumeric<T>>(floatValue - secondFloat.floatValue);
        } catch (const std::bad_alloc& e) {
            throw std::runtime_error("subtractOperation: Memory allocation failed.");
        }
    }
    
    std::unique_ptr<Numeric> multiplyOperation(const Numeric& second) {
        try {
            const FloatNumeric<T>& secondFloat = dynamic_cast<const FloatNumeric<T>&>(second);
            return std::make_unique<FloatNumeric<T>>(floatValue * secondFloat.floatValue);
        } catch (const std::bad_cast&) {
            // If the types don't match, convert the second operand to FloatNumeric<T>
            auto converted = second.convertTo(typeid(FloatNumeric<T>));
            const FloatNumeric<T>& secondFloat = dynamic_cast<const FloatNumeric<T>&>(*converted);
            return std::make_unique<FloatNumeric<T>>(floatValue * secondFloat.floatValue);
        } catch (const std::bad_alloc& e) {
            throw std::runtime_error("multiplyOperation: Memory allocation failed.");
        }
    }
    
    std::unique_ptr<Numeric> divideOperation(const Numeric& second) {
        try {
            const FloatNumeric<T>& secondFloat = dynamic_cast<const FloatNumeric<T>&>(second);
    
            if (secondFloat.floatValue == 0) {
                throw std::runtime_error("divideOperation: Division by zero is not allowed.");
            }
    
            return std::make_unique<FloatNumeric<T>>(floatValue / secondFloat.floatValue);
        } catch (const std::bad_cast&) {
            // If the types don't match, convert the second operand to FloatNumeric<T>
            auto converted = second.convertTo(typeid(FloatNumeric<T>));
            const FloatNumeric<T>& secondFloat = dynamic_cast<const FloatNumeric<T>&>(*converted);
    
            if (secondFloat.floatValue == 0) {
                throw std::runtime_error("divideOperation: Division by zero is not allowed.");
            }
    
            return std::make_unique<FloatNumeric<T>>(floatValue / secondFloat.floatValue);
        } catch (const std::bad_alloc& e) {
            throw std::runtime_error("divideOperation: Memory allocation failed.");
        }
    }
    
    bool lessThanOperation(const Numeric& second) {
        try {
            const FloatNumeric<T>& secondFloat = dynamic_cast<const FloatNumeric<T>&>(second);
            return (floatValue < secondFloat.floatValue);
        } catch (const std::bad_cast&) {
            // If the types don't match, convert the second operand to FloatNumeric<T>
            auto converted = second.convertTo(typeid(FloatNumeric<T>));
            const FloatNumeric<T>& secondFloat = dynamic_cast<const FloatNumeric<T>&>(*converted);
            return (floatValue < secondFloat.floatValue);
        }
    }
    
    bool greaterThanOperation(const Numeric& second) {
        try {
            const FloatNumeric<T>& secondFloat = dynamic_cast<const FloatNumeric<T>&>(second);
            return (floatValue > secondFloat.floatValue);
        } catch (const std::bad_cast&) {
            // If the types don't match, convert the second operand to FloatNumeric<T>
            auto converted = second.convertTo(typeid(FloatNumeric<T>));
            const FloatNumeric<T>& secondFloat = dynamic_cast<const FloatNumeric<T>&>(*converted);
            return (floatValue > secondFloat.floatValue);
        }
    }
    
    bool equalOperation(const Numeric& second) {
        try {
            const FloatNumeric<T>& secondFloat = dynamic_cast<const FloatNumeric<T>&>(second);
            return (floatValue == secondFloat.floatValue);
        } catch (const std::bad_cast&) {
            // If the types don't match, convert the second operand to FloatNumeric<T>
            auto converted = second.convertTo(typeid(FloatNumeric<T>));
            const FloatNumeric<T>& secondFloat = dynamic_cast<const FloatNumeric<T>&>(*converted);
            return (floatValue == secondFloat.floatValue);
        }
    }
    
    std::string toString() const {
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

    std::unique_ptr<Numeric> convertTo(const std::type_info& targetType) const {
        if (targetType == typeid(FloatNumeric<float>)) {
            return std::unique_ptr<Numeric>(std::make_unique<FloatNumeric<float>>(static_cast<float>(complexNum.real())));
        } else if (targetType == typeid(FloatNumeric<double>)) {
            return std::unique_ptr<Numeric>(std::make_unique<FloatNumeric<double>>(static_cast<double>(complexNum.real())));
        } else if (targetType == typeid(ComplexNumeric<float>)) {
            return std::unique_ptr<Numeric>(std::make_unique<ComplexNumeric<float>>(std::complex<float>(complexNum.real(), complexNum.imag())));
        } else if (targetType == typeid(ComplexNumeric<double>)) {
            return std::unique_ptr<Numeric>(std::make_unique<ComplexNumeric<double>>(std::complex<double>(complexNum.real(), complexNum.imag())));
        } else if (targetType == typeid(IntNumeric)) {
            return std::unique_ptr<Numeric>(std::make_unique<IntNumeric>(static_cast<int>(complexNum.real())));
        } else {
            throw std::runtime_error("Unsupported conversion");
        }
    }

    std::unique_ptr<Numeric> sumOperation(const Numeric& second) {
        try {
            const ComplexNumeric<T>& complexValue = dynamic_cast<const ComplexNumeric<T>&>(second);
            std::complex<T> result = complexNum + complexValue.complexNum;
            return std::unique_ptr<Numeric>(std::make_unique<ComplexNumeric<T>>(result));
        } catch (const std::bad_cast&) {
            // If the types don't match, convert the second operand to ComplexNumeric<T>
            auto converted = second.convertTo(typeid(ComplexNumeric<T>));
            const ComplexNumeric<T>& complexValue = dynamic_cast<const ComplexNumeric<T>&>(*converted);
            std::complex<T> result = complexNum + complexValue.complexNum;
            return std::unique_ptr<Numeric>(std::make_unique<ComplexNumeric<T>>(result));
        } catch (const std::bad_alloc& e) {
            throw std::runtime_error("sumOperation: Memory allocation failed.");
        }
    }
    
    std::unique_ptr<Numeric> subtractOperation(const Numeric& second) {
        try {
            const ComplexNumeric<T>& complexValue = dynamic_cast<const ComplexNumeric<T>&>(second);
            std::complex<T> result = complexNum - complexValue.complexNum;
            return std::unique_ptr<Numeric>(std::make_unique<ComplexNumeric<T>>(result));
        } catch (const std::bad_cast&) {
            // If the types don't match, convert the second operand to ComplexNumeric<T>
            auto converted = second.convertTo(typeid(*this));
            const ComplexNumeric<T>& complexValue = dynamic_cast<const ComplexNumeric<T>&>(*converted);
            std::complex<T> result = complexNum - complexValue.complexNum;
            return std::unique_ptr<Numeric>(std::make_unique<ComplexNumeric<T>>(result));
        } catch (const std::bad_alloc& e) {
            throw std::runtime_error("subtractOperation: Memory allocation failed.");
        }
    }

    std::unique_ptr<Numeric> multiplyOperation(const Numeric& second) {
        try {
            const ComplexNumeric<T>& complexValue = dynamic_cast<const ComplexNumeric<T>&>(second);
            std::complex<T> result = complexNum * complexValue.complexNum;
            return std::unique_ptr<Numeric>(std::make_unique<ComplexNumeric<T>>(result));
        } catch (const std::bad_cast&) {
            // If the types don't match, convert the second operand to ComplexNumeric<T>
            auto converted = second.convertTo(typeid(*this));
            const ComplexNumeric<T>& complexValue = dynamic_cast<const ComplexNumeric<T>&>(*converted);
            std::complex<T> result = complexNum * complexValue.complexNum;
            return std::unique_ptr<Numeric>(std::make_unique<ComplexNumeric<T>>(result));
        } catch (const std::bad_alloc& e) {
            throw std::runtime_error("multiplyOperation: Memory allocation failed.");
        }
    }

    std::unique_ptr<Numeric> divideOperation(const Numeric& second) {
        try {
            const ComplexNumeric<T>& complexValue = dynamic_cast<const ComplexNumeric<T>&>(second);

            if (std::abs(complexValue.complexNum) == 0) {
                throw std::runtime_error("divideOperation: Division by zero is not allowed.");
            }

            std::complex<T> result = complexNum / complexValue.complexNum;
            return std::unique_ptr<Numeric>(std::make_unique<ComplexNumeric<T>>(result));
        } catch (const std::bad_cast&) {
            // If the types don't match, convert the second operand to ComplexNumeric<T>
            auto converted = second.convertTo(typeid(*this));
            const ComplexNumeric<T>& complexValue = dynamic_cast<const ComplexNumeric<T>&>(*converted);

            if (std::abs(complexValue.complexNum) == 0) {
                throw std::runtime_error("divideOperation: Division by zero is not allowed.");
            }

            std::complex<T> result = complexNum / complexValue.complexNum;
            return std::unique_ptr<Numeric>(std::make_unique<ComplexNumeric<T>>(result));
        } catch (const std::bad_alloc& e) {
            throw std::runtime_error("divideOperation: Memory allocation failed.");
        }
    }
    
    bool lessThanOperation(const Numeric& second) {
        try {
            const ComplexNumeric<T>& complexValue = dynamic_cast<const ComplexNumeric<T>&>(second);
            if (complexNum.real() == complexValue.complexNum.real()) {
                return complexNum.imag() < complexValue.complexNum.imag();
            }
            return complexNum.real() < complexValue.complexNum.real();
        } catch (const std::bad_cast&) {
            // If the types don't match, convert the second operand to ComplexNumeric<T>
            auto converted = second.convertTo(typeid(ComplexNumeric<T>));
            const ComplexNumeric<T>& complexValue = dynamic_cast<const ComplexNumeric<T>&>(*converted);
            if (complexNum.real() == complexValue.complexNum.real()) {
                return complexNum.imag() < complexValue.complexNum.imag();
            }
            return complexNum.real() < complexValue.complexNum.real();
        }
    }
    
    bool greaterThanOperation(const Numeric& second) {
        try {
            const ComplexNumeric<T>& complexValue = dynamic_cast<const ComplexNumeric<T>&>(second);
            if (complexNum.real() == complexValue.complexNum.real()) {
                return complexNum.imag() > complexValue.complexNum.imag();
            }
            return complexNum.real() > complexValue.complexNum.real();
        } catch (const std::bad_cast&) {
            // If the types don't match, convert the second operand to ComplexNumeric<T>
            auto converted = second.convertTo(typeid(ComplexNumeric<T>));
            const ComplexNumeric<T>& complexValue = dynamic_cast<const ComplexNumeric<T>&>(*converted);
            if (complexNum.real() == complexValue.complexNum.real()) {
                return complexNum.imag() > complexValue.complexNum.imag();
            }
            return complexNum.real() > complexValue.complexNum.real();
        }
    }
    
    bool equalOperation(const Numeric& second) {
        try {
            const ComplexNumeric<T>& complexValue = dynamic_cast<const ComplexNumeric<T>&>(second);
            return (complexNum == complexValue.complexNum);
        } catch (const std::bad_cast&) {
            // If the types don't match, convert the second operand to ComplexNumeric<T>
            auto converted = second.convertTo(typeid(ComplexNumeric<T>));
            const ComplexNumeric<T>& complexValue = dynamic_cast<const ComplexNumeric<T>&>(*converted);
            return (complexNum == complexValue.complexNum);
        }
    }
    
    std::string toString() const {
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

template <charTemp T>
class charNumeric : public Numeric {
public:
    T charValue;

    charNumeric(T char_val) : charValue(char_val) {
        #if DEBUG == 1
        std::cout << "CharNumeric class constructor is called with value: " << charValue << std::endl;
        #endif // DEBUG
    }

    std::unique_ptr<Numeric> convertTo(const std::type_info& targetType) const {
        if (targetType == typeid(FloatNumeric<float>)) {
            return std::unique_ptr<Numeric>(std::make_unique<FloatNumeric<float>>(static_cast<float>(static_cast<int>(charValue))));
        } else if (targetType == typeid(FloatNumeric<double>)) {
            return std::unique_ptr<Numeric>(std::make_unique<FloatNumeric<double>>(static_cast<double>(static_cast<int>(charValue))));
        } else if (targetType == typeid(IntNumeric)) {
            return std::unique_ptr<Numeric>(std::make_unique<IntNumeric>(static_cast<int>(charValue)));
        } else {
            throw std::runtime_error("Unsupported conversion");
        }
    }

    std::unique_ptr<Numeric> sumOperation(const Numeric& second) {
        try {
            const charNumeric<T>& secondChar = dynamic_cast<const charNumeric<T>&>(second);
            return std::unique_ptr<Numeric>(std::make_unique<charNumeric<T>>(toascii(charValue + secondChar.charValue)));
        } catch (const std::bad_cast&) {
            // If the types don't match, convert the second operand to charNumeric<T>
            auto converted = second.convertTo(typeid(charNumeric<T>));
            const charNumeric<T>& secondChar = dynamic_cast<const charNumeric<T>&>(*converted);
            return std::unique_ptr<Numeric>(std::make_unique<charNumeric<T>>(toascii(charValue + secondChar.charValue)));
        } catch (const std::bad_alloc& e) {
            throw std::runtime_error("sumOperation: Memory allocation failed.");
        }
    }

    std::unique_ptr<Numeric> subtractOperation(const Numeric& second) {
        try {
            const charNumeric<T>& secondChar = dynamic_cast<const charNumeric<T>&>(second);
            return std::unique_ptr<Numeric>(std::make_unique<charNumeric<T>>(toascii(charValue - secondChar.charValue)));
        } catch (const std::bad_cast&) {
            // If the types don't match, convert the second operand to charNumeric<T>
            auto converted = second.convertTo(typeid(charNumeric<T>));
            const charNumeric<T>& secondChar = dynamic_cast<const charNumeric<T>&>(*converted);
            return std::unique_ptr<Numeric>(std::make_unique<charNumeric<T>>(toascii(charValue - secondChar.charValue)));
        } catch (const std::bad_alloc& e) {
            throw std::runtime_error("subtractOperation: Memory allocation failed.");
        }
    }

    std::unique_ptr<Numeric> multiplyOperation(const Numeric& second) {
        throw std::runtime_error("multiplyOperation: Operation not supported for characters.");
    }

    std::unique_ptr<Numeric> divideOperation(const Numeric& second) {
        throw std::runtime_error("divideOperation: Operation not supported for characters.");
    }

    bool lessThanOperation(const Numeric& second) {
        try {
            const charNumeric<T>& secondChar = dynamic_cast<const charNumeric<T>&>(second);
            return (charValue < secondChar.charValue);
        } catch (const std::bad_cast&) {
            // If the types don't match, convert the second operand to charNumeric<T>
            auto converted = second.convertTo(typeid(charNumeric<T>));
            const charNumeric<T>& secondChar = dynamic_cast<const charNumeric<T>&>(*converted);
            return (charValue < secondChar.charValue);
        }
    }

    bool greaterThanOperation(const Numeric& second) {
        try {
            const charNumeric<T>& secondChar = dynamic_cast<const charNumeric<T>&>(second);
            return (charValue > secondChar.charValue);
        } catch (const std::bad_cast&) {
            // If the types don't match, convert the second operand to charNumeric<T>
            auto converted = second.convertTo(typeid(charNumeric<T>));
            const charNumeric<T>& secondChar = dynamic_cast<const charNumeric<T>&>(*converted);
            return (charValue > secondChar.charValue);
        }
    }

    bool equalOperation(const Numeric& second) {
        try {
            const charNumeric<T>& secondChar = dynamic_cast<const charNumeric<T>&>(second);
            return (charValue == secondChar.charValue);
        } catch (const std::bad_cast&) {
            // If the types don't match, convert the second operand to charNumeric<T>
            auto converted = second.convertTo(typeid(charNumeric<T>));
            const charNumeric<T>& secondChar = dynamic_cast<const charNumeric<T>&>(*converted);
            return (charValue == secondChar.charValue);
        }
    }

    std::string toString() const {
        return std::string(1, charValue);
    }

    ~charNumeric() {
        #if DEBUG == 1
        std::cout << "CharNumeric Class destructor with value: " << charValue << std::endl;
        #endif // DEBUG
    }
};

#endif // __NUMERIC_HPP__