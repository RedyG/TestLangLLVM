#include "VariableAST.h"
#include "ExprAST.h"
VariableAST::VariableAST(TypeAST type, std::string_view name, NodeAST node, ExprPtr defaultValue) : ReturnType(type), Name(name), DefaultValue(std::move(defaultValue)), Node(node) {}