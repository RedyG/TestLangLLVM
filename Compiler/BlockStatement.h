#pragma once
#include "StatementAST.h"
#include <variant>
#include "ParamAST.h"

class BlockStatement : public StatementAST {
public:
	std::vector<StatementPtr> Statements;

	void TypeCheckStatement(RedyModule& module, llvm::LLVMContext& context) override;
	void CodeGenStatement(CodeGenCtx ctx) override;
	void CodeGenWithParams(CodeGenCtx ctx, std::vector<ParamAST>& params);

	BlockStatement(std::vector<StatementPtr> statements) : Statements(std::move(statements)) {}
};

