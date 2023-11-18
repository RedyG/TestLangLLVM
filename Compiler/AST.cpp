#include "AST.h"

double DoubleExprAST::CodeGen() {
	std::cout << "inside double\n";

	return Value;
}

double BinOpExprAST::CodeGen() {
	std::cout << "inside BinOp " << (int)Op << '\n';

	auto lhs = LHS->CodeGen();
	auto rhs = RHS->CodeGen();

	switch (Op) {
		case TokenType::Add: return lhs + rhs;
		case TokenType::Sub: return lhs - rhs;
		case TokenType::Mul: return lhs * rhs;
		case TokenType::Div: return lhs / rhs;
		default: throw std::exception("invalid op");
	}
}

double VariableExprAST::CodeGen() {
	std::cout << "inside Variable\n";


	throw std::exception("no var yet");
}

double CallExprAST::CodeGen() {
	std::cout << "inside Call\n";

	return Callee->CodeGen();

	//throw std::exception("no call yet");
}

double UnaryExprAST::CodeGen() {
	std::cout << "inside Unary\n";

	return -Expr->CodeGen();
}