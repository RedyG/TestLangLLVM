#pragma once
#include "TypeDeclAST.h"
#include "FieldAST.h"
#include <vector>

class StructAST : public TypeDeclAST {
public:
	std::vector<FieldAST> Fields;

	void CodeGen(CodeGenCtx ctx);

	void GenLLVMType(llvm::LLVMContext& context, RedyModule& module) override;

	StructAST(std::vector<FieldAST> fields, VisibilityNodeAST visibility, std::string_view name, std::unordered_map<std::string_view, FuncAST> methods, std::string_view file, NodeAST node) : TypeDeclAST(visibility, name, std::move(methods), file, node) {}
};