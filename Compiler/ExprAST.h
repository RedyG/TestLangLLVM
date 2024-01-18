#pragma once
#include "UnknownType.h"
#include "CodeGenCtx.h"
class TypeDeclAST;
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h"

class ExprAST {
public:
	TypeDeclAST* TypeDecl = UnknownType;

	// not using the visitor pattern because every passes return different types
	// so I would have to deal with std::any or something
	virtual llvm::Value* CodeGen(CodeGenCtx ctx) = 0;
	TypeDeclAST* TypeCheck(RedyModule& module, llvm::LLVMContext& context);

private:
	virtual TypeDeclAST* OnTypeCheck(RedyModule& module, llvm::LLVMContext& context) = 0;
};