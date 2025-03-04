#include "MathExpr.hpp"
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