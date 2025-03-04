#include "MathExpr.hpp"
#include <iostream>

int main() {
    MathExpression<double> a(2.5);
    MathExpression<double> b(3.0);
    MathExpression<double> c = a + b;

    std::cout << "Expression: " << c << std::endl;
    std::cout << "Result: " << c.evaluate({}) << std::endl;

    return 0;
}

// #include <cassert>
// void testMathExpression() {
//     MathExpression expr1(2);
//     MathExpression expr2(3);
//     MathExpression expr3 = expr1 + expr2;

//     std::cout << "Testing addition:" << std::endl;
//     std::cout << expr1.toString() << " + " << expr2.toString() << " = " << expr3.toString() << std::endl;
//     std::cout << "Expected result: 5, Actual result: " << expr3.evaluate({}) << std::endl;
//     assert(expr3.evaluate({}) == 5);  
//     std::cout << "Addition test passed!\n" << std::endl;

//     MathExpression expr4 = expr1 ^ expr2;

//     std::cout << "Testing power:" << std::endl;
//     std::cout << expr1.toString() << " ^ " << expr2.toString() << " = " << expr4.toString() << std::endl;
//     std::cout << "Expected result: 8, Actual result: " << expr4.evaluate({}) << std::endl;
//     assert(expr4.evaluate({}) == 8);  
//     std::cout << "Power test passed!\n" << std::endl;

//     MathExpression expr5 = sin(expr1);

//     std::cout << "Testing sine function:" << std::endl;
//     std::cout << "sin(" << expr1.toString() << ") = " << expr5.toString() << std::endl;
//     std::cout << "Expected result: " << std::sin(2) << ", Actual result: " << expr5.evaluate({}) << std::endl;
//     assert(expr5.evaluate({}) == std::sin(2)); 
//     std::cout << "Sine test passed!\n" << std::endl;

//     MathExpression expr6 = expr1 * expr2;

//     std::cout << "Testing multiplication:" << std::endl;
//     std::cout << expr1.toString() << " * " << expr2.toString() << " = " << expr6.toString() << std::endl;
//     std::cout << "Expected result: 6, Actual result: " << expr6.evaluate({}) << std::endl;
//     assert(expr6.evaluate({}) == 6);  
//     std::cout << "Multiplication test passed!\n" << std::endl;

//     MathExpression expr7 = expr2 / expr1;

//     std::cout << "Testing division:" << std::endl;
//     std::cout << expr2.toString() << " / " << expr1.toString() << " = " << expr7.toString() << std::endl;
//     std::cout << "Expected result: 1.5, Actual result: " << expr7.evaluate({}) << std::endl;
//     assert(expr7.evaluate({}) == 1.5);  
//     std::cout << "Division test passed!\n" << std::endl;

//     MathExpression expr8 = cos(expr1);

//     std::cout << "Testing cosine function:" << std::endl;
//     std::cout << "cos(" << expr1.toString() << ") = " << expr8.toString() << std::endl;
//     std::cout << "Expected result: " << std::cos(2) << ", Actual result: " << expr8.evaluate({}) << std::endl;
//     assert(expr8.evaluate({}) == std::cos(2));  
//     std::cout << "Cosine test passed!\n" << std::endl;

//     MathExpression expr9 = exp(expr1);

//     std::cout << "Testing exponential function:" << std::endl;
//     std::cout << "exp(" << expr1.toString() << ") = " << expr9.toString() << std::endl;
//     std::cout << "Expected result: " << std::exp(2) << ", Actual result: " << expr9.evaluate({}) << std::endl;
//     assert(expr9.evaluate({}) == std::exp(2));  
//     std::cout << "Exponential test passed!\n" << std::endl;

    
//     MathExpression expr10 = log(expr1);

//     std::cout << "Testing natural logarithm function:" << std::endl;
//     std::cout << "log(" << expr1.toString() << ") = " << expr10.toString() << std::endl;
//     std::cout << "Expected result: " << std::log(2) << ", Actual result: " << expr10.evaluate({}) << std::endl;
//     assert(expr10.evaluate({}) == std::log(2)); 
//     std::cout << "Logarithm test passed!\n" << std::endl;

//     std::cout << "All tests passed!" << std::endl;
// }

// int main() {
//     testMathExpression();
//     return 0;
// }