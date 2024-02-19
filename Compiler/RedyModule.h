#pragma once
#include <vector>
#include "UseDeclAST.h"
class Project;
#include "StructAST.h"
struct CodeGenCtx;
class TypeDeclAST;
namespace llvm {
	class Module;
	class LLVMContext;
}

class RedyModule {
public:
	std::string_view Name;

	void TypeCheck(llvm::LLVMContext& context);
	void CodeGen(CodeGenCtx ctx);
	void Register(llvm::Module& module);
	TypeDeclAST* GetPubType(TypeAST typeAST, llvm::LLVMContext& context, bool error = true);
	TypeDeclAST* GetType(TypeAST typeAST, llvm::LLVMContext& context);
	void AddType(TypeAST typeAST, std::unique_ptr<TypeDeclAST> typeDecl);
	void AddFunc(std::string_view name, FuncAST func);
	FuncAST* GetPubFunc(std::string_view name, bool error = true);
	FuncAST* GetFunc(std::string_view name);
	RedyModule* GetChild(std::string_view name, bool error = true);
	RedyModule(std::string_view name, std::vector<UseDeclAST> useDecls, Project* project) : Name(name), m_useDecls(std::move(useDecls)), m_project(project) {}

private:
	void RegisterImports(llvm::LLVMContext& context);

	Project* m_project;

	std::vector<UseDeclAST> m_useDecls;

	std::unordered_map<std::string_view, RedyModule> m_children;

	std::unordered_map<TypeAST, std::unique_ptr<TypeDeclAST>> m_typeDecls;
	std::unordered_map<std::string_view, FuncAST> m_funcs;

	std::unordered_map<std::string_view, FuncAST*> m_importedFuncs;
	std::unordered_map<TypeAST, TypeDeclAST*> m_importedTypes;
	std::unordered_map<std::string_view, RedyModule*> m_importedModules;
};