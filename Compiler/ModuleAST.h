#pragma once
#include <vector>
#include "StructAST.h"
#include "CodeGenCtx.h"
namespace llvm {
	class Module;
}

class ModuleAST {
public:
	std::vector<std::unique_ptr<TypeDeclAST>> TypeDecls;

	void TypeCheck(llvm::LLVMContext& context);
	void CodeGen(CodeGenCtx ctx);
	void Register(llvm::Module& module);

	ModuleAST(std::vector<std::unique_ptr<TypeDeclAST>> typeDecls) : TypeDecls(std::move(typeDecls)) {}
};