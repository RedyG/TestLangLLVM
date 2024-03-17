#pragma once
#include "StatementAST.h"
#include "ExprAST.h"
#include "ExprPtr.h"

class ExprStatement : StatementAST {
public:
	ExprPtr Expr;

	ExprStatement(ExprPtr expr, NodeAST node) : Expr(std::move(expr)), StatementAST(node) {}
};
