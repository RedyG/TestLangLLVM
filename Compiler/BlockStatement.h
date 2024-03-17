#pragma once
#include "StatementAST.h"
#include <variant>
#include "ParamAST.h"

class BlockStatement : public StatementAST {
public:
	std::vector<StatementPtr> Statements;

	void TypeCheckStatement(TypeCheckCtx ctx) override;
	void CodeGenStatement(CodeGenCtx ctx) override;
	void CodeGenWithParams(CodeGenCtx ctx, std::vector<ParamAST>& params);
	llvm::BasicBlock* CodeGen(CodeGenCtx ctx, std::string_view name);

	BlockStatement(std::vector<StatementPtr> statements, NodeAST node) : Statements(std::move(statements)), StatementAST(node) {}
};

