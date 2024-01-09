#pragma once
#include <map>
#include "AST.h" 

namespace TypeTable {
	ExprType GetExprType(TypeAST type);
	void AddExprType(TypeAST type, ExprType exprType);
}