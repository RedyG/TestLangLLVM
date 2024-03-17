#include "RedyModule.h"
#include "BuiltInTypes.h"
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h"
#include "Logger.h"
#include <format>
using namespace llvm;

TypeDeclAST* RedyModule::GetType(TypeAST typeAST, LLVMContext& context) {
	auto builtInType = BuiltInTypes::Get(typeAST.Name);
	if (builtInType != nullptr)
		return builtInType;

	auto importedType = m_importedTypes.find(typeAST);
	if (importedType != m_importedTypes.end()) {
		return importedType->second;
	}

	auto type = m_typeDecls.find(typeAST);
	if (type != m_typeDecls.end()) {
		if (type->second->LLVMType == nullptr) {
			type->second->GenLLVMType(context, *this);
		}
		return type->second.get();
	}

	Logger::Error(std::format("The type {0} doesn't exist.", type->second->Name));
	return nullptr;
}


TypeDeclAST* RedyModule::GetPubType(TypeAST typeAST, LLVMContext& context, bool error) {
	auto type = m_typeDecls.find(typeAST);
	if (type == m_typeDecls.end()) {
		if (error)
			Logger::Error(std::format("The type {0} doesn't exist.", (std::string_view)typeAST));
		return nullptr;
	}
	if (type->second->Visibility.Value != VisibilityAST::Public) {
		if (error)
			Logger::Error(std::format("The type \"{}\" is private.", (std::string_view)typeAST));
		return nullptr;
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
	auto importedFunc = m_importedFuncs.find(name);

	if (importedFunc != m_importedFuncs.end()) {
		return importedFunc->second;
	}

	auto func = m_funcs.find(name);
	if (func != m_funcs.end()) {
		return &func->second;
	}

	Logger::Error(std::format("The function \"{}\" does not exist.", name));
	return nullptr;
}

FuncAST* RedyModule::GetPubFunc(std::string_view name, bool error) {
	auto func = m_funcs.find(name);
	if (func == m_funcs.end()) {
		if (error)
			Logger::Error(std::format("The function \"{}\" does not exist.", name));
		return nullptr;
	}
	if (func->second.Proto.Visibility.Value != VisibilityAST::Public) {
		if (error)
			Logger::Error(std::format("The function \"{}\" is private.", name));
		return nullptr;
	}
	return &func->second;
}

RedyModule* RedyModule::GetChild(std::string_view name, bool error) {
	auto module = m_children.find(name);
	if (module == m_children.end()) {
		if (error)
			Logger::Error(std::format("The module \"{}\" does not exist.", name));
		return nullptr;
	}
	return &module->second;
}