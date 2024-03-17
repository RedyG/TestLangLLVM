#pragma once
#include <string>
#include "ExprAST.h"
#include "VariableDeclStatement.h"
namespace llvm {
	class Value;
}

class VariableExpr : public ExprAST {
public:
	SymbolAST* Symbol;
	std::string_view Name;
	llvm::Value* CodeGen(CodeGenCtx ctx) override;

	VariableExpr(std::string_view name, NodeAST node) : Name(name), ExprAST(node) {}

private:

	TypeDeclAST* OnTypeCheck(TypeCheckCtx ctx) override;
};