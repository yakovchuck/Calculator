#include <iostream>
#include <string>
#include "CArithmeticExpressionsCalculator.h"

namespace {
    const char* exitCommand = "exit";
}

int main(int argc, char* argv[])
{
    CArithmeticExpressionsCalculator calculator;
    
    std::string expr;
    
    while (expr != exitCommand) {
        std::cout << "Enter expression:" << std::endl;
        std::getline(std::cin, expr);
        if (expr != exitCommand) {
            try {
                double result = calculator.calculate(expr);
                std::cout << "Result: " << result << std::endl;
            } catch (std::string err) {
                std::cout << err << std::endl;
            }
        }
    }
    
    return 0;
}
