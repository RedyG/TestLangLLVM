#include "RedyParser.h"
#include <map>
#include <optional>
#include <string>
#include "TypeDeclAST.h"

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

ExprPtr RedyParser::ParseInt() {
	auto content = m_lexer.Current().Content;
	m_lexer.Consume();
	if (content.contains('_')) {
		return std::make_unique<IntExpr>(std::stoi(EraseChar(content, '_')));
	}

	return std::make_unique<IntExpr>(std::stoi(std::string(content)));
}


ExprPtr RedyParser::ParseDouble() {
	auto content = m_lexer.Current().Content;
	m_lexer.Consume();
	if (content.contains('_')) {
		return std::make_unique<FloatExpr>(std::stod(EraseChar(content, '_')));
	}
	return std::make_unique<FloatExpr>(std::stod(std::string(content)));
}

// This method might return nullptr
ExprPtr RedyParser::ParsePrimary() {
	if (m_lexer.Current().Type == TokenType::Float) {
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
	else if (m_lexer.Current().Type == TokenType::Int) {
		return std::move(ParseInt());
	}
	
	return nullptr;
}

ExprPtr RedyParser::ParseArgs(ExprPtr expr) {
	m_lexer.Consume();
	std::vector<ExprPtr> params {};

	if (m_lexer.ConsumeIf(TokenType::RParen)){
		auto res = std::make_unique<CallExpr>(std::move(expr), std::move(params));
		return res;
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

const int StatementPrecendence = -1;

std::variant<ExprPtr, StatementPtr> RedyParser::ParseExprOrStatement(int precedence) {
	if (precedence == StatementPrecendence) {
		if (m_lexer.ConsumeIf(TokenType::LCurly)) {
			std::vector<StatementPtr> statements;
			while (m_lexer.Current().Type != TokenType::RCurly) {
				statements.push_back(std::move(ParseStatement()));
			}
			m_lexer.Consume();
			return std::make_unique<BlockStatement>(std::move(statements));
		}


		if (m_lexer.ConsumeIf(TokenType::Return)) {
			auto expr = ParseExpr();
			if (m_lexer.ConsumeIf(TokenType::SemiColon)) {
				return std::make_unique<ReturnStatement>(std::move(expr));
			}
		}

		if (auto type = ParseType()) {
			if (m_lexer.Current().Type == TokenType::Identifier) {
				auto name = m_lexer.Current().Content;
				m_lexer.Consume();
				if (m_lexer.ConsumeIf(TokenType::Equal)) {
					auto expr = ParseExpr();
					if (m_lexer.ConsumeIf(TokenType::SemiColon)) {
						return std::make_unique<VariableDeclStatement>(std::move(VariableAST(type.value(), name, std::move(expr))));
					}
				}
			}
		}
	}


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
	return expr;
}

ExprPtr RedyParser::ParseExpr(int precedence) {
	return std::move(std::get<ExprPtr>(ParseExprOrStatement(precedence))); // TODO: handle error
}

StatementPtr RedyParser::ParseStatement() {
	return std::move(std::get<StatementPtr>(ParseExprOrStatement(StatementPrecendence))); // TODO: handle error
}

std::optional<TypeAST> RedyParser::ParseType() {
	if (m_lexer.Current().Type == TokenType::Identifier) {
		auto name = m_lexer.Current().Content;
		m_lexer.Consume();
		return std::make_optional(TypeAST(name));
	}

	return std::nullopt;
}

TypeAST RedyParser::ParseTypeUnwrap() {
	if (auto type = ParseType()) {
		return type.value();
	}

	throw std::exception("invalid type parsing");
}

VisibilityAST RedyParser::ParseVisibility() {
	if (m_lexer.ConsumeIf(TokenType::Pub)) {
		return VisibilityAST::Public;
	}
	return VisibilityAST::Private;
}

std::vector<ParamAST> RedyParser::ParseParams() {
	std::vector<ParamAST> params {};

	if (m_lexer.ConsumeIf(TokenType::RParen)) {
		return params;
	}

	do {
		auto type = ParseTypeUnwrap();
		if (m_lexer.Current().Type == TokenType::Identifier) {
			auto name = m_lexer.Current().Content;
			m_lexer.Consume();
			params.emplace_back(std::move(SymbolAST(VariableAST(type, name))));
		}
		
		if (m_lexer.ConsumeIf(TokenType::RParen)) {
			return params;
		}
	} while (m_lexer.ConsumeIf(TokenType::Comma));

	throw std::exception("couldn't parse params");
}

template<typename T, typename S>
std::unique_ptr<T> dynamic_pointer_cast(std::unique_ptr<S>&& p) noexcept
{
	auto converted = std::unique_ptr<T>{ dynamic_cast<T*>(p.get()) };
	if (converted) {
		p.release();            // no longer owns the pointer
	}
	return converted;
}

std::variant<FuncAST, FieldAST, ProtoAST> RedyParser::ParseMember() {
	auto visibility = ParseVisibility();
	auto type = ParseTypeUnwrap();
	if (m_lexer.Current().Type == TokenType::Identifier) {
		auto name = m_lexer.Current().Content;
		m_lexer.Consume();
		if (m_lexer.ConsumeIf(TokenType::LParen)) {
			auto params = ParseParams();
			auto proto = ProtoAST(visibility, type, name, std::move(params));
			if (m_lexer.ConsumeIf(TokenType::SemiColon)) {
				return proto;
			}
			else {
				if (m_lexer.ConsumeIf(TokenType::Arrow)) {
					auto body = ParseExpr();
					if (m_lexer.ConsumeIf(TokenType::SemiColon)) {
						return std::move(FuncAST(std::move(proto), std::move(body)));
					}
				}
				else {
					auto statement = ParseStatement();
					auto block = dynamic_pointer_cast<BlockStatement, StatementAST>(std::move(statement));
					if (block) {
						return std::move(FuncAST(std::move(proto), std::move(block)));
					}
				}
			}
		}
		else if (m_lexer.ConsumeIf(TokenType::SemiColon)) {
			return std::move(FieldAST(visibility, VariableAST(type, name)));
		}
	}
}

RedyModule RedyParser::Parse(std::string_view input) {
	m_lexer = CreateRedyLexer(input);
	m_lexer.Consume();

	RedyModule module;
	
	do {
		auto visibility = ParseVisibility();
		if (m_lexer.ConsumeIf(TokenType::Struct)) {
			if (m_lexer.Current().Type == TokenType::Identifier) {
				auto name = m_lexer.Current().Content;;
				m_lexer.Consume();
				if (m_lexer.ConsumeIf(TokenType::LCurly)) {
					std::vector<FieldAST> fields;
					std::unordered_map<std::string_view, FuncAST> methods;
					while (m_lexer.Current().Type != TokenType::RCurly) {
						auto member = ParseMember();
						if (std::holds_alternative<FieldAST>(member)) {
							fields.emplace_back(std::get<FieldAST>(std::move(member)));
						}
						else if (std::holds_alternative<FuncAST>(member)) {
							auto method = std::move(std::get<FuncAST>(std::move(member)));
							methods.emplace(method.Proto.Name, std::move(method));
						}
						else {
							throw std::exception("Methods in structs must contain bodies");
						}
					}
					m_lexer.Consume();
					module.AddType(name, std::make_unique<StructAST>(std::move(fields), visibility, name, std::move(methods)));
				}
			}
			else {
				throw std::exception("invalid type definition");
			}
		}
		else {
			auto member = ParseMember();
			if (!std::holds_alternative<FuncAST>(member)) {
				throw std::exception("we only support static methods for now bro");
			}
			auto func = std::move(std::get<FuncAST>(std::move(member)));
			func.Proto.Visibility = visibility; // important
			module.AddFunc(func.Proto.Name, std::move(func));
		}
	} while (m_lexer.Current().Type != TokenType::Invalid);

	return module;
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