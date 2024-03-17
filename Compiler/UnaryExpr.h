#pragma once
#include "ExprAST.h"
#include "RedyLexer.h"
#include "ExprPtr.h"

class UnaryExpr : public ExprAST {
public:
	TokenType Op;
	ExprPtr Expr;

	llvm::Value* CodeGen(CodeGenCtx ctx) override;

	UnaryExpr(TokenType op, ExprPtr expr, NodeAST node) : Op(op), Expr(std::move(expr)), ExprAST(node) {}

private:
	TypeDeclAST* OnTypeCheck(TypeCheckCtx ctx) override;
};
