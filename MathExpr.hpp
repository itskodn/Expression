#ifndef MATHEXPR_HPP
#define MATHEXPR_HPP

#include <memory>
#include <map>
#include <stdexcept>
#include <string> 
#include <iostream>  
using ValueType = long double;

ValueType power(ValueType base, ValueType exponent);

enum class OperationType {Number,Symbol, Add,Subtract,Multiply,Divide,Power, Sine, Cosine, Log,Exp };

std::string operationTypeToString(OperationType type);
class MathNode {
public:
    virtual ~MathNode() = default;
    virtual ValueType evaluate(const std::map<std::string, ValueType>& vars) const = 0;
    virtual std::string toString() const = 0;
    virtual std::shared_ptr<MathNode> clone() const = 0;
};

class NumberNode : public MathNode {
    ValueType value;
public:
    NumberNode(ValueType val);
    ValueType evaluate(const std::map<std::string, ValueType>& vars) const override;
    std::string toString() const override;
    std::shared_ptr<MathNode> clone() const override;
};

class SymbolNode : public MathNode {
    std::string symbol;
public:
    SymbolNode(const std::string& sym);
    ValueType evaluate(const std::map<std::string, ValueType>& vars) const override;
    std::string toString() const override;
    std::shared_ptr<MathNode> clone() const override;
};

class BinaryOpNode : public MathNode {
    OperationType op;
    std::shared_ptr<MathNode> left, right;
public:
    BinaryOpNode(OperationType op, std::shared_ptr<MathNode> left, std::shared_ptr<MathNode> right);
    ValueType evaluate(const std::map<std::string, ValueType>& vars) const override;
    std::string toString() const override;
    std::shared_ptr<MathNode> clone() const override;
};

class FunctionNode : public MathNode {
    OperationType func;
    std::shared_ptr<MathNode> arg;
public:
    FunctionNode(OperationType func, std::shared_ptr<MathNode> arg);
    ValueType evaluate(const std::map<std::string, ValueType>& vars) const override;
    std::string toString() const override;
    std::shared_ptr<MathNode> clone() const override;
};

class MathExpression {
    std::shared_ptr<MathNode> root;
public:
    MathExpression pow(const MathExpression& exponent) const;
    MathExpression(ValueType val);
    MathExpression(const std::string& sym);
    MathExpression(std::shared_ptr<MathNode> node);
    MathExpression(const MathExpression& other);
    MathExpression(MathExpression&& other) noexcept;

    MathExpression& operator=(const MathExpression& other);
    MathExpression& operator=(MathExpression&& other) noexcept;

    MathExpression operator+(const MathExpression& other) const;
    MathExpression operator-(const MathExpression& other) const;
    MathExpression operator*(const MathExpression& other) const;
    MathExpression operator/(const MathExpression& other) const;
    MathExpression operator^(const MathExpression& other) const;

    ValueType evaluate(const std::map<std::string, ValueType>& vars) const;
    std::string toString() const;
    std::shared_ptr<MathNode> cloneRoot() const;  
};

std::ostream& operator<<(std::ostream& out, const MathExpression& expr);

MathExpression sin(const MathExpression& expr);
MathExpression cos(const MathExpression& expr);
MathExpression exp(const MathExpression& expr);
MathExpression log(const MathExpression& expr);

#endif 