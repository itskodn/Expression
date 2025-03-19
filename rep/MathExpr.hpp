#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <cmath>
#include <complex>
#include <concepts>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <stack>
#include <stdexcept>
#include <type_traits>

using Real = long double;
using Complex = std::complex<Real>;

template <typename T>
concept IsNumeric = std::is_arithmetic_v<T> || std::is_same_v<T, Complex>;

std::string complexToString(const Complex& c) {
    return "(" + std::to_string(c.real()) + "," + std::to_string(c.imag()) +
           ")";
}

enum class NodeType : char {
    Constant,
    Variable,
    Add,
    Subtract,
    Multiply,
    Divide,
    Power,
    Negate,
    Sin,
    Cos,
    Ln,
    Exp
};

inline std::string nodeTypeToString(NodeType type) {
    switch (type) {
        case NodeType::Constant:  return "Const";
        case NodeType::Variable:  return "Var";
        case NodeType::Add:  return "+";
        case NodeType::Subtract:  return "-";
        case NodeType::Multiply:  return "*";
        case NodeType::Divide:  return "/";
        case NodeType::Power:  return "^";
        case NodeType::Negate:  return "-";
        case NodeType::Sin:  return "sin";
        case NodeType::Cos:  return "cos";
        case NodeType::Ln:  return "ln";
        case NodeType::Exp:  return "exp";
        default:  return "Unknown";
    }
}

template <IsNumeric T>
class Node {
   public:
    virtual ~Node() = default;
    virtual T eval(const std::map<std::string, T>& vars) const = 0;
    virtual std::string toString() const = 0;
    virtual std::shared_ptr<Node<T>> copy() const = 0;
    virtual std::shared_ptr<Node<T>> diff(const std::string& var) const = 0;
    virtual NodeType getType() const = 0;
};

template <IsNumeric T>
class ConstantNode : public Node<T> {
    T value;
    NodeType type;

   public:
    explicit ConstantNode(T val) : value(val), type(NodeType::Constant) {}
    T eval(const std::map<std::string, T>& vars) const override {
        return value;
    }
    std::string toString() const override {
        if constexpr (std::is_same_v<T, Complex>)
            return complexToString(value);
        else
            return std::to_string(value);
    }
    std::shared_ptr<Node<T>> copy() const override {
        return std::make_shared<ConstantNode<T>>(value);
    }
    std::shared_ptr<Node<T>> diff(const std::string& var) const override {
        return std::make_shared<ConstantNode<T>>(0);
    }
    NodeType getType() const override { return type; }
    T getValue() const { return value; }
};

template <IsNumeric T>
class VariableNode : public Node<T> {
    std::string name;
    NodeType type;

   public:
    explicit VariableNode(std::string var)
        : name(std::move(var)), type(NodeType::Variable) {}
    T eval(const std::map<std::string, T>& vars) const override {
        if constexpr (std::is_same_v<T, Complex>) {
            if (name == "i") {
                return Complex(0, 1);
            }
        }
        auto it = vars.find(name);
        if (it == vars.end())
            throw std::runtime_error("Variable '" + name + "' not found");
        return it->second;
    }
    std::string toString() const override { return name; }
    std::shared_ptr<Node<T>> copy() const override {
        return std::make_shared<VariableNode<T>>(name);
    }
    std::shared_ptr<Node<T>> diff(const std::string& var) const override {
        return std::make_shared<ConstantNode<T>>(name == var ? 1 : 0);
    }
    NodeType getType() const override { return type; }
};

template <IsNumeric T>
class BinaryOperationNode : public Node<T> {
    NodeType type;
    std::shared_ptr<Node<T>> leftOperand, rightOperand;

