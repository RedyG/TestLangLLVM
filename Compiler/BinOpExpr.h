#pragma once
#include "ExprAST.h"
#include "RedyLexer.h"
#include "ExprPtr.h"

class BinOpExpr : public ExprAST {
public:
	ExprPtr LHS;
	TokenType Op;
	ExprPtr RHS;

	llvm::Value* CodeGen(CodeGenCtx ctx) override;

	BinOpExpr(ExprPtr lhs, TokenType op, ExprPtr rhs) : LHS(std::move(lhs)), Op(op), RHS(std::move(rhs)) {}

private:
	ExprType OnTypeCheck(llvm::LLVMContext& context) override;
};