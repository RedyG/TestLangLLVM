#pragma once
#include "TypeDeclAST.h"
#include "FieldAST.h"
#include <vector>

class StructAST : public TypeDeclAST {
public:
	std::vector<FieldAST> Fields;

	void CodeGen(CodeGenCtx ctx);

	llvm::Type* GenLLVMType(llvm::LLVMContext& context) override;

	StructAST(std::vector<FieldAST> fields, VisibilityAST visibility, std::string_view name, std::vector<FuncAST> methods) : TypeDeclAST(visibility, name, std::move(methods)) {}
};