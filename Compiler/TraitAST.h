#pragma once
#include "TypeDeclAST.h"
#include "FuncAST.h"

struct TraitAST : public TypeDeclAST {
public:
	std::unordered_map<std::string_view, ProtoAST> RequiredProtos;

	void CodeGen(CodeGenCtx ctx);

	void GenLLVMType(llvm::LLVMContext& context, RedyModule& module) override;

	TraitAST(std::unordered_map<std::string_view, ProtoAST> requiredProtos, VisibilityAST visibility, std::string_view name, std::unordered_map<std::string_view, FuncAST> methods)
		: TypeDeclAST(visibility, name, std::move(methods)), RequiredProtos(std::move(requiredProtos)) {}
};