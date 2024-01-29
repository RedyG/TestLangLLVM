#pragma once
#include <vector>
#include "StructAST.h"
struct CodeGenCtx;
class TypeDeclAST;
#include "UnknownType.h"
namespace llvm {
	class Module;
	class LLVMContext;
}

class RedyModule {
public:
	void TypeCheck(llvm::LLVMContext& context);
	void CodeGen(CodeGenCtx ctx);
	void Register(llvm::Module& module);
	TypeDeclAST* GetPubType(TypeAST typeAST, llvm::LLVMContext& context);
	TypeDeclAST* GetType(TypeAST typeAST, llvm::LLVMContext& context);
	void AddType(TypeAST typeAST, std::unique_ptr<TypeDeclAST> typeDecl);
	void AddFunc(std::string_view name, FuncAST func);
	FuncAST* GetPubFunc(std::string_view name);
	FuncAST* GetFunc(std::string_view name);

private:
	std::unordered_map<TypeAST, std::unique_ptr<TypeDeclAST>> m_typeDecls;
	std::unordered_map<std::string_view, FuncAST> m_funcs;

	std::unordered_map<std::string_view, FuncAST*> m_importedFuncs;
	std::unordered_map<TypeAST, TypeDeclAST*> m_importedTypes;
};