#include "../include/Numeric.hpp"


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

    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "An unknown error occurred." << std::endl;
    }

    try {
        std::unique_ptr<Numeric> intNum = Numeric::create(10);
        std::unique_ptr<Numeric> floatNum = Numeric::create(5.5f);
        std::unique_ptr<Numeric> doubleNum = Numeric::create(3.14159);
        std::unique_ptr<Numeric> complexNum = Numeric::create(std::complex<double>(1, 2));
        std::unique_ptr<Numeric> charNum = Numeric::create('A'); // ASCII 65
    
        // Integer + Float
        auto intFloatResult = intNum->sumOperation(*floatNum);
        std::cout << "10 + 5.5 = " << intFloatResult->toString() << std::endl;
    
        // Float + Double
        auto floatDoubleResult = floatNum->sumOperation(*doubleNum);
        std::cout << "5.5 + 3.14159 = " << floatDoubleResult->toString() << std::endl;
    
        // Double + Complex
        auto doubleComplexResult = doubleNum->sumOperation(*complexNum);
        std::cout << "3.14159 + (1 + 2i) = " << doubleComplexResult->toString() << std::endl;
    
        // Char + Int (Expect 'A' (65) + 10 = 75, which is 'K' in ASCII)
        auto charIntResult = charNum->sumOperation(*intNum);
        std::cout << "'A' + 10 = " << charIntResult->toString() << std::endl;
    
    } catch (const std::exception &e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
    
    return 0;
}
