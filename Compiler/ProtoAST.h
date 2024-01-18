#pragma once
#include "VisibilityAST.h"
#include "TypeAST.h"
#include "VariableDeclStatement.h"
namespace llvm {
	class Module;
}

class ProtoAST {
public:
	VisibilityAST Visibility;
	TypeAST Type;
	std::string_view Name;
	std::vector<VariableDeclStatement> Params;

	ProtoAST(VisibilityAST visibility, TypeAST type, std::string_view name, std::vector<VariableDeclStatement> params)
		: Visibility(visibility), Type(type), Name(name), Params(std::move(params)) {}

	void Register(RedyModule& redyModule, llvm::Module& module);
};