#pragma once
#include "CodeGenCtx.h"
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h"

class StatementAST {
public:
	virtual void TypeCheckStatement(llvm::LLVMContext& context) = 0;
	virtual void CodeGenStatement(CodeGenCtx ctx) = 0;
};

using StatementPtr = std::unique_ptr<StatementAST>;