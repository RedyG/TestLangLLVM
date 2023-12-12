#pragma once
#include <map>
#include "AST.h"

namespace TypeTable {
	TypeDeclAST* GetTypeDecl(TypeAST type);
	void AddModuleTypes(ModuleAST& module);
	void AddTypeDecl(TypeAST type, TypeDeclAST* typeDecl);
}