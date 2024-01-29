#pragma once
#include "StatementAST.h"
class RedyModule;
#include "ExprPtr.h"

class ReturnStatement : public StatementAST {
public:
	ExprPtr Expr;

	void TypeCheckStatement(RedyModule& module, llvm::LLVMContext& context) override;
	void CodeGenStatement(CodeGenCtx ctx) override;
	bool IsBranch() override;

	ReturnStatement(ExprPtr expr);
};