#pragma once
#include "RedyLexer.h"
#include "AST.h"

class RedyParser {
public:
	FuncAST Parse(std::string_view input);

	RedyParser() : m_lexer(CreateRedyLexer("")) {}
private:
	ExprPtr ParseDouble();
	ExprPtr ParsePrimary();
	ExprPtr ParseArgs(ExprPtr expr);
	ExprPtr ParsePostfix(ExprPtr expr);
	ExprPtr ParsePostfix();
	ExprPtr ParseUnary();
	ExprPtr ParseExpr(int precedence = 1);
	TypeAST ParseType();
	FuncVisibility ParseFuncVisibility();
	std::vector<ParamAST> ParseParams();
	ProtoAST ParseProto();
	FuncAST ParseFunc();

	RedyLexer m_lexer;
};