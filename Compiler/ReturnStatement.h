#pragma once
#include "StatementAST.h"
class RedyModule;
#include "ExprPtr.h"

class ReturnStatement : public StatementAST {
public:
	ExprPtr Expr;

	void TypeCheckStatement(RedyModule& module, llvm::LLVMContext& context) override;
	void CodeGenStatement(CodeGenCtx ctx) override;

	ReturnStatement(ExprPtr expr) : Expr(std::move(expr)) {}
};