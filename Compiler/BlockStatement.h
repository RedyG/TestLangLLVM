#pragma once
#include "StatementAST.h"
#include <variant>

class BlockStatement : public StatementAST {
public:
	std::vector<StatementPtr> Statements;

	void TypeCheckStatement(RedyModule& module, llvm::LLVMContext& context) override;
	void CodeGenStatement(CodeGenCtx ctx) override;

	BlockStatement(std::vector<StatementPtr> statements) : Statements(std::move(statements)) {}
};

