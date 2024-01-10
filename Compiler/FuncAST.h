#pragma once
#include "ProtoAST.h"
#include "BlockStatement.h"
#include "CodeGenCtx.h"
#include "ExprPtr.h"

using BlockOrExpr = std::variant<ExprPtr, std::unique_ptr<BlockStatement>>;

class FuncAST {
public:
	ProtoAST Proto;
	BlockOrExpr Body;

	FuncAST(ProtoAST proto, BlockOrExpr body) : Proto(std::move(proto)), Body(std::move(body)) {}

	void TypeCheck();
	void CodeGen(CodeGenCtx ctx);
};