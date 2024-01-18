#pragma once
#include "ExprAST.h"

class IntExpr : public ExprAST {
public:
	int Value;

	llvm::Value* CodeGen(CodeGenCtx ctx) override;

	IntExpr(int value) : Value(value) {}

private:
	TypeDeclAST* OnTypeCheck(RedyModule& module, llvm::LLVMContext& context) override;
};