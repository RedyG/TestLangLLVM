#pragma once
#include "ExprAST.h"

class BoolExpr : public ExprAST {
public:
	bool Value;

	BoolExpr(bool value, NodeAST node) : Value(value), ExprAST(node) {}

	llvm::Value* CodeGen(CodeGenCtx ctx) override;

private:
	TypeDeclAST* OnTypeCheck(TypeCheckCtx ctx) override;
};