   public:
    BinaryOperationNode(NodeType op, std::shared_ptr<Node<T>> left,
                        std::shared_ptr<Node<T>> right)
        : type(op),
          leftOperand(std::move(left)),
          rightOperand(std::move(right)) {}
    T eval(const std::map<std::string, T>& vars) const override {
        T leftVal = leftOperand->eval(vars);
        T rightVal = rightOperand->eval(vars);
        switch (type) {
            case NodeType::Add:
                return leftVal + rightVal;
            case NodeType::Subtract:
                return leftVal - rightVal;
            case NodeType::Multiply:
                return leftVal * rightVal;
            case NodeType::Divide:
                if (rightVal == T(0.)) {
                    throw std::runtime_error("Division by zero");
                }
                return leftVal / rightVal;
            case NodeType::Power:
                return std::pow(leftVal, rightVal);
            default:
                throw std::runtime_error("Invalid binary operation");
        }
    }
    std::string toString() const override {
        return "(" + leftOperand->toString() + nodeTypeToString(type) +
               rightOperand->toString() + ")";
    }
    std::shared_ptr<Node<T>> copy() const override {
        return std::make_shared<BinaryOperationNode<T>>(
            type, leftOperand->copy(), rightOperand->copy());
    }

    std::shared_ptr<Node<T>> diff(const std::string& var) const;

    NodeType getType() const override { return type; }
};

template <IsNumeric T>
class FunctionNode : public Node<T> {
    NodeType type;
    std::shared_ptr<Node<T>> argument;

   public:
    FunctionNode(NodeType funcType, std::shared_ptr<Node<T>> arg)
        : type(funcType), argument(std::move(arg)) {}
    T eval(const std::map<std::string, T>& vars) const override {
        T argVal = argument->eval(vars);
        switch (type) {
            case NodeType::Sin:
                return std::sin(argVal);
            case NodeType::Cos:
                return std::cos(argVal);
            case NodeType::Exp:
                return std::exp(argVal);
            case NodeType::Ln:
                if constexpr (std::is_same_v<T, Complex>) {
                } else {
                    if (argVal <= T(0)) {
                        throw std::runtime_error("Ln domain error");
                    }
                }
                return std::log(argVal);
            default:
                throw std::runtime_error("Unsupported function");
        }
    }
    std::string toString() const override {
        return nodeTypeToString(type) + "(" + argument->toString() + ")";
    }
    std::shared_ptr<Node<T>> copy() const override {
        return std::make_shared<FunctionNode<T>>(type, argument->copy());
    }
    std::shared_ptr<Node<T>> diff(const std::string& var) const;

    NodeType getType() const override { return type; }
};

template <IsNumeric T = Real>
class Expression {
    std::shared_ptr<Node<T>> root;

   public:
    Expression(T val) : root(std::make_shared<ConstantNode<T>>(val)) {}
    Expression(const std::string& var)
        : root(std::make_shared<VariableNode<T>>(var)) {}
    Expression(const char var[])
        : root(std::make_shared<VariableNode<T>>(std::string(var))) {}
    explicit Expression(std::shared_ptr<Node<T>> node)
        : root(std::move(node)) {}

    Expression(const Expression& other) : root(other.root->copy()) {}
    Expression(Expression&& other) noexcept : root(std::move(other.root)) {}

    Expression& operator=(const Expression& other) {
        root = other.root->copy();
        return *this;
    }
    Expression& operator=(Expression&& other) noexcept {
        root = std::move(other.root);
        return *this;
    }

    Expression operator+(const Expression& other) const {
        return Expression(std::make_shared<BinaryOperationNode<T>>(
            NodeType::Add, root->copy(), other.root->copy()));
    }
    Expression operator-(const Expression& other) const {
        return Expression(std::make_shared<BinaryOperationNode<T>>(
            NodeType::Subtract, root->copy(), other.root->copy()));
    }
    Expression operator*(const Expression& other) const {
        return Expression(std::make_shared<BinaryOperationNode<T>>(
            NodeType::Multiply, root->copy(), other.root->copy()));
    }
    Expression operator/(const Expression& other) const {
        return Expression(std::make_shared<BinaryOperationNode<T>>(
            NodeType::Divide, root->copy(), other.root->copy()));
    }
    Expression operator^(const Expression& other) const {
        return Expression(std::make_shared<BinaryOperationNode<T>>(
            NodeType::Power, root->copy(), other.root->copy()));
    }

    T eval(const std::map<std::string, T>& vars) const {
        return root->eval(vars);
    }
    std::string toString() const { return root->toString(); }
    std::shared_ptr<Node<T>> copy() const { return root->copy(); }

