#pragma once
#include "RedyLexer.h"
#include <variant>
#include <map>
#include "CodeGenCtx.h"
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h"
#include "TypeDeclAST.h"
#include "ExprType.h"

class ExprAST {
public:
	ExprType Type = UnknownType;

	// not using the visitor pattern because every passes return different types
	// so I would have to deal with std::any or something
	virtual llvm::Value* CodeGen(CodeGenCtx ctx) = 0;
	ExprType TypeCheck() {
		Type = OnTypeCheck();
		return Type;
	}

private:
	virtual ExprType OnTypeCheck() = 0;
};

using ExprPtr = std::unique_ptr<ExprAST>;

class IntExpr : public ExprAST {
public:
	int Value;

	llvm::Value* CodeGen(CodeGenCtx ctx) override;

	IntExpr(int value) : Value(value) {}

private:
	ExprType OnTypeCheck() override;
};

class FloatExpr : public ExprAST {
public:
	double Value;

	llvm::Value* CodeGen(CodeGenCtx ctx) override;

	FloatExpr(double value) : Value(value) {}

private:
	ExprType OnTypeCheck() override;
};

class BinOpExpr : public ExprAST {
public:
	ExprPtr LHS;
	TokenType Op;
	ExprPtr RHS;

	llvm::Value* CodeGen(CodeGenCtx ctx) override;

	BinOpExpr(ExprPtr lhs, TokenType op, ExprPtr rhs) : LHS(std::move(lhs)), Op(op), RHS(std::move(rhs)) {}

private:
	ExprType OnTypeCheck() override;
};

class TypeAST {
public:
	std::string_view Name;

	TypeAST(std::string_view name) : Name(name) {}
};

struct TypeASTComparer
{
	bool operator() (const TypeAST& lhs, const TypeAST& rhs) const
	{
		return lhs.Name < rhs.Name;
	}
};

class VariableAST {
public:
	TypeAST Type;
	std::string_view Name;
	ExprPtr DefaultValue;

	VariableAST(TypeAST type, std::string_view name, ExprPtr defaultValue = nullptr)
		: Type(type), Name(name), DefaultValue(std::move(defaultValue)) {}
};

class VariableDeclStatement;

class VariableExpr : public ExprAST {
public:
	VariableDeclStatement* Symbol;
	std::string_view Name; 
	llvm::Value* CodeGen(CodeGenCtx ctx) override;

	VariableExpr(std::string_view name) : Name(name) {}

private:
	ExprType OnTypeCheck() override;
};

class CallExpr : public ExprAST {
public:
	ExprPtr Callee;
	std::vector<ExprPtr> Params;

	llvm::Value* CodeGen(CodeGenCtx ctx) override;

	CallExpr(ExprPtr callee, std::vector<ExprPtr> params)
		: Callee(std::move(callee)), Params(std::move(params)) {}

private:
	ExprType OnTypeCheck() override;
};

class UnaryExpr : public ExprAST {
public:
	TokenType Op;
	ExprPtr Expr;

	llvm::Value* CodeGen(CodeGenCtx ctx) override;

	UnaryExpr(TokenType op, ExprPtr expr) : Op(op), Expr(std::move(expr)) {}

private:
	ExprType OnTypeCheck() override;
};


enum class VisibilityAST {
	Public,
	Private
};

class ProtoAST {
public:
	VisibilityAST Visibility;
	TypeAST Type;
	std::string_view Name;
	std::vector<VariableDeclStatement> Params;

	ProtoAST(VisibilityAST visibility, TypeAST type, std::string_view name, std::vector<VariableDeclStatement> params)
		: Visibility(visibility), Type(type), Name(name), Params(std::move(params)) {}

	void Register(llvm::Module& module);
};

class StatementAST {
public:
	virtual void TypeCheckStatement() = 0;
	virtual void CodeGenStatement(CodeGenCtx ctx) = 0;
};

using StatementPtr = std::unique_ptr<StatementAST>;

class BlockStatement : public StatementAST {
public:
	std::vector<StatementPtr> Statements;

	void TypeCheckStatement() override;
	void CodeGenStatement(CodeGenCtx ctx) override;

	BlockStatement(std::vector<StatementPtr> statements) : Statements(std::move(statements)) {}
};

class ReturnStatement : public StatementAST {
public:
	ExprPtr Expr;

	void TypeCheckStatement() override;
	void CodeGenStatement(CodeGenCtx ctx) override;

	ReturnStatement(ExprPtr expr) : Expr(std::move(expr)) {}
};

using BlockOrExpr = std::variant<ExprPtr, std::unique_ptr<BlockStatement>>;

class FuncAST {
public:
	ProtoAST Proto;
	BlockOrExpr Body;

	FuncAST(ProtoAST proto, BlockOrExpr body) : Proto(std::move(proto)), Body(std::move(body)) {}

	void TypeCheck();
	void CodeGen(CodeGenCtx ctx);
};

class ExprStatement : StatementAST {
public:
	ExprPtr Expr;

	ExprStatement(ExprPtr expr) : Expr(std::move(expr)) {}
};

class VariableDeclStatement: public StatementAST {
public:
	VariableAST Variable;
	llvm::AllocaInst* Alloca;

	void TypeCheckStatement() override;
	void CodeGenStatement(CodeGenCtx ctx) override;

	VariableDeclStatement(VariableAST variable) : Variable(std::move(variable)) {}
};

// TODO: add other visiblites like pub(get) and pub(set)
class FieldAST {
public:
	VisibilityAST Visibility;
	VariableAST Variable;

	FieldAST(VisibilityAST visibility, VariableAST variable)
		: Visibility(visibility), Variable(std::move(variable)) {}
};


class StructAST {
public:
	TypeDeclAST TypeDecl;
	std::vector<FieldAST> Fields;
	
	void TypeCheck();
	void CodeGen(CodeGenCtx ctx);
	void Register(llvm::Module& module);

	StructAST(std::vector<FieldAST> fields, TypeDeclAST typeDecl)
		: Fields(std::move(fields)), TypeDecl(std::move(typeDecl)) {}
};

class ModuleAST {
public:
	std::vector<StructAST> Structs;

	void TypeCheck();
	void CodeGen(CodeGenCtx ctx);
	void Register(llvm::Module& module);

	ModuleAST(std::vector<StructAST> structs) : Structs(std::move(structs)) {}
};

TypeDeclAST* UnknownDecl = new TypeDeclAST(VisibilityAST::Public, "Unknown", {});