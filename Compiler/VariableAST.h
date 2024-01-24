#pragma once
#include "TypeAST.h"
#include <string>
#include "ExprPtr.h"
#include "ExprAST.h"

struct VariableAST {
	TypeAST Type;
	std::string_view Name;
	ExprPtr DefaultValue;

	VariableAST(TypeAST type, std::string_view name, ExprPtr defaultValue = nullptr);
};