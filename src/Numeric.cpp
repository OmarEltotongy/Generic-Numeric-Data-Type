#include "../include/Numeric.hpp"


/************************ Numeric Class ********************************/

Numeric::Numeric()
{
    #if DEBUG == 1
    std::cout << "Numeric Class constructor is called" << std::endl;
    #endif // DEBUG

}


Numeric::~Numeric()
    {
        #if DEBUG == 1
        std::cout << "Numeric Class destructor is called" << std::endl;
        #endif // DEBUG
    }



/* When to Use dynamic_cast?
When working with polymorphic base classes (Numeric has virtual functions).
When you need to convert a base class reference/pointer to a derived class safely.
When there’s a possibility that the object is not the expected type.*/

/**Templates do not allow me to use runtime polymorphism as easily.
 * So, if i want to have different types (e.g., integer vs. floating-point vs. complex)
 *  in the same container, a base class with polymorphism (like my current design) becomes more useful */




 /************************ IntNumeric Class ********************************/

 //firstly a derived class with polymorphism

IntNumeric::IntNumeric(int value) : intValue(value)
    {
        #if DEBUG == 1
        std::cout << "IntNumeric Class constructor with value: " << value << std::endl;
        #endif // DEBUG
    }
    
    std::unique_ptr<Numeric> IntNumeric::convertTo(const std::type_info& targetType) const {
        if (targetType == typeid(FloatNumeric<float>)) {
            return std::unique_ptr<Numeric>(std::make_unique<FloatNumeric<float>>(static_cast<float>(intValue)));
        } else if (targetType == typeid(FloatNumeric<double>)) {
            return std::unique_ptr<Numeric>(std::make_unique<FloatNumeric<double>>(static_cast<double>(intValue)));
        } else if (targetType == typeid(ComplexNumeric<float>)) {
            return std::unique_ptr<Numeric>(std::make_unique<ComplexNumeric<float>>(std::complex<float>(intValue, 0)));
        } else if (targetType == typeid(ComplexNumeric<double>)) {
            return std::unique_ptr<Numeric>(std::make_unique<ComplexNumeric<double>>(std::complex<double>(intValue, 0)));
        } else if (targetType == typeid(IntNumeric)) {
            return std::unique_ptr<Numeric>(std::make_unique<IntNumeric>(intValue));
        } else {
            throw std::runtime_error("Unsupported conversion");
        }
    }

    std::unique_ptr<Numeric> IntNumeric::sumOperation(const Numeric& second) {
        try {
            const IntNumeric& secondInt = dynamic_cast<const IntNumeric&>(second);
            return std::make_unique<IntNumeric>(intValue + secondInt.intValue);
        } catch (const std::bad_cast&) {
            // If the types don't match, convert the current object to the type of the second operand
            if (typeid(second) == typeid(FloatNumeric<float>)) {
                const FloatNumeric<float>& secondFloat = dynamic_cast<const FloatNumeric<float>&>(second);
                return std::make_unique<FloatNumeric<float>>(static_cast<float>(intValue) + secondFloat.floatValue);
            } else if (typeid(second) == typeid(FloatNumeric<double>)) {
                const FloatNumeric<double>& secondDouble = dynamic_cast<const FloatNumeric<double>&>(second);
                return std::make_unique<FloatNumeric<double>>(static_cast<double>(intValue) + secondDouble.floatValue);
            } else if (typeid(second) == typeid(ComplexNumeric<float>)) {
                const ComplexNumeric<float>& secondComplex = dynamic_cast<const ComplexNumeric<float>&>(second);
                return std::make_unique<ComplexNumeric<float>>(std::complex<float>(intValue, 0) + secondComplex.complexNum);
            } else if (typeid(second) == typeid(ComplexNumeric<double>)) {
                const ComplexNumeric<double>& secondComplex = dynamic_cast<const ComplexNumeric<double>&>(second);
                return std::make_unique<ComplexNumeric<double>>(std::complex<double>(intValue, 0) + secondComplex.complexNum);
            } else {
                // If the type is not supported, throw an exception
                throw std::runtime_error("Unsupported type for addition.");
            }
        } catch (const std::bad_alloc& e) {
            throw std::runtime_error("sumOperation: Memory allocation failed.");
        }
    }
std::unique_ptr<Numeric> IntNumeric::subtractOperation(const Numeric& second) {
    try {
        const IntNumeric& secondInt = dynamic_cast<const IntNumeric&>(second);
        return std::make_unique<IntNumeric>(intValue - secondInt.intValue);
    } catch (const std::bad_cast&) {
        // If the types don't match, convert the second operand to IntNumeric
        auto converted = second.convertTo(typeid(IntNumeric));
        const IntNumeric& secondInt = dynamic_cast<const IntNumeric&>(*converted);
        return std::make_unique<IntNumeric>(intValue - secondInt.intValue);
    } catch (const std::bad_alloc& e) {
        throw std::runtime_error("subtractOperation: Memory allocation failed.");
    }
}

