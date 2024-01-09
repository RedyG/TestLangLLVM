#pragma once
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h"

struct CodeGenCtx {
public:
	llvm::Module& Module;
	llvm::IRBuilder<>& Builder;

	llvm::LLVMContext& GetContext() {
		return Module.getContext();
	}

	CodeGenCtx(llvm::Module& module, llvm::IRBuilder<>& builder) : Module(module), Builder(builder) {}
};