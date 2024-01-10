#pragma once
#include "ExprAST.h"
#include "ExprPtr.h"
#include <vector>

class CallExpr : public ExprAST {
public:
	ExprPtr Callee;
	std::vector<ExprPtr> Params;

	llvm::Value* CodeGen(CodeGenCtx ctx) override;

	CallExpr(ExprPtr callee, std::vector<ExprPtr> params)
		: Callee(std::move(callee)), Params(std::move(params)) {}

private:
	ExprType OnTypeCheck(llvm::LLVMContext& context) override;
};