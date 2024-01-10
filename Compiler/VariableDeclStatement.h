#pragma once
#include "VariableAST.h"
#include "StatementAST.h"

namespace llvm {
	class AllocaInst;
}

class VariableDeclStatement : public StatementAST {
public:
	VariableAST Variable;
	llvm::AllocaInst* Alloca;

	void TypeCheckStatement() override;
	void CodeGenStatement(CodeGenCtx ctx) override;

	VariableDeclStatement(VariableAST variable) : Variable(std::move(variable)) {}
};
