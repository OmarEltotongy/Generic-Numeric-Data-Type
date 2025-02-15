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
    
std::unique_ptr<Numeric> IntNumeric::sumOperation(const Numeric& second )
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

std::unique_ptr<Numeric> IntNumeric::subtractOperation(const Numeric& second) 
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
    
std::unique_ptr<Numeric> IntNumeric::multiplyOperation(const Numeric& second) 
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
    
std::unique_ptr<Numeric> IntNumeric::divideOperation(const Numeric& second) 
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

bool IntNumeric::lessThanOperation(const Numeric& second ) 
{
    const IntNumeric& secondInt= dynamic_cast<const IntNumeric&>(second);
    return (intValue < secondInt.intValue );
}
bool IntNumeric::greaterThanOperation(const Numeric& second ) 

{
    const IntNumeric& secondInt= dynamic_cast<const IntNumeric&>(second);
    return (intValue >  secondInt.intValue);
}
    
bool IntNumeric::equalOperation(const Numeric& second ) 
{
    const IntNumeric& secondInt= dynamic_cast<const IntNumeric&>(second);
    return (intValue ==  secondInt.intValue);
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


