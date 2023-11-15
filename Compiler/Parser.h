#pragma once
#include "RedyLexer.h"
#include "AST.h"

class Parser {
public:
	std::unique_ptr<ExprAST> Parse(std::string_view input);
private:
	RedyLexer m_lexer;
};