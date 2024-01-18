#include "RedyModule.h"
#include "StructAST.h"
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h"
using namespace llvm;

void StructAST::GenLLVMType(LLVMContext& context, RedyModule& module) {
	std::vector<Type*> types;
	for (auto& field : Fields) {
		types.push_back(module.GetType(field.Variable.Type, context)->LLVMType);
	}

	LLVMType = StructType::create(types, Name);
}
