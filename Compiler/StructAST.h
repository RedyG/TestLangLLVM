#pragma once
#include "TypeDeclAST.h"
#include "FieldAST.h"
#include <vector>
namespace llvm {
	class Module;
}

class StructAST {
public:
	TypeDeclAST TypeDecl;
	std::vector<FieldAST> Fields;

	void TypeCheck();
	void CodeGen(CodeGenCtx ctx);
	void Register(llvm::Module& module);

	StructAST(std::vector<FieldAST> fields, TypeDeclAST typeDecl)
		: Fields(std::move(fields)), TypeDecl(std::move(typeDecl)) {}
};