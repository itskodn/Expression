#include <gtest/gtest.h>

#include "MathExpr.hpp"

TEST(ExpressionParsingTest, SimpleAddition) {
    auto expr = parseExpression<Real>("5 + 7");
    EXPECT_EQ(expr.eval({}), 12);
}

TEST(ExpressionParsingTest, VariableEvaluation) {
    auto expr = parseExpression<Real>("y + 4");
    std::map<std::string, Real> vars = {{"y", 6}};
    EXPECT_EQ(expr.eval(vars), 10);
}

TEST(ExpressionParsingTest, MultiplicationAndDivision) {
    auto expr = parseExpression<Real>("3 * y / 6");
    std::map<std::string, Real> vars = {{"y", 12}};
    EXPECT_EQ(expr.eval(vars), 6);
}

TEST(ExpressionParsingTest, PowerFunction) {
    auto expr = parseExpression<Real>("y ^ 3");
    std::map<std::string, Real> vars = {{"y", 4}};
    EXPECT_EQ(expr.eval(vars), 64);
}

TEST(ExpressionParsingTest, SinFunction) {
    auto expr = parseExpression<Real>("sin(y)");
    std::map<std::string, Real> vars = {{"y", M_PI / 2}};
    EXPECT_NEAR(expr.eval(vars), 1, 1e-9);
}

TEST(SymbolicDifferentiationTest, PowerFunction) {
    auto expr = parseExpression<Real>("y ^ 3");
    auto diff_expr = expr.diff("y");
    std::map<std::string, Real> vars = {{"y", 3}};
    EXPECT_EQ(diff_expr.eval(vars), 27);
}

TEST(SymbolicDifferentiationTest, SinFunction) {
    auto expr = parseExpression<Real>("sin(y)");
    auto diff_expr = expr.diff("y");
    std::map<std::string, Real> vars = {{"y", M_PI}};
    EXPECT_NEAR(diff_expr.eval(vars), -1, 1e-9);
}

TEST(SymbolicDifferentiationTest, LnFunction) {
    auto expr = parseExpression<Real>("ln(y)");
    auto diff_expr = expr.diff("y");
    std::map<std::string, Real> vars = {{"y", 2}};
    EXPECT_NEAR(diff_expr.eval(vars), 0.5, 1e-9);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}