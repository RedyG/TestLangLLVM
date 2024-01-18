#include "RedyModule.h"
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h"
#include "Logger.h"
#include <format>
using namespace llvm;

TypeDeclAST* RedyModule::GetType(TypeAST typeAST, LLVMContext& context) {
	auto type = TypeDecls.find(typeAST);
	if (type == TypeDecls.end()) {
		Logger::Error(std::format("The type {0} doesn't exist.", type->second->Name));
		return UnknownType;
	}

	if (type->second->LLVMType == nullptr) {
		type->second->GenLLVMType(context, *this);
	}
	return type->second.get();
}

void RedyModule::AddType(TypeAST typeAST, std::unique_ptr<TypeDeclAST> typeDecl) {
	TypeDecls.insert(std::move(std::make_pair( typeAST, std::move(typeDecl) )));
}

RedyModule::RedyModule(std::unordered_map<TypeAST, std::unique_ptr<TypeDeclAST>> typeDecls) : TypeDecls(std::move(typeDecls)) {}