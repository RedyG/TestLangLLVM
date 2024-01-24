#pragma once
#include "VariableAST.h"
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h"

struct SymbolAST {
	VariableAST Variable;
	llvm::AllocaInst* Alloca = nullptr;

	SymbolAST(VariableAST variable) : Variable(std::move(variable)) {}
};