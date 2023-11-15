#pragma once

class ExprAST {

};

class DoubleExprAST : public ExprAST {
public:
	double Value;

	DoubleExprAST(double value) : Value(value) {}
};

class BinOpExprAST : public ExprAST {
public:
	std::unique_ptr<ExprAST> LHS;
	TokenType Op;
	std::unique_ptr<ExprAST> RHS;

	BinOpExprAST(std::unique_ptr<ExprAST> lhs, TokenType op, std::unique_ptr<ExprAST> rhs) : LHS(std::move(lhs)), Op(op), RHS(std::move(rhs)) {}
};