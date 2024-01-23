#pragma once
#include <string>
#include <vector>
#include "VisibilityAST.h"
#include "FuncAST.h"
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h"

class TypeDeclAST {
public:
	std::unordered_map<std::string_view, FuncAST> Methods;
	llvm::Type* LLVMType = nullptr;
	VisibilityAST Visibility;
	std::string_view Name;

	TypeDeclAST(VisibilityAST visibility, std::string_view name, std::unordered_map<std::string_view, FuncAST> methods);

	bool IsUnknown();

	FuncAST* GetMethod(std::string_view name);

	virtual void GenLLVMType(llvm::LLVMContext& context, RedyModule& module) = 0;
};