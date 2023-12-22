#pragma once
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h"
using namespace llvm;

struct CodeGenCtx {
public:
	Module& Module;
	IRBuilder<>& Builder;

	LLVMContext& GetContext() {
		return Module.getContext();
	}

	CodeGenCtx(llvm::Module& module, IRBuilder<>& builder) : Module(module), Builder(builder) {}
};