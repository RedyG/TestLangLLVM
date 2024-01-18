#pragma once
#include "ExprAST.h"

class FloatExpr : public ExprAST {
public:
	double Value;

	llvm::Value* CodeGen(CodeGenCtx ctx) override;

	FloatExpr(double value) : Value(value) {}

private:
	TypeDeclAST* OnTypeCheck(RedyModule& module, llvm::LLVMContext& context) override;
};