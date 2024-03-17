#pragma once
#include "RedyLexer.h"
#include "AST.h"
#include "TraitAST.h"

class RedyParser {
public:
	RedyModule ParseFile(Project* project, std::string file);
	RedyModule Parse(Project* project, std::string input, std::string file);

	RedyParser() : m_lexer(CreateRedyLexer("")), m_file("") {}
private:
	ExprPtr ParseInt(NodeAST node);
	ExprPtr ParseDouble(NodeAST node);
	ExprPtr ParsePrimary();
	ExprPtr ParseArgs(ExprPtr expr);
	ExprPtr ParsePostfix(ExprPtr expr);
	ExprPtr ParsePostfix();
	ExprPtr ParseUnary();
	std::variant<ExprPtr, StatementPtr> ParseExprOrStatement(int precedence);
	ExprPtr ParseExpr(int precedence = 1);
	StatementPtr ParseStatement();
	std::optional<TypeAST> ParseType();
	TypeAST ParseTypeUnwrap();
	VisibilityNodeAST ParseVisibility();
	std::vector<ParamAST> ParseParams();
	RedyModule ParseDecls(Project* project);
	UseDeclAST ParseUseDecl();
	StructAST ParseStruct(VisibilityNodeAST visibility);
	TraitAST ParseTrait(VisibilityNodeAST visibility);
	std::variant<FuncAST, FieldAST, ProtoAST> ParseMember();

	std::string_view m_file;
	RedyLexer m_lexer;
};   
