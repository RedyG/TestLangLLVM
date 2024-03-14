#pragma once
#include "RedyLexer.h"
#include "AST.h"
#include "TraitAST.h"

class RedyParser {
public:
	RedyModule Parse(Project* project, std::string_view input);

	RedyParser() : m_lexer(CreateRedyLexer("")) {}
private:
	ExprPtr ParseInt();
	ExprPtr ParseDouble();
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
	VisibilityAST ParseVisibility();
	std::vector<ParamAST> ParseParams();
	RedyModule ParseDecls(Project* project);
	UseDeclAST ParseUseDecl();
	StructAST ParseStruct(VisibilityAST visibility);
	TraitAST ParseTrait(VisibilityAST visibility);
	std::variant<FuncAST, FieldAST, ProtoAST> ParseMember();

	RedyLexer m_lexer;
};   
