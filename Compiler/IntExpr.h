#pragma once
#include "ExprAST.h"

class IntExpr : public ExprAST {
public:
	int Value;

	llvm::Value* CodeGen(CodeGenCtx ctx) override;

	IntExpr(int value, NodeAST node) : Value(value), ExprAST(node) {}

private:
	TypeDeclAST* OnTypeCheck(TypeCheckCtx ctx) override;
};