#pragma once
#include <vector>
#include "StructAST.h"
#include "CodeGenCtx.h"
namespace llvm {
	class Module;
}

class ModuleAST {
public:
	std::vector<StructAST> Structs;

	void TypeCheck();
	void CodeGen(CodeGenCtx ctx);
	void Register(llvm::Module& module);

	ModuleAST(std::vector<StructAST> structs) : Structs(std::move(structs)) {}
};