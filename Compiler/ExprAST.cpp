#include "ExprAST.h"
#include "TypeDeclAST.h"

TypeDeclAST* ExprAST::TypeCheck(RedyModule& module, llvm::LLVMContext& context) {
	TypeDecl = OnTypeCheck(module, context);
	return TypeDecl;
}

