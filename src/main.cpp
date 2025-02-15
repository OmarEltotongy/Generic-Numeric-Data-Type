#include "../include/Numeric.hpp"


int main() {
    // Create a vector to hold different numeric types
    std::vector<std::unique_ptr<Numeric>> vec;

    // Push different numeric types into the vector
    vec.push_back(Numeric::create(10));              // IntNumeric
    vec.push_back(Numeric::create(5.5f));            // FloatNumeric<float>
    vec.push_back(Numeric::create(3.14159));         // FloatNumeric<double>
    vec.push_back(Numeric::create(std::complex<float>(1, 2))); // ComplexNumeric<float>
    vec.push_back(Numeric::create('A'));             // charNumeric<char>

    // Display initial values in the vector
    std::cout << "Initial values in the vector:\n";
    for (const auto& num : vec) {
        std::cout << num->toString() << "\n";
    }

    // Perform operations on each element with every other element
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << "\nOperations for element " << i << " (" << vec[i]->toString() << "):\n";

        for (size_t j = 0; j < vec.size(); ++j) {
            if (i == j) continue; // Skip operations with itself

            std::cout << "\nWith element " << j << " (" << vec[j]->toString() << "):\n";

            // Addition
            try {
                auto result_add = vec[i]->sumOperation(*vec[j]);
                std::cout << "Addition: " << vec[i]->toString() << " + " << vec[j]->toString() << " = " << result_add->toString() << "\n";
            } catch (const std::exception& e) {
                std::cerr << "Error during addition: " << e.what() << "\n";
            }

            // Subtraction
            try {
                auto result_sub = vec[i]->subtractOperation(*vec[j]);
                std::cout << "Subtraction: " << vec[i]->toString() << " - " << vec[j]->toString() << " = " << result_sub->toString() << "\n";
            } catch (const std::exception& e) {
                std::cerr << "Error during subtraction: " << e.what() << "\n";
            }

            // Multiplication
            try {
                auto result_mul = vec[i]->multiplyOperation(*vec[j]);
                std::cout << "Multiplication: " << vec[i]->toString() << " * " << vec[j]->toString() << " = " << result_mul->toString() << "\n";
            } catch (const std::exception& e) {
                std::cerr << "Error during multiplication: " << e.what() << "\n";
            }

            // Division
            try {
                auto result_div = vec[i]->divideOperation(*vec[j]);
                std::cout << "Division: " << vec[i]->toString() << " / " << vec[j]->toString() << " = " << result_div->toString() << "\n";
            } catch (const std::exception& e) {
                std::cerr << "Error during division: " << e.what() << "\n";
            }

            // Comparison: Less Than
            try {
                bool is_less = vec[i]->lessThanOperation(*vec[j]);
                std::cout << "Comparison (Less Than): " << vec[i]->toString() << " < " << vec[j]->toString() << " = " << (is_less ? "true" : "false") << "\n";
            } catch (const std::exception& e) {
                std::cerr << "Error during less-than comparison: " << e.what() << "\n";
            }

            // Comparison: Greater Than
            try {
                bool is_greater = vec[i]->greaterThanOperation(*vec[j]);
                std::cout << "Comparison (Greater Than): " << vec[i]->toString() << " > " << vec[j]->toString() << " = " << (is_greater ? "true" : "false") << "\n";
            } catch (const std::exception& e) {
                std::cerr << "Error during greater-than comparison: " << e.what() << "\n";
            }

            // Comparison: Equal
            try {
                bool is_equal = vec[i]->equalOperation(*vec[j]);
                std::cout << "Comparison (Equal): " << vec[i]->toString() << " == " << vec[j]->toString() << " = " << (is_equal ? "true" : "false") << "\n";
            } catch (const std::exception& e) {
                std::cerr << "Error during equality comparison: " << e.what() << "\n";
            }
        }
    }

    // Sort the vector in ascending order using lessThanOperation
    std::sort(vec.begin(), vec.end(), [](const std::unique_ptr<Numeric>& a, const std::unique_ptr<Numeric>& b) {
        return a->lessThanOperation(*b);
    });

    // Display sorted values in the vector
    std::cout << "\nSorted values in the vector (ascending order):\n";
    for (const auto& num : vec) {
        std::cout << num->toString() << "\n";
    }

    return 0;
}

