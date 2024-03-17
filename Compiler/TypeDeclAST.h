#pragma once
#include <string>
#include <vector>
#include "NodeAST.h"
#include "VisibilityAST.h"
#include "FuncAST.h"
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h"

class TypeDeclAST {
public:
	NodeAST Node;
	std::unordered_map<std::string_view, FuncAST> Methods;
	llvm::Type* LLVMType = nullptr;
	VisibilityNodeAST Visibility;
	std::string_view Name;
	std::string_view File;

	TypeDeclAST(VisibilityNodeAST visibility, std::string_view name, std::unordered_map<std::string_view, FuncAST> methods, std::string_view file, NodeAST node);

	bool IsUnknown();

	FuncAST* GetMethod(std::string_view name);

	virtual void GenLLVMType(llvm::LLVMContext& context, RedyModule& module) = 0;
};