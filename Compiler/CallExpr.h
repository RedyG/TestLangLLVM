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
	TypeDeclAST* OnTypeCheck(RedyModule& module, llvm::LLVMContext& context) override;
};