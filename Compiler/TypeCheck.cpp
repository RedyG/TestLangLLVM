// pass to add type check and add symbols to ast nodes

#include "AST.h"
#include "Logger.h"
#include <format>
#include "TypeTable.h"
#include <stack>

std::vector<VariableAST*> symbols;
std::stack<int> symbolsCount;
TypeDeclAST* ReturnType;

VariableAST* FindSymbol(std::string_view name) {
	for (unsigned int i = symbols.size() - 1; i != -1; i--) {
		auto symbol = symbols[i];
		if (symbol->Name == name) {
			return symbol;
		}
	}

	return nullptr;
}

TypeDeclAST* DoubleExpr::OnTypeCheck() {
	return TypeTable::GetTypeDecl(TypeAST("f64"));
}

std::string_view GetOpName(TokenType op) {
	switch (op) {
		case TokenType::Add: return "add";
		case TokenType::Div: return "divide";
		case TokenType::Mul: return "multiply";
		case TokenType::Sub: return "substract";
	}
}

TypeDeclAST* BinOpExpr::OnTypeCheck() {
	auto lhs = LHS->TypeCheck();
	auto rhs = RHS->TypeCheck();

	if (lhs == nullptr || rhs == nullptr) {
		return nullptr;
	}

	if (lhs != rhs) {
		Logger::Error(std::format("Couldn't {0} values of type {1} and {2}", GetOpName(Op), lhs->Name, rhs->Name));
		return nullptr;
	}

	return lhs;
}

TypeDeclAST* VariableExpr::OnTypeCheck() {
	auto symbol = FindSymbol(Name);
	if (symbol == nullptr) {
		Logger::Error(std::format("Tried to use undeclared variable {0}", Name));
		return nullptr;
	}
	return TypeTable::GetTypeDecl(symbol->Type);
}

TypeDeclAST* CallExpr::OnTypeCheck() {
	auto callee = Callee->TypeCheck();
	return nullptr; // todo
}

TypeDeclAST* UnaryExpr::OnTypeCheck() {
	auto type = Expr->TypeCheck();
	if (type != TypeTable::GetTypeDecl(TypeAST("bool"))) {
		Logger::Error(std::format("Operator ! expected operand of type bool and got type {0}", type->Name));
	}
	return type;
}

void FuncAST::TypeCheck() {
	ReturnType = TypeTable::GetTypeDecl(Proto.Type);

	for (auto& symbol : Proto.Params) {
		symbols.push_back(&symbol);
	}

	if (std::holds_alternative<ExprPtr>(Body)) {
		auto& expr = std::get<ExprPtr>(Body);
		auto bodyType = expr->TypeCheck();

		if (bodyType == nullptr)
			return;

		if (bodyType != ReturnType)
			Logger::Error(std::format("Expected type {0} and got type {1}", ReturnType->Name, bodyType->Name));
	} else {
		auto& block = std::get<std::unique_ptr<BlockStatement>>(Body);
		block->TypeCheck();
	}

	symbols.resize(symbols.size() - Proto.Params.size());
}

void StructAST::TypeCheck() {
	for (auto& func : TypeDecl.Methods) {
		func.TypeCheck();
	}
}

void ModuleAST::TypeCheck() {
	for (auto& structAST : Structs) {
		structAST.TypeCheck();
	}
}

void VariableDeclStatement::TypeCheck() {
	symbols.push_back(&Variable);
	symbolsCount.top()++;
}

void BlockStatement::TypeCheck() {
	symbolsCount.push(0);

	for (auto& statement : Statements) {
		statement->TypeCheck();
	}

	symbols.resize(symbols.size() - symbolsCount.top());
	symbolsCount.pop();
}

void ReturnStatement::TypeCheck() {
	auto exprType = Expr->TypeCheck();
	if (exprType == nullptr)
		return;

	if (exprType != ReturnType)
		Logger::Error(std::format("Expected type {0} and got type {1}", ReturnType->Name, exprType->Name));
}
