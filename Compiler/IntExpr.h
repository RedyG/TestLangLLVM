#pragma once
#include "ExprAST.h"

class IntExpr : public ExprAST {
public:
	int Value;

	llvm::Value* CodeGen(CodeGenCtx ctx) override;

	IntExpr(int value) : Value(value) {}

private:
	ExprType OnTypeCheck(llvm::LLVMContext& context) override;
};