#pragma once
class RedyModule;
#include "llvm/IR/IRBuilder.h"

struct TypeCheckCtx {
	RedyModule& RedyMod;
	llvm::LLVMContext& LLVMCtx;
	std::string_view File;

	TypeCheckCtx(RedyModule& redyModule, llvm::LLVMContext& llvmContext, std::string_view file);
};