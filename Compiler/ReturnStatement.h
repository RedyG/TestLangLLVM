#pragma once
#include "StatementAST.h"
class RedyModule;
#include "ExprPtr.h"

class ReturnStatement : public StatementAST {
public:
	ExprPtr Expr;

	void TypeCheckStatement(TypeCheckCtx ctx) override;
	void CodeGenStatement(CodeGenCtx ctx) override;
	bool IsBranch() override;

	ReturnStatement(ExprPtr expr, NodeAST node);
};