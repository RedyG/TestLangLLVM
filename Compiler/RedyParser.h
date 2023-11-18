#pragma once
#include "RedyLexer.h"
#include "AST.h"

class RedyParser {
public:
	ExprPtr Parse(std::string_view input);

	RedyParser() : m_lexer(CreateRedyLexer("")) {}
private:
	ExprPtr ParseDouble();
	ExprPtr ParsePrimary();
	ExprPtr ParseArgs(ExprPtr expr);
	ExprPtr ParsePostfix(ExprPtr expr);
	ExprPtr ParsePostfix();
	ExprPtr ParseUnary();
	ExprPtr ParseExpr(int precedence = 1);

	RedyLexer m_lexer;
};