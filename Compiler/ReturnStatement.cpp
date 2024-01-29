#include "ReturnStatement.h"
#include "ExprAST.h"

bool ReturnStatement::IsBranch() {
	return true;
}

ReturnStatement::ReturnStatement(ExprPtr expr) : Expr(std::move(expr)) {}