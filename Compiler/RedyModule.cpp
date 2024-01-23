#include "RedyModule.h"
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h"
#include "Logger.h"
#include <format>
using namespace llvm;

TypeDeclAST* RedyModule::GetType(TypeAST typeAST, LLVMContext& context) {
	auto type = m_typeDecls.find(typeAST);
	if (type == m_typeDecls.end()) {
		Logger::Error(std::format("The type {0} doesn't exist.", type->second->Name));
		return UnknownType;
	}

	if (type->second->LLVMType == nullptr) {
		type->second->GenLLVMType(context, *this);
	}
	return type->second.get();
}

void RedyModule::AddType(TypeAST typeAST, std::unique_ptr<TypeDeclAST> typeDecl) {
	m_typeDecls.emplace(typeAST, std::move(typeDecl));
}

void RedyModule::AddFunc(std::string_view name, FuncAST func) {
	m_funcs.emplace(name, std::move(func));
}

FuncAST* RedyModule::GetFunc(std::string_view name) {
	auto func = m_funcs.find(name);
	if (func == m_funcs.end()) {
		return nullptr;
	}
	return &func->second;
}