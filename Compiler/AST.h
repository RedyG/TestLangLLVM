#pragma once
#include "RedyLexer.h"
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h";

class ExprAST {
public:
	virtual double CodeGen() = 0;
};

using ExprPtr = std::unique_ptr<ExprAST>;

class DoubleExp : public ExprAST {
public:
	double Value;

	double CodeGen() override;

	DoubleExp(double value) : Value(value) {}
};

class BinOpExpr : public ExprAST {
public:
	ExprPtr LHS;
	TokenType Op;
	ExprPtr RHS;

	double CodeGen() override;

	BinOpExpr(ExprPtr lhs, TokenType op, ExprPtr rhs) : LHS(std::move(lhs)), Op(op), RHS(std::move(rhs)) {}
};

class VariableExpr : public ExprAST {
public:
	std::string_view Name;

	double CodeGen() override;

	VariableExpr(std::string_view name) : Name(name) {}
};

class CallExpr : public ExprAST {
public:
	ExprPtr Callee;
	std::vector<ExprPtr> Params;

	double CodeGen() override;

	CallExpr(ExprPtr callee, std::vector<ExprPtr> params)
		: Callee(std::move(callee)), Params(std::move(params)) {}
};

class UnaryExpr : public ExprAST {
public:
	TokenType Op;
	ExprPtr Expr;

	double CodeGen() override;

	UnaryExpr(TokenType op, ExprPtr expr) : Op(op), Expr(std::move(expr)) {}
};
