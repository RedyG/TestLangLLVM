#pragma once
#include "StatementAST.h"
#include "ExprPtr.h"

class ReturnStatement : public StatementAST {
public:
	ExprPtr Expr;

	void TypeCheckStatement() override;
	void CodeGenStatement(CodeGenCtx ctx) override;

	ReturnStatement(ExprPtr expr) : Expr(std::move(expr)) {}
};