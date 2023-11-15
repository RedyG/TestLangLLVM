#pragma once
#include "RedyLexer.h"
#include "AST.h"

class RedyParser {
public:
	std::unique_ptr<ExprAST> Parse(std::string_view input);

	RedyParser() : m_lexer(CreateRedyLexer("")) {}
private:
	std::unique_ptr<ExprAST> ParseExpr(int precedence);
	std::unique_ptr<ExprAST> ParseExpr();
	std::unique_ptr<ExprAST> ParseAtom();

	RedyLexer m_lexer;
};