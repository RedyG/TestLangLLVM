#pragma once
#include "TypeDeclAST.h"
#include "FuncAST.h"

struct TraitAST : public TypeDeclAST {
public:
	std::unordered_map<std::string_view, ProtoAST> RequiredProtos;

	void CodeGen(CodeGenCtx ctx);

	void GenLLVMType(llvm::LLVMContext& context, RedyModule& module) override;

	TraitAST(std::unordered_map<std::string_view, ProtoAST> requiredProtos, VisibilityNodeAST visibility, std::string_view name, std::unordered_map<std::string_view, FuncAST> methods, std::string_view file, NodeAST node)
		: TypeDeclAST(visibility, name, std::move(methods), file, node), RequiredProtos(std::move(requiredProtos)) {}
};