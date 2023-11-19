#pragma once
#include "RedyLexer.h"
#include <map>
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h";
using namespace llvm;

class ExprAST {
public:
	virtual Value* CodeGen() = 0;
};

using ExprPtr = std::unique_ptr<ExprAST>;

class DoubleExpr : public ExprAST {
public:
	double Value;

	llvm::Value* CodeGen() override;

	DoubleExpr(double value) : Value(value) {}
};

class BinOpExpr : public ExprAST {
public:
	ExprPtr LHS;
	TokenType Op;
	ExprPtr RHS;

	Value* CodeGen() override;

	BinOpExpr(ExprPtr lhs, TokenType op, ExprPtr rhs) : LHS(std::move(lhs)), Op(op), RHS(std::move(rhs)) {}
};

class VariableExpr : public ExprAST {
public:
	std::string_view Name;

	Value* CodeGen() override;

	VariableExpr(std::string_view name) : Name(name) {}
};

class CallExpr : public ExprAST {
public:
	ExprPtr Callee;
	std::vector<ExprPtr> Params;

	Value* CodeGen() override;

	CallExpr(ExprPtr callee, std::vector<ExprPtr> params)
		: Callee(std::move(callee)), Params(std::move(params)) {}
};

class UnaryExpr : public ExprAST {
public:
	TokenType Op;
	ExprPtr Expr;

	Value* CodeGen() override;

	UnaryExpr(TokenType op, ExprPtr expr) : Op(op), Expr(std::move(expr)) {}
};


class TypeAST {
public:
	std::string_view Name;

	TypeAST(std::string_view name) : Name(name) {}

	Type* CodeGen();
};

class ParamAST {
public:
	TypeAST Type;
	std::string_view Name;
	ExprPtr DefaultValue;

	ParamAST(TypeAST type, std::string_view name, ExprPtr defaultValue = nullptr)
		: Type(type), Name(name), DefaultValue(std::move(defaultValue)) {}
};

enum class FuncVisibility {
	Public,
	Private
};

class ProtoAST {
public:
	FuncVisibility Visibility;
	TypeAST Type;
	std::string_view Name;
	std::vector<ParamAST> Params;

	ProtoAST(FuncVisibility visibility, TypeAST type, std::string_view name, std::vector<ParamAST> params)
		: Visibility(visibility), Type(type), Name(name), Params(std::move(params)) {}

	Function* CodeGen();

};

class FuncAST {
public:
	ProtoAST Proto;
	ExprPtr Body;

	FuncAST(ProtoAST proto, ExprPtr body) : Proto(std::move(proto)), Body(std::move(body)) {}

	Function* CodeGen();
};