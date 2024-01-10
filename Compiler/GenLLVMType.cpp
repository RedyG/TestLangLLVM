
#include "StructAST.h"
#include "TypeTable.h"
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h"
using namespace llvm;

Type* StructAST::GenLLVMType(LLVMContext& context) {
	std::vector<Type*> types;
	for (auto& field : Fields) {
		types.push_back(TypeTable::GetExprType(field.Variable.Type, context).LLVMType);
	}

	return StructType::create(types, Name);
}
