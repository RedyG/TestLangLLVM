#include "TypeTable.h"
#include <format>
#include "Logger.h"

std::map<TypeAST, TypeDeclAST*, TypeASTComparer> typeDeclMap;

namespace TypeTable {
	TypeDeclAST* GetTypeDecl(TypeAST type) {
		auto decl = typeDeclMap.find(type);
		if (decl == typeDeclMap.end()) {
			Logger::Error(std::format("The type {0} doesn't exist.", type.Name));
			return nullptr;
		}

		return decl->second;
	}

	void AddModuleTypes(ModuleAST& module) {
		for (auto& structAST : module.Structs) {
			typeDeclMap.insert({ TypeAST(structAST.TypeDecl.Name), &structAST.TypeDecl });
		}
	}

	void AddTypeDecl(TypeAST type, TypeDeclAST* typeDecl) {
		typeDeclMap.insert({ type, typeDecl });
	}
}