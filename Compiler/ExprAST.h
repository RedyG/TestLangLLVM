#pragma once
#include "ExprType.h"
#include "CodeGenCtx.h"
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h"

class ExprAST {
public:
	ExprType Type = UnknownType;

	// not using the visitor pattern because every passes return different types
	// so I would have to deal with std::any or something
	virtual llvm::Value* CodeGen(CodeGenCtx ctx) = 0;
	ExprType TypeCheck(llvm::LLVMContext& context) {
		Type = OnTypeCheck(context);
		return Type;
	}

private:
	virtual ExprType OnTypeCheck(llvm::LLVMContext& context) = 0;
};