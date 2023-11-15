#pragma once

class ExprAST {

};

class DoubleAST : public ExprAST {
public:
	double Value;

	DoubleAST(double value) : Value(value) {}
};