#pragma once
class RedyModule;
class FuncAST;
#include "llvm/IR/IRBuilder.h"

struct TypeCheckCtx {
	RedyModule& RedyMod;
	FuncAST& Func;
	llvm::LLVMContext& LLVMCtx;
	std::string_view File;

	TypeCheckCtx(RedyModule& redyModule, llvm::LLVMContext& llvmContext, std::string_view file, FuncAST& func);
};