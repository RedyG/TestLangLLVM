#pragma once
#include "VisibilityAST.h"
#include "TypeAST.h"
#include "ParamAST.h"
namespace llvm {
	class Module;
}

class ProtoAST {
public:
	NodeAST Node;
	VisibilityNodeAST Visibility;
	TypeAST ReturnType;
	TypeDeclAST* ReturnTypeDecl;
	std::string_view Name;
	std::string_view File;
	std::vector<ParamAST> Params;

	ProtoAST(VisibilityNodeAST visibility, TypeAST type, std::string_view name, std::vector<ParamAST> params, std::string_view file, NodeAST node)
		: Visibility(visibility), ReturnType(type), Name(name), Params(std::move(params)), File(file), Node(node) {}
};