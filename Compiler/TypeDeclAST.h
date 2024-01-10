#pragma once
#include <string>
#include <vector>
#include "VisibilityAST.h"
#include "FuncAST.h"
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h"

class TypeDeclAST {
public:
	VisibilityAST Visibility;
	std::string_view Name;
	std::vector<FuncAST> Methods;

	TypeDeclAST(VisibilityAST visibility, std::string_view name, std::vector<FuncAST> methods)
		: Visibility(visibility), Name(name), Methods(std::move(methods)) {}

	virtual llvm::Type* GenLLVMType(llvm::LLVMContext& context) = 0;
};