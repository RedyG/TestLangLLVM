#include "RedyParser.h"
#include <map>
#include <optional>

struct Operator {
	int Precedence;
	bool LeftAssociative;

	Operator(int precedence, bool leftAssociative)
		: Precedence(precedence), LeftAssociative(leftAssociative) {}
};

std::map<TokenType, Operator> operators = {
	{ TokenType::Add, Operator(1, true)},
	{ TokenType::Sub, Operator(1, true)},
	{ TokenType::Mul, Operator(2, true)},
	{ TokenType::Div, Operator(2, true)},
};

std::optional<Operator> GetOperator(TokenType type) {
	auto it = operators.find(type);
	return it == operators.end() ? std::nullopt : std::make_optional(it->second);
}


std::unique_ptr<ExprAST> RedyParser::ParseAtom() {
	return nullptr;
}

std::unique_ptr<ExprAST> RedyParser::ParseExpr(int precedence) {
	auto expr = ParseAtom();
	while (true) {
		auto op = GetOperator(m_lexer.Current().Type);
		if (!op || op.value().Precedence < precedence)
			break;
		auto type = m_lexer.Current().Type;
		m_lexer.Consume();
		expr = std::make_unique<BinOpExprAST>(
			std::move(expr), type,
			std::move(ParseExpr(op.value().Precedence + (int)op.value().LeftAssociative))
		);
	}
	return std::move(expr);
}

std::unique_ptr<ExprAST> RedyParser::ParseExpr() {
	return ParseExpr(1);
}


std::unique_ptr<ExprAST> RedyParser::Parse(std::string_view input) {
	m_lexer = CreateRedyLexer(input);

	return std::move(ParseExpr(1));
}
