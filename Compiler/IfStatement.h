#pragma once
#include "ExprPtr.h"
#include "BlockStatement.h"

class IfStatement : public StatementAST {
public:
	ExprPtr Condition;
	BlockStatement ThenBlock;
	StatementPtr ElseBlock;

	void CodeGenStatement(CodeGenCtx ctx) override;

	IfStatement(ExprPtr condition, BlockStatement thenBlock, StatementPtr elseBlock = nullptr)
		: Condition(std::move(condition)), ThenBlock(std::move(thenBlock)), ElseBlock(std::move(elseBlock)) {}
private:
	void TypeCheckStatement(RedyModule& module, llvm::LLVMContext& context) override;
};