    Expression sin() const {
        return Expression(
            std::make_shared<FunctionNode<T>>(NodeType::Sin, root->copy()));
    }
    Expression cos() const {
        return Expression(
            std::make_shared<FunctionNode<T>>(NodeType::Cos, root->copy()));
    }
    Expression exp() const {
        return Expression(
            std::make_shared<FunctionNode<T>>(NodeType::Exp, root->copy()));
    }
    Expression ln() const {
        return Expression(
            std::make_shared<FunctionNode<T>>(NodeType::Ln, root->copy()));
    }

    Expression diff(const std::string& var) const {
        return Expression(root->diff(var));
    }
};

template <IsNumeric T>
std::shared_ptr<Node<T>> simplifyAdd(std::shared_ptr<Node<T>> left,
                                     std::shared_ptr<Node<T>> right) {
    if (isZero(left)) return right;
    if (isZero(right)) return left;
    if (left->getType() == NodeType::Constant &&
        right->getType() == NodeType::Constant) {
        return std::make_shared<ConstantNode<T>>(left->eval({}) +
                                                 right->eval({}));
    }
    return std::make_shared<BinaryOperationNode<T>>(NodeType::Add, left, right);
}

template <IsNumeric T>
std::shared_ptr<Node<T>> simplifyMultiply(std::shared_ptr<Node<T>> left,
                                          std::shared_ptr<Node<T>> right) {
    if (isOne(left)) return right;
    if (isOne(right)) return left;
    if (isZero(left) || isZero(right))
        return std::make_shared<ConstantNode<T>>(0);
    if (left->getType() == NodeType::Constant &&
        right->getType() == NodeType::Constant) {
        return std::make_shared<ConstantNode<T>>(left->eval({}) *
                                                 right->eval({}));
    }
    return std::make_shared<BinaryOperationNode<T>>(NodeType::Multiply, left,
                                                    right);
}

template <IsNumeric T>
std::shared_ptr<Node<T>> simplifyDivide(std::shared_ptr<Node<T>> left,
                                        std::shared_ptr<Node<T>> right) {
    if (isOne(right)) return left;
    if (isZero(left)) return std::make_shared<ConstantNode<T>>(0);
    if (left->getType() == NodeType::Constant &&
        right->getType() == NodeType::Constant) {
        T divider = right->eval({});
        if (divider == T(0.)) {
            throw std::runtime_error("Division by zero");
        }
        return std::make_shared<ConstantNode<T>>(left->eval({}) / divider);
    }
    return std::make_shared<BinaryOperationNode<T>>(NodeType::Divide, left,
                                                    right);
}

template <IsNumeric T>
std::shared_ptr<Node<T>> simplifyPower(std::shared_ptr<Node<T>> base,
                                       std::shared_ptr<Node<T>> exponent) {
    if (isOne(exponent)) return base;
    if (isZero(exponent)) return std::make_shared<ConstantNode<T>>(1);
    if (base->getType() == NodeType::Constant &&
        exponent->getType() == NodeType::Constant) {
        return std::make_shared<ConstantNode<T>>(
            std::pow(base->eval({}), exponent->eval({})));
    }
    return std::make_shared<BinaryOperationNode<T>>(NodeType::Power, base,
                                                    exponent);
}