std::unique_ptr<Numeric> IntNumeric::multiplyOperation(const Numeric& second) {
    try {
        const IntNumeric& secondInt = dynamic_cast<const IntNumeric&>(second);
        return std::make_unique<IntNumeric>(intValue * secondInt.intValue);
    } catch (const std::bad_cast&) {
        // If the types don't match, convert the second operand to IntNumeric
        auto converted = second.convertTo(typeid(IntNumeric));
        const IntNumeric& secondInt = dynamic_cast<const IntNumeric&>(*converted);
        return std::make_unique<IntNumeric>(intValue * secondInt.intValue);
    } catch (const std::bad_alloc& e) {
        throw std::runtime_error("multiplyOperation: Memory allocation failed.");
    }
}

std::unique_ptr<Numeric> IntNumeric::divideOperation(const Numeric& second) {
    try {
        const IntNumeric& secondInt = dynamic_cast<const IntNumeric&>(second);

        if (secondInt.intValue == 0) {
            throw std::runtime_error("divideOperation: Division by zero is not allowed.");
        }

        return std::make_unique<IntNumeric>(intValue / secondInt.intValue);
    } catch (const std::bad_cast&) {
        // If the types don't match, convert the second operand to IntNumeric
        auto converted = second.convertTo(typeid(IntNumeric));
        const IntNumeric& secondInt = dynamic_cast<const IntNumeric&>(*converted);

        if (secondInt.intValue == 0) {
            throw std::runtime_error("divideOperation: Division by zero is not allowed.");
        }

        return std::make_unique<IntNumeric>(intValue / secondInt.intValue);
    } catch (const std::bad_alloc& e) {
        throw std::runtime_error("divideOperation: Memory allocation failed.");
    }
}

bool IntNumeric::lessThanOperation(const Numeric& second) {
    try {
        const IntNumeric& secondInt = dynamic_cast<const IntNumeric&>(second);
        return (intValue < secondInt.intValue);
    } catch (const std::bad_cast&) {
        // If the types don't match, convert the second operand to IntNumeric
        auto converted = second.convertTo(typeid(IntNumeric));
        const IntNumeric& secondInt = dynamic_cast<const IntNumeric&>(*converted);
        return (intValue < secondInt.intValue);
    }
}

bool IntNumeric::greaterThanOperation(const Numeric& second) {
    try {
        const IntNumeric& secondInt = dynamic_cast<const IntNumeric&>(second);
        return (intValue > secondInt.intValue);
    } catch (const std::bad_cast&) {
        // If the types don't match, convert the second operand to IntNumeric
        auto converted = second.convertTo(typeid(IntNumeric));
        const IntNumeric& secondInt = dynamic_cast<const IntNumeric&>(*converted);
        return (intValue > secondInt.intValue);
    }
}

bool IntNumeric::equalOperation(const Numeric& second) {
    try {
        const IntNumeric& secondInt = dynamic_cast<const IntNumeric&>(second);
        return (intValue == secondInt.intValue);
    } catch (const std::bad_cast&) {
        // If the types don't match, convert the second operand to IntNumeric
        auto converted = second.convertTo(typeid(IntNumeric));
        const IntNumeric& secondInt = dynamic_cast<const IntNumeric&>(*converted);
        return (intValue == secondInt.intValue);
    }
}
std::string IntNumeric::toString() const
{
    return std::to_string(intValue); // from std library 
}

IntNumeric::~IntNumeric()
{
    #if DEBUG == 1
    std::cout << "IntNumeric Class destructor with value: " << intValue << std::endl;
    #endif // DEBUG
}


/************************ FloatNumeric Class ********************************/

//secondly, derived class with template parameter for floating and double
/*We could use concepts (modern c++23) here to limit this class to
only using float and double with using: std::is_floating_point<T>::value*/


/**
 * Template classes and their methods must be fully defined in the header file
 * (or made visible to the compiler in some other way) because the compiler
 *  needs to generate the code for each instantiation of the template.
 */
/**So we will declare this function in the .hpp file insted of .cpp */


 /************************ ComplexNumeric Class ********************************/

 /**Also this class will be defined in the .hpp as it is a template class */


 /************************ CharNumeric Class ********************************/

 /**Also this class will be defined in the .hpp as it is a template class */


