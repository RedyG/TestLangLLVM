#pragma once
#include "ExprAST.h"

class BoolExpr : public ExprAST {
public:
	bool Value;

	BoolExpr(bool value) : Value(value) {}

	llvm::Value* CodeGen(CodeGenCtx ctx) override;

private:
	TypeDeclAST* OnTypeCheck(RedyModule& module, llvm::LLVMContext& context) override;
};