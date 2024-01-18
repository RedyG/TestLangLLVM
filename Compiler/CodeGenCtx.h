#pragma once
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h"
class RedyModule;

struct CodeGenCtx {
public:
	RedyModule& RedyMod;
	llvm::Module& Mod;
	llvm::IRBuilder<>& Builder;

	llvm::LLVMContext& GetLLVMCtx() {
		return Mod.getContext();
	}

	CodeGenCtx(llvm::Module& module, RedyModule& redyModule, llvm::IRBuilder<>& builder) : Mod(module), Builder(builder), RedyMod(redyModule) {}
};