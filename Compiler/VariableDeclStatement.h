#pragma once
#include "StatementAST.h"
#include "SymbolAST.h"

namespace llvm {
	class AllocaInst;
}

class VariableDeclStatement : public StatementAST {
public:
	SymbolAST Symbol;

	void TypeCheckStatement(RedyModule& module, llvm::LLVMContext& context) override;
	void CodeGenStatement(CodeGenCtx ctx) override;

	VariableDeclStatement(SymbolAST symbol) : Symbol(std::move(symbol)) {}
};