#pragma once
#include "RedyLexer.h"
#include "AST.h"

class RedyParser {
public:
	RedyModule Parse(std::string_view input);

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
	std::variant<FuncAST, FieldAST, ProtoAST> ParseMember();
	std::unique_ptr<StructAST> ParseStruct();

	RedyLexer m_lexer;
};   



/*

class AGAF : fwaiojfw, wfawfw ,fawf {
 : 
}

extend AGAF : 8fiawf, fwaf {
	
}

*/