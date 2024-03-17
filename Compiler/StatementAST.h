#pragma once
#include "NodeAST.h"
#include "CodeGenCtx.h"
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h"
#include "TypeCheckCtx.h"

class StatementAST {
public:
	NodeAST Node;
	virtual void TypeCheckStatement(TypeCheckCtx ctx) = 0;
	virtual void CodeGenStatement(CodeGenCtx ctx) = 0;
	virtual bool IsBranch();
	StatementAST(NodeAST node);
};

using StatementPtr = std::unique_ptr<StatementAST>;