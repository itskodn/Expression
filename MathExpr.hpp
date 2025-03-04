#include <complex>
#include <memory>
#include <map>
#include <stdexcept>
#include <string>
#include <iostream>

enum class OperationType {
    Number,  
    Symbol,  
    Add,    
    Subtract,
    Multiply,
    Divide,  
    Power,   
    Sine,    
    Cosine, 
    Log,    
    Exp      
};
std::string operationTypeToString(OperationType type);


template <typename T>
class MathNode {
public:
    virtual ~MathNode() = default;
    virtual T evaluate(const std::map<std::string, T>& vars) const = 0;
    virtual std::string toString() const = 0;
    virtual std::shared_ptr<MathNode<T>> clone() const = 0;
};

template <typename T>
class NumberNode : public MathNode<T> {
    T value;
public:
    NumberNode(T val) : value(val) {}
    T evaluate(const std::map<std::string, T>& vars) const override {
        return value;
    }
    std::string toString() const override {
        return std::to_string(value);
    }
    std::shared_ptr<MathNode<T>> clone() const override {
        return std::make_shared<NumberNode<T>>(value);
    }
};

template <typename T>
class SymbolNode : public MathNode<T> {
    std::string symbol;
public:
    SymbolNode(const std::string& sym) : symbol(sym) {}
    T evaluate(const std::map<std::string, T>& vars) const override {
        auto it = vars.find(symbol);
        if (it == vars.end()) throw std::runtime_error("Symbol not found: " + symbol);
        return it->second;
    }
    std::string toString() const override {
        return symbol;
    }
    std::shared_ptr<MathNode<T>> clone() const override {
        return std::make_shared<SymbolNode<T>>(symbol);
    }
};

template <typename T>
class BinaryOpNode : public MathNode<T> {
    OperationType op;
    std::shared_ptr<MathNode<T>> left, right;
public:
    BinaryOpNode(OperationType op, std::shared_ptr<MathNode<T>> left, std::shared_ptr<MathNode<T>> right)
        : op(op), left(left), right(right) {}
    T evaluate(const std::map<std::string, T>& vars) const override {
        T leftVal = left->evaluate(vars);
        T rightVal = right->evaluate(vars);
        switch (op) {
            case OperationType::Add: return leftVal + rightVal;
            case OperationType::Subtract: return leftVal - rightVal;
            case OperationType::Multiply: return leftVal * rightVal;
            case OperationType::Divide: return leftVal / rightVal;
            case OperationType::Power: return std::pow(leftVal, rightVal);
            default: throw std::runtime_error("Unknown binary operation");
        }
    }
    std::string toString() const override {
        return left->toString() + " " + operationTypeToString(op) + " " + right->toString();
    }
    std::shared_ptr<MathNode<T>> clone() const override {
        return std::make_shared<BinaryOpNode<T>>(op, left->clone(), right->clone());
    }
};

template <typename T>
class FunctionNode : public MathNode<T> {
    OperationType func;
    std::shared_ptr<MathNode<T>> arg;
public:
    FunctionNode(OperationType func, std::shared_ptr<MathNode<T>> arg)
        : func(func), arg(arg) {}
    T evaluate(const std::map<std::string, T>& vars) const override {
        T argVal = arg->evaluate(vars);
        switch (func) {
            case OperationType::Sine: return std::sin(argVal);
            case OperationType::Cosine: return std::cos(argVal);
            case OperationType::Exp: return std::exp(argVal);
            case OperationType::Log: return std::log(argVal);
            default: throw std::runtime_error("Unknown function");
        }
    }
    std::string toString() const override {
        return operationTypeToString(func) + "(" + arg->toString() + ")";
    }
    std::shared_ptr<MathNode<T>> clone() const override {
        return std::make_shared<FunctionNode<T>>(func, arg->clone());
    }
};

template <typename T>
class MathExpression {
    std::shared_ptr<MathNode<T>> root;
public:
    MathExpression(T val) : root(std::make_shared<NumberNode<T>>(val)) {}
    MathExpression(const std::string& sym) : root(std::make_shared<SymbolNode<T>>(sym)) {}
    MathExpression(std::shared_ptr<MathNode<T>> node) : root(node) {}

    T evaluate(const std::map<std::string, T>& vars) const {
        return root->evaluate(vars);
    }

    std::string toString() const {
        return root->toString();
    }

    MathExpression<T> operator+(const MathExpression<T>& other) const {
        return MathExpression<T>(std::make_shared<BinaryOpNode<T>>(OperationType::Add, root->clone(), other.root->clone()));
    }

    MathExpression<T> operator-(const MathExpression<T>& other) const {
        return MathExpression<T>(std::make_shared<BinaryOpNode<T>>(OperationType::Subtract, root->clone(), other.root->clone()));
    }

    MathExpression<T> operator*(const MathExpression<T>& other) const {
        return MathExpression<T>(std::make_shared<BinaryOpNode<T>>(OperationType::Multiply, root->clone(), other.root->clone()));
    }

    MathExpression<T> operator/(const MathExpression<T>& other) const {
        return MathExpression<T>(std::make_shared<BinaryOpNode<T>>(OperationType::Divide, root->clone(), other.root->clone()));
    }

    MathExpression<T> operator^(const MathExpression<T>& other) const {
        return MathExpression<T>(std::make_shared<BinaryOpNode<T>>(OperationType::Power, root->clone(), other.root->clone()));
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const MathExpression<T>& expr) {
    out << expr.toString();
    return out;
}

template <typename T>
MathExpression<T> sin(const MathExpression<T>& expr) {
    return MathExpression<T>(std::make_shared<FunctionNode<T>>(OperationType::Sine, expr.root->clone()));
}

template <typename T>
MathExpression<T> cos(const MathExpression<T>& expr) {
    return MathExpression<T>(std::make_shared<FunctionNode<T>>(OperationType::Cosine, expr.root->clone()));
}

template <typename T>
MathExpression<T> exp(const MathExpression<T>& expr) {
    return MathExpression<T>(std::make_shared<FunctionNode<T>>(OperationType::Exp, expr.root->clone()));
}

template <typename T>
MathExpression<T> log(const MathExpression<T>& expr) {
    return MathExpression<T>(std::make_shared<FunctionNode<T>>(OperationType::Log, expr.root->clone()));
}