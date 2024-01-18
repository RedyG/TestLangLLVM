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
	std::unordered_map<TypeAST, std::unique_ptr<TypeDeclAST>> TypeDecls;

	void TypeCheck(llvm::LLVMContext& context);
	void CodeGen(CodeGenCtx ctx);
	void Register(llvm::Module& module);
	TypeDeclAST* GetType(TypeAST typeAST, llvm::LLVMContext& context);
	void AddType(TypeAST typeAST, std::unique_ptr<TypeDeclAST> typeDecl);

	RedyModule(std::unordered_map<TypeAST, std::unique_ptr<TypeDeclAST>> typeDecls);
	RedyModule(const RedyModule&) = delete;
};