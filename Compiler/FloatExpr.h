#pragma once
#include "ExprAST.h"

class FloatExpr : public ExprAST {
public:
	double Value;

	llvm::Value* CodeGen(CodeGenCtx ctx) override;

	FloatExpr(double value, NodeAST node) : Value(value), ExprAST(node) {}

private:
	TypeDeclAST* OnTypeCheck(TypeCheckCtx ctx) override;
};