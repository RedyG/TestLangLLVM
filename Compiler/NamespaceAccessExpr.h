#pragma once
#include "ExprAST.h"
#include <vector>

class NamespaceAccessExpr : public ExprAST {
public:
	std::vector<std::string_view> Identifiers;

	llvm::Value* CodeGen(CodeGenCtx ctx) override;

private:
	TypeDeclAST* OnTypeCheck(TypeCheckCtx ctx) override;
};
