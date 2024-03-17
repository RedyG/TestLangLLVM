#pragma once
#include "CodeGenCtx.h"
#include "NodeAST.H"
class TypeDeclAST;
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h"
#include "TypeCheckCtx.h"

class ExprAST {
public:
	NodeAST Node;
	TypeDeclAST* TypeDecl = nullptr;

	// not using the visitor pattern because every passes return different types
	// so I would have to deal with std::any or something

	virtual llvm::Value* CodeGen(CodeGenCtx ctx) = 0;

	// Typechecks the expression and caches the type
	TypeDeclAST* TypeCheck(TypeCheckCtx ctx);

	ExprAST(NodeAST Node);

private:
	virtual TypeDeclAST* OnTypeCheck(TypeCheckCtx ctx) = 0;
};