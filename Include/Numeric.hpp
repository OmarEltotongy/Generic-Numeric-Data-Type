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

template <charTemp T>
class charNumeric : public Numeric
{
    public:
    T charValue;

    charNumeric(T char_val)
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

#endif // __NUMERIC_HPP__