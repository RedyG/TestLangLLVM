#pragma once
#include "StatementAST.h"
#include "ExprAST.h"
#include "ExprPtr.h"

class ExprStatement : StatementAST {
public:
	ExprPtr Expr;

	ExprStatement(ExprPtr expr) : Expr(std::move(expr)) {}
};
