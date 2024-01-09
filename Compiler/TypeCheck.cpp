// pass to add type check and add symbols to ast nodes

#include "AST.h"
#include "Logger.h"
#include <format>
#include "TypeTable.h"
#include <stack>
#include "TypeDeclAST.h"

std::vector<VariableDeclStatement*> symbols;
std::stack<int> symbolsCount;
ExprType ReturnType = UnknownType;

VariableDeclStatement* FindSymbol(std::string_view name) {
	for (unsigned int i = symbols.size() - 1; i != -1; i--) {
		auto symbol = symbols[i];
		if (symbol->Variable.Name == name) {
			return symbol;
		}
	}

	return nullptr;
}

ExprType IntExpr::OnTypeCheck() {
	return TypeTable::GetExprType(TypeAST("i32"));
}

ExprType FloatExpr::OnTypeCheck() {
	return TypeTable::GetExprType(TypeAST("f64"));
}

std::string_view GetOpName(TokenType op) {
	switch (op) {
		case TokenType::Add: return "add";
		case TokenType::Div: return "divide";
		case TokenType::Mul: return "multiply";
		case TokenType::Sub: return "substract";
	}
}

ExprType BinOpExpr::OnTypeCheck() {
	auto lhs = LHS->TypeCheck();
	auto rhs = RHS->TypeCheck();

	if (lhs.IsUnknown() || rhs.IsUnknown()) {
		return UnknownType;
	}

	if (lhs != rhs) {
		Logger::Error(std::format("Couldn't {0} values of type {1} and {2}", GetOpName(Op), lhs.Decl->Name, rhs.Decl->Name));
		return UnknownType;
	}

	return lhs;
}

ExprType VariableExpr::OnTypeCheck() {
	Symbol = FindSymbol(Name);
	if (Symbol == nullptr) {
		Logger::Error(std::format("Tried to use undeclared variable {0}", Name));
		return UnknownType;
	}
	return TypeTable::GetExprType(Symbol->Variable.Type);
}

ExprType CallExpr::OnTypeCheck() {
	auto callee = Callee->TypeCheck();
	return UnknownType; // todo
}

ExprType UnaryExpr::OnTypeCheck() {
	auto type = Expr->TypeCheck();
	if (type != TypeTable::GetExprType(TypeAST("bool"))) {
		Logger::Error(std::format("Operator ! expected operand of type bool and got type {0}", type.Name));
	}
	return type;
}

void FuncAST::TypeCheck() {
	ReturnType = TypeTable::GetExprType(Proto.Type);

	for (auto& symbol : Proto.Params) {
		symbols.push_back(&symbol);
	}

	if (std::holds_alternative<ExprPtr>(Body)) {
		auto& expr = std::get<ExprPtr>(Body);
		auto bodyType = expr->TypeCheck();

		if (bodyType.IsUnknown())
			return;

		if (bodyType != ReturnType)
			Logger::Error(std::format("Expected type {0} and got type {1}", ReturnType.Name, bodyType.Name));
	} else {
		auto& block = std::get<std::unique_ptr<BlockStatement>>(Body);
		block->TypeCheckStatement();
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

void VariableDeclStatement::TypeCheckStatement() {
	symbols.push_back(this);
	symbolsCount.top()++;
}

void BlockStatement::TypeCheckStatement() {
	symbolsCount.push(0);

	for (auto& statement : Statements) {
		statement->TypeCheckStatement();
	}

	symbols.resize(symbols.size() - symbolsCount.top());
	symbolsCount.pop();
}

void ReturnStatement::TypeCheckStatement() {
	auto exprType = Expr->TypeCheck();
	if (exprType.IsUnknown())
		return;

	if (exprType != ReturnType)
		Logger::Error(std::format("Expected type {0} and got type {1}", ReturnType.Name, exprType.Name));
}
