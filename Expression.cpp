#include "MathExpr.hpp"
#include <cmath>

MathExpression MathExpression::pow(const MathExpression& exponent) const {
    return MathExpression(std::make_shared<BinaryOpNode>(OperationType::Power, root->clone(), exponent.cloneRoot()));
}

ValueType power(ValueType base, ValueType exponent) {
    if (exponent == 0) return 1;  
    ValueType result = base;
    for (ValueType i = 1; i < exponent; ++i) {
        result *= base;
    }
    return result;
} 

std::ostream& operator<<(std::ostream& out, const MathExpression& expr) {
    out << expr.toString();  
    return out;
}

std::string operationTypeToString(OperationType type) {
    switch (type) {
        case OperationType::Number: return "Num";
        case OperationType::Symbol: return "Sym";
        case OperationType::Add: return "+";
        case OperationType::Subtract: return "-";
        case OperationType::Multiply: return "*";
        case OperationType::Divide: return "/";
        case OperationType::Power: return "^";
        case OperationType::Sine: return "sin";
        case OperationType::Cosine: return "cos";
        case OperationType::Log: return "log";
        case OperationType::Exp: return "exp";
        default: return "Unknown";
    }
}

NumberNode::NumberNode(ValueType val) : value(val) {}

ValueType NumberNode::evaluate(const std::map<std::string, ValueType>& vars) const {
    return value;
}

std::string NumberNode::toString() const {
    return std::to_string(value);
}

std::shared_ptr<MathNode> NumberNode::clone() const {
    return std::make_shared<NumberNode>(value);
}

SymbolNode::SymbolNode(const std::string& sym) : symbol(sym) {}

ValueType SymbolNode::evaluate(const std::map<std::string, ValueType>& vars) const {
    auto it = vars.find(symbol);
    if (it == vars.end()) throw std::runtime_error("Symbol not found: " + symbol);
    return it->second;
}

std::string SymbolNode::toString() const {
    return symbol;
}

std::shared_ptr<MathNode> SymbolNode::clone() const {
    return std::make_shared<SymbolNode>(symbol);
}

BinaryOpNode::BinaryOpNode(OperationType op, std::shared_ptr<MathNode> left, std::shared_ptr<MathNode> right)
    : op(op), left(left), right(right) {}

ValueType BinaryOpNode::evaluate(const std::map<std::string, ValueType>& vars) const {
    ValueType leftVal = left->evaluate(vars);
    ValueType rightVal = right->evaluate(vars);
    switch (op) {
        case OperationType::Add: return leftVal + rightVal;
        case OperationType::Subtract: return leftVal - rightVal;
        case OperationType::Multiply: return leftVal * rightVal;
        case OperationType::Divide: return leftVal / rightVal;
        case OperationType::Power: return power(leftVal, rightVal); 
        default: throw std::runtime_error("Unknown binary operation");
    }
}

std::string BinaryOpNode::toString() const {
    return left->toString() + operationTypeToString(op) + right->toString();
}

std::shared_ptr<MathNode> BinaryOpNode::clone() const {
    return std::make_shared<BinaryOpNode>(op, left->clone(), right->clone());
}

FunctionNode::FunctionNode(OperationType func, std::shared_ptr<MathNode> arg) : func(func), arg(arg) {}

ValueType FunctionNode::evaluate(const std::map<std::string, ValueType>& vars) const {
    ValueType argVal = arg->evaluate(vars);
    switch (func) {
        case OperationType::Sine: return std::sin(argVal);
        case OperationType::Cosine: return std::cos(argVal);
        case OperationType::Exp: return std::exp(argVal);
        case OperationType::Log: return std::log(argVal);
        default: throw std::runtime_error("Unknown function");
    }
}

std::string FunctionNode::toString() const {
    return operationTypeToString(func) + "(" + arg->toString() + ")";
}

std::shared_ptr<MathNode> FunctionNode::clone() const {
    return std::make_shared<FunctionNode>(func, arg->clone());
}

MathExpression::MathExpression(ValueType val) : root(std::make_shared<NumberNode>(val)) {}

MathExpression::MathExpression(const std::string& sym) : root(std::make_shared<SymbolNode>(sym)) {}

MathExpression::MathExpression(std::shared_ptr<MathNode> node) : root(node) {}

MathExpression::MathExpression(const MathExpression& other) : root(other.root->clone()) {}

MathExpression::MathExpression(MathExpression&& other) noexcept : root(std::move(other.root)) {}

MathExpression& MathExpression::operator=(const MathExpression& other) {
    root = other.root->clone();
    return *this;
}

MathExpression& MathExpression::operator=(MathExpression&& other) noexcept {
    root = std::move(other.root);
    return *this;
}

MathExpression MathExpression::operator+(const MathExpression& other) const {
    return MathExpression(std::make_shared<BinaryOpNode>(OperationType::Add, root->clone(), other.root->clone()));
}

MathExpression MathExpression::operator-(const MathExpression& other) const {
    return MathExpression(std::make_shared<BinaryOpNode>(OperationType::Subtract, root->clone(), other.root->clone()));
}

MathExpression MathExpression::operator*(const MathExpression& other) const {
    return MathExpression(std::make_shared<BinaryOpNode>(OperationType::Multiply, root->clone(), other.root->clone()));
}

MathExpression MathExpression::operator/(const MathExpression& other) const {
    return MathExpression(std::make_shared<BinaryOpNode>(OperationType::Divide, root->clone(), other.root->clone()));
}

MathExpression MathExpression::operator^(const MathExpression& other) const {
    return MathExpression(std::make_shared<BinaryOpNode>(OperationType::Power, root->clone(), other.root->clone()));
}

ValueType MathExpression::evaluate(const std::map<std::string, ValueType>& vars) const {
    return root->evaluate(vars);
}

std::string MathExpression::toString() const {
    return root->toString();
}

std::shared_ptr<MathNode> MathExpression::cloneRoot() const {
    return root->clone();
}

MathExpression sin(const MathExpression& expr) {
    return MathExpression(std::make_shared<FunctionNode>(OperationType::Sine, expr.cloneRoot()));
}

MathExpression cos(const MathExpression& expr) {
    return MathExpression(std::make_shared<FunctionNode>(OperationType::Cosine, expr.cloneRoot()));
}

MathExpression exp(const MathExpression& expr) {
    return MathExpression(std::make_shared<FunctionNode>(OperationType::Exp, expr.cloneRoot()));
}

MathExpression log(const MathExpression& expr) {
    return MathExpression(std::make_shared<FunctionNode>(OperationType::Log, expr.cloneRoot()));
}