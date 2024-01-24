#pragma once
#include "SymbolAST.h"

struct ParamAST {
	SymbolAST Symbol;
	llvm::Argument* LLVMArg = nullptr;

	ParamAST(SymbolAST symbol) : Symbol(std::move(symbol)) {}

	void CodeGen(CodeGenCtx ctx);
};