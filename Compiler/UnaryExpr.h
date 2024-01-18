#pragma once
#include "ExprAST.h"
#include "RedyLexer.h"
#include "ExprPtr.h"

class UnaryExpr : public ExprAST {
public:
	TokenType Op;
	ExprPtr Expr;

	llvm::Value* CodeGen(CodeGenCtx ctx) override;

	UnaryExpr(TokenType op, ExprPtr expr) : Op(op), Expr(std::move(expr)) {}

private:
	TypeDeclAST* OnTypeCheck(RedyModule& module, llvm::LLVMContext& context) override;
};
