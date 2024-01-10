#pragma once
#include <string>
#include <vector>
#include "VisibilityAST.h"
#include "FuncAST.h"

class TypeDeclAST {
public:
	VisibilityAST Visibility;
	std::string_view Name;
	std::vector<FuncAST> Methods;

	TypeDeclAST(VisibilityAST visibility, std::string_view name, std::vector<FuncAST> methods)
		: Visibility(visibility), Name(name), Methods(std::move(methods)) {}
};