template <IsNumeric T>
std::shared_ptr<Node<T>> BinaryOperationNode<T>::diff(
    const std::string& var) const {
    auto leftDeriv = leftOperand->diff(var);
    auto rightDeriv = rightOperand->diff(var);

    switch (type) {
        case NodeType::Add:
            return simplifyAdd(leftDeriv, rightDeriv);

        case NodeType::Subtract:
            return simplifyAdd(
                leftDeriv,
                simplifyMultiply(std::static_pointer_cast<Node<T>>(
                                     std::make_shared<ConstantNode<T>>(-1)),
                                 rightDeriv));

        case NodeType::Multiply: {
            auto term1 = simplifyMultiply(leftDeriv, rightOperand->copy());
            auto term2 = simplifyMultiply(leftOperand->copy(), rightDeriv);
            return simplifyAdd(term1, term2);
        }

        case NodeType::Divide: {
            auto numerator = simplifyAdd(
                simplifyMultiply(leftDeriv, rightOperand->copy()),
                simplifyMultiply(
                    std::static_pointer_cast<Node<T>>(
                        std::make_shared<ConstantNode<T>>(-1)),
                    simplifyMultiply(leftOperand->copy(), rightDeriv)));
            auto denominator =
                simplifyPower(rightOperand->copy(),
                              std::static_pointer_cast<Node<T>>(
                                  std::make_shared<ConstantNode<T>>(2)));
            return simplifyDivide(numerator, denominator);
        }

        case NodeType::Power: {
            auto base = leftOperand->copy();
            auto exponent = rightOperand->copy();
            auto lnNode = std::static_pointer_cast<Node<T>>(
                std::make_shared<FunctionNode<T>>(NodeType::Ln, base->copy()));
            auto term1 = simplifyPower(base->copy(), exponent->copy());
            auto term2 = simplifyAdd(
                simplifyMultiply(rightDeriv->copy(), lnNode),
                simplifyMultiply(
                    exponent->copy(),
                    simplifyDivide(leftDeriv->copy(), base->copy())));
            return simplifyMultiply(term1, term2);
        }

        default:
            throw std::runtime_error("Unsupported binary operation for diff");
    }
}

template <IsNumeric T>
std::shared_ptr<Node<T>> FunctionNode<T>::diff(const std::string& var) const {
    auto argDeriv = argument->diff(var);

    switch (type) {
        case NodeType::Sin: {
            auto cosNode = std::static_pointer_cast<Node<T>>(
                std::make_shared<FunctionNode<T>>(NodeType::Cos,
                                                  argument->copy()));
            return simplifyMultiply(cosNode, argDeriv);
        }

        case NodeType::Cos: {
            auto sinNode = std::static_pointer_cast<Node<T>>(
                std::make_shared<FunctionNode<T>>(NodeType::Sin,
                                                  argument->copy()));
            auto negNode =
                simplifyMultiply(std::static_pointer_cast<Node<T>>(
                                     std::make_shared<ConstantNode<T>>(-1)),
                                 sinNode);
            return simplifyMultiply(negNode, argDeriv);
        }

        case NodeType::Exp: {
            auto expNode = std::static_pointer_cast<Node<T>>(
                std::make_shared<FunctionNode<T>>(NodeType::Exp,
                                                  argument->copy()));
            return simplifyMultiply(expNode, argDeriv);
        }

        case NodeType::Ln: {
            auto reciprocal =
                simplifyDivide(std::static_pointer_cast<Node<T>>(
                                   std::make_shared<ConstantNode<T>>(1)),
                               argument->copy());
            return simplifyMultiply(reciprocal, argDeriv);
        }

        default:
            throw std::runtime_error("Unsupported function for diff");
    }
}

template <IsNumeric T>
std::ostream& operator<<(std::ostream& out, const Expression<T>& expr) {
    out << expr.toString();
    return out;
}

template <IsNumeric T>
bool isOne(std::shared_ptr<Node<T>> node) {
    auto constNode = std::dynamic_pointer_cast<ConstantNode<T>>(node);
    return constNode && constNode->eval({}) == T(1);
}

template <IsNumeric T>
bool isZero(std::shared_ptr<Node<T>> node) {
    auto constNode = std::dynamic_pointer_cast<ConstantNode<T>>(node);
    return constNode && constNode->eval({}) == T(0);
}

