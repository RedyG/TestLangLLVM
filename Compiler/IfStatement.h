#pragma once
#include "ExprPtr.h"
#include "BlockStatement.h"

class IfStatement : public StatementAST {
public:
	ExprPtr Condition;
	BlockStatement ThenBlock;
	StatementPtr ElseBlock;

	void CodeGenStatement(CodeGenCtx ctx) override;

	IfStatement(ExprPtr condition, BlockStatement thenBlock, NodeAST node, StatementPtr elseBlock = nullptr)
		: Condition(std::move(condition)), ThenBlock(std::move(thenBlock)), ElseBlock(std::move(elseBlock)), StatementAST(node) {}
private:
	void TypeCheckStatement(TypeCheckCtx ctx) override;
};