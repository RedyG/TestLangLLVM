#include "ReturnStatement.h"
#include "ExprAST.h"

bool ReturnStatement::IsBranch() {
	return true;
}

ReturnStatement::ReturnStatement(ExprPtr expr, NodeAST node) : Expr(std::move(expr)), StatementAST(node) {}