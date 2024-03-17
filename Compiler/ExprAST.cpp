#include "ExprAST.h"
#include "TypeDeclAST.h"

TypeDeclAST* ExprAST::TypeCheck(TypeCheckCtx ctx) {
	TypeDecl = OnTypeCheck(ctx);
	return TypeDecl;
}

ExprAST::ExprAST(NodeAST node) : Node(node) {}