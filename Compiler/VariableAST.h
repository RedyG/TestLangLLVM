#pragma once
#include "TypeAST.h"
#include <string>
#include "ExprAST.h"
#include "ExprPtr.h"

class VariableAST {
public:
	TypeAST Type;
	std::string_view Name;
	ExprPtr DefaultValue;

	VariableAST(TypeAST type, std::string_view name, ExprPtr defaultValue = nullptr)
		: Type(type), Name(name), DefaultValue(std::move(defaultValue)) {}
};