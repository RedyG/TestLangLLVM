#pragma once
#include "StatementAST.h"
#include "SymbolAST.h"

namespace llvm {
	class AllocaInst;
}

class VariableDeclStatement : public StatementAST {
public:
	SymbolAST Symbol;

	void TypeCheckStatement(TypeCheckCtx ctx) override;
	void CodeGenStatement(CodeGenCtx ctx) override;

	VariableDeclStatement(SymbolAST symbol, NodeAST node) : Symbol(std::move(symbol)), StatementAST(node)  {}
};