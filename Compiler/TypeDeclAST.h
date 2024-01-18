#pragma once
#include <string>
#include <vector>
#include "VisibilityAST.h"
#include "FuncAST.h"
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h"

class TypeDeclAST {
public:
	llvm::Type* LLVMType = nullptr;
	VisibilityAST Visibility;
	std::string_view Name;
	std::vector<FuncAST> Methods;

	TypeDeclAST(VisibilityAST visibility, std::string_view name, std::vector<FuncAST> methods)
		: Visibility(visibility), Name(name), Methods(std::move(methods)) {}

	bool IsUnknown() {
		return LLVMType == nullptr;
	}

	virtual void GenLLVMType(llvm::LLVMContext& context, RedyModule& module) = 0;
};