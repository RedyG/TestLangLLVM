#pragma once
#include "RedyLexer.h"
#include "AST.h"

class RedyParser {
public:
	ModuleAST Parse(std::string_view input);

	RedyParser() : m_lexer(CreateRedyLexer("")) {}
private:
	ExprPtr ParseInt();
	ExprPtr ParseDouble();
	ExprPtr ParsePrimary();
	ExprPtr ParseArgs(ExprPtr expr);
	ExprPtr ParsePostfix(ExprPtr expr);
	ExprPtr ParsePostfix();
	ExprPtr ParseUnary();
	ExprPtr ParseExpr(int precedence = 1);
	StatementPtr ParseStatement();
	TypeAST ParseType();
	VisibilityAST ParseVisibility();
	std::vector<VariableAST> ParseParams();
	void ParseMembers(std::vector<FieldAST>& fields, std::vector<FuncAST>& funcs, std::vector<ProtoAST>& protos);
	StructAST ParseStruct();

	RedyLexer m_lexer;
};