int getPrecedence(char op) {
    if (op == '^') return 4;
    if (op == '*' || op == '/') return 3;
    if (op == '+' || op == '-') return 2;
    return 0;
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

bool isFunction(const std::string& token) {
    static const std::set<std::string> functions = {"sin", "cos", "exp", "ln"};
    return functions.count(token) > 0;
}

bool is_complex(const std::string& s) {
    size_t pos = 0;
    while ((pos = s.find('i', pos)) != std::string::npos) {
        bool is_part_of_other = false;

        if (pos > 0 && std::isalpha(s[pos - 1])) {
            is_part_of_other = true;
        }
        if (pos < s.size() - 1 && std::isalpha(s[pos + 1])) {
            is_part_of_other = true;
        }

        if (!is_part_of_other) {
            bool left_valid = (pos == 0) || std::isspace(s[pos - 1]) ||
                              std::isdigit(s[pos - 1]) || s[pos - 1] == '-' ||
                              s[pos - 1] == '.';

            bool right_valid = (pos == s.size() - 1) ||
                               std::isspace(s[pos + 1]) ||
                               std::isdigit(s[pos + 1]);

            if (left_valid && right_valid) {
                return true;
            }
        }

        pos++;
    }

    return false;
}

Complex ParseComplex(const std::string& s) {
    size_t i_pos = s.find('i');
    if (i_pos == std::string::npos) {
        return Complex(std::stod(s), 0.0);
    }

    std::string real_part, imag_part;
    size_t sign_pos = s.find_last_of("+-", i_pos - 1);
    if (sign_pos == std::string::npos) {
        imag_part = s.substr(0, i_pos);
    } else {
        real_part = s.substr(0, sign_pos);
        imag_part = s.substr(sign_pos, i_pos - sign_pos);
    }

    double real = real_part.empty() ? 0.0 : std::stod(real_part);
    double imag = imag_part.empty() || imag_part == "+" ? 1.0
                  : imag_part == "-"                    ? -1.0
                                                        : std::stod(imag_part);

    return Complex(real, imag);
}

template <IsNumeric T>
Expression<T> parseExpression(const std::string& input) {
    std::stack<Expression<T>> values;
    std::stack<char> operators;
    std::stack<std::string> functions;

    auto applyOperation = [&]() {
        char op = operators.top();
        operators.pop();

        Expression<T> right = values.top();
        values.pop();
        Expression<T> left = values.top();
        values.pop();

        switch (op) {
            case '+':
                values.push(left + right);
                break;
            case '-':
                values.push(left - right);
                break;
            case '*':
                values.push(left * right);
                break;
            case '/':
                values.push(left / right);
                break;
            case '^':
                values.push(left ^ right);
                break;
            default:
                throw std::runtime_error("Invalid operator");
        }
    };

    auto applyFunction = [&]() {
        std::string func = functions.top();
        functions.pop();

        Expression<T> arg = values.top();
        values.pop();

        if (func == "sin")
            values.push(arg.sin());
        else if (func == "cos")
            values.push(arg.cos());
        else if (func == "exp")
            values.push(arg.exp());
        else if (func == "ln")
            values.push(arg.ln());
        else
            throw std::runtime_error("Invalid function");
    };

    for (size_t i = 0; i < input.size(); ++i) {
        char current = input[i];

        if (std::isspace(current)) continue;

        if (std::isdigit(current) || current == '.') {
            std::string number;
            while (i < input.size() &&
                   (std::isdigit(input[i]) || input[i] == '.')) {
                number += input[i++];
            }
            --i;
            values.push(Expression<T>(static_cast<T>(std::stold(number))));
        }

        else if (std::isalpha(current)) {
            std::string token;
            while (i < input.size() && std::isalpha(input[i])) {
                token += input[i++];
            }
            --i;

            if (isFunction(token)) {
                functions.push(token);
            } else {
                values.push(Expression<T>(token));
            }
        }

        else if (current == '(') {
            operators.push(current);
        }

        else if (current == ')') {
            while (!operators.empty() && operators.top() != '(') {
                applyOperation();
            }
            operators.pop();

            if (!functions.empty()) {
                applyFunction();
            }
        }

        else if (isOperator(current)) {
            while (!operators.empty() &&
                   getPrecedence(operators.top()) >= getPrecedence(current)) {
                applyOperation();
            }
            operators.push(current);
        }

        else {
            throw std::runtime_error("Invalid character in input");
        }
    }

    while (!operators.empty()) {
        applyOperation();
    }

    if (values.size() != 1) {
        throw std::runtime_error("Invalid expression");
    }

    return values.top();
}

template <IsNumeric T>
Expression<T> fromString(const std::string& input) {
    return parseExpression<T>(input);
}

#endif