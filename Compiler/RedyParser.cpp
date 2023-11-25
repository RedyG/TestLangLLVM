#include "RedyParser.h"
#include <map>
#include <optional>
#include <string>


std::string EraseChar(std::string_view str, char c) {
	std::string result;
	for (size_t i = 0; i < str.size(); i++)
	{
		char currentChar = str[i];
		if (currentChar != c)
			result += currentChar;
	}
	return std::move(result);
}
namespace Compiler {
	struct Operator {
		int Precedence;
		bool LeftAssociative;

		Operator(int precedence, bool leftAssociative)
			: Precedence(precedence), LeftAssociative(leftAssociative) {}
	};
}

std::map<TokenType, Compiler::Operator> operators = {
	{ TokenType::Add, Compiler::Operator(1, true)},
	{ TokenType::Sub, Compiler::Operator(1, true)},
	{ TokenType::Mul, Compiler::Operator(2, true)},
	{ TokenType::Div, Compiler::Operator(2, true)},
};

bool IsUnary(TokenType type) {
	return type == TokenType::Sub;
}

std::optional<Compiler::Operator> GetOperator(TokenType type) {
	auto it = operators.find(type);
	return it == operators.end() ? std::nullopt : std::make_optional(it->second);
}


ExprPtr RedyParser::ParseDouble() {
	auto content = m_lexer.Current().Content;
	m_lexer.Consume();
	if (content.contains('_')) { // checking first because most numbers won't contain _
		return std::make_unique<DoubleExpr>(std::stod(EraseChar(content, '_')));
	}
	return std::make_unique<DoubleExpr>(std::stod(std::string(content)));
}

// This method might return nullptr
ExprPtr RedyParser::ParsePrimary() {
	if (m_lexer.Current().Type == TokenType::Double) {
		return std::move(ParseDouble());
	}
	else if (m_lexer.Current().Type == TokenType::Identifier)
	{
		auto name = m_lexer.Current().Content;
		m_lexer.Consume();
		return std::make_unique<VariableExpr>(name);
	}
	else if (m_lexer.ConsumeIf(TokenType::LParen)) {
		auto expr = ParseExpr();
		if (m_lexer.ConsumeIf(TokenType::RParen)) {
			return std::move(expr);
		}
	}
	
	return nullptr;
}

ExprPtr RedyParser::ParseArgs(ExprPtr expr) {
	m_lexer.Consume();
	std::vector<ExprPtr> params {};

	if (m_lexer.ConsumeIf(TokenType::RParen)){
		return std::make_unique<CallExpr>(std::move(expr), std::move(params));
	}

	do {
		params.push_back(std::move(ParseExpr()));

		if (m_lexer.ConsumeIf(TokenType::RParen)) {
			return std::make_unique<CallExpr>(std::move(expr), std::move(params));
		}
	} while (m_lexer.ConsumeIf(TokenType::Comma));

	throw std::exception("couldn't parse args");
}

ExprPtr RedyParser::ParsePostfix(ExprPtr expr) {
	if (m_lexer.Current().Type == TokenType::LParen) {
		return std::move(ParsePostfix(std::move(ParseArgs(std::move(expr)))));
	}

	return std::move(expr);
}

ExprPtr RedyParser::ParsePostfix() {
	return std::move(ParsePostfix(std::move(ParsePrimary())));
}

ExprPtr RedyParser::ParseUnary() {
	if (IsUnary(m_lexer.Current().Type)) {
		auto type = m_lexer.Current().Type;
		m_lexer.Consume();
		auto expr = ParsePostfix();
		return std::make_unique<UnaryExpr>(type, std::move(expr));
	}

	return std::move(ParsePostfix());
}

ExprPtr RedyParser::ParseExpr(int precedence) {
	auto expr = ParseUnary();
	while (true) {
		auto op = GetOperator(m_lexer.Current().Type);
		if (!op || op.value().Precedence < precedence)
			break;
		auto type = m_lexer.Current().Type;
		m_lexer.Consume();
		expr = std::make_unique<BinOpExpr>(
			std::move(expr), type,
			std::move(ParseExpr(op.value().Precedence + (int)op.value().LeftAssociative))
		);
	}
	return std::move(expr);
}

TypeAST RedyParser::ParseType() {
	if (m_lexer.Current().Type == TokenType::Identifier) {
		auto name = m_lexer.Current().Content;
		m_lexer.Consume();
		return TypeAST(name);
	}

	throw std::exception("couldn't parse type");
}

VisibilityAST RedyParser::ParseVisibility() {
	if (m_lexer.ConsumeIf(TokenType::Pub)) {
		return VisibilityAST::Public;
	}
	return VisibilityAST::Private;
}

std::vector<VariableAST> RedyParser::ParseParams() {
	std::vector<VariableAST> params {};

	if (m_lexer.ConsumeIf(TokenType::RParen)) {
		return params;
	}

	do {
		auto type = ParseType();
		if (m_lexer.Current().Type == TokenType::Identifier) {
			auto name = m_lexer.Current().Content;
			m_lexer.Consume();
			params.emplace_back(type, name);
		}
		
		if (m_lexer.ConsumeIf(TokenType::RParen)) {
			return params;
		}
	} while (m_lexer.ConsumeIf(TokenType::Comma));

	throw std::exception("couldn't parse params");
}

void RedyParser::ParseMembers(std::vector<FieldAST>& fields, std::vector<FuncAST>& funcs, std::vector<ProtoAST>& protos) {
	while (true) {

		if (m_lexer.ConsumeIf(TokenType::RCurly))
			return;

		auto visibility = ParseVisibility();
		auto type = ParseType();
		if (m_lexer.Current().Type == TokenType::Identifier) {
			auto name = m_lexer.Current().Content;
			m_lexer.Consume();
			if (m_lexer.ConsumeIf(TokenType::LParen)) {
				auto params = ParseParams();
				auto proto = ProtoAST(visibility, type, name, std::move(params));
				if (m_lexer.ConsumeIf(TokenType::SemiColon)) {
					protos.push_back(std::move(proto));
				}
				else {
					auto body = ParseExpr();
					if (m_lexer.ConsumeIf(TokenType::SemiColon)) {
						funcs.emplace_back(std::move(proto), std::move(body));
					}
				}
			}
			else if (m_lexer.ConsumeIf(TokenType::SemiColon)) {
				fields.emplace_back(visibility, VariableAST(type, name));
			}
		}
	}
}

std::vector<ProtoAST> protos;

StructAST RedyParser::ParseStruct() {
	auto visibility = ParseVisibility();
	if (m_lexer.ConsumeIf(TokenType::Struct)) {
		if (m_lexer.Current().Type == TokenType::Identifier) {
			auto name = m_lexer.Current().Content;;
			m_lexer.Consume();
			if (m_lexer.ConsumeIf(TokenType::LCurly)) {
				std::vector<FieldAST> fields;
				std::vector<FuncAST> methods;
				ParseMembers(fields, methods, protos);
				if (protos.size() != 0) {
					protos.clear();
					throw std::exception("Struct methods should have a body.");
				}

				return StructAST(visibility, name, std::move(fields), std::move(methods));	
			}
		}
	}

	throw std::exception("invalid struct definition");
}

StructAST RedyParser::Parse(std::string_view input) {
	m_lexer = CreateRedyLexer(input);
	m_lexer.Consume();
	return std::move(ParseStruct());
}

/*

AST {
	Accept(Visitor visitor);
}

Expr {
	Accept(Visitor visitor) => visitor.visitExthis);
}

ASTVisitor {
}

*/