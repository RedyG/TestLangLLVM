// pass to add type check and add symbols to ast nodes

#include "AST.h"
#include "Logger.h"
#include <format>
#include "TypeTable.h"
#include <stack>

std::vector<VariableAST*> symbols;
std::stack<int> symbolsCount;

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
	return TypeTable::GetTypeDecl(TypeAST("double"));
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
	for (auto& symbol : Proto.Params) {
		symbols.push_back(&symbol);
	}

	auto bodyType = Body->TypeCheck();

	symbols.resize(symbols.size() - Proto.Params.size());

	auto returnType = TypeTable::GetTypeDecl(Proto.Type);

	if (bodyType == nullptr || returnType == nullptr) {
		return;
	}

	if (bodyType != returnType) {
		Logger::Error(std::format("Excpected type {0} and got type {1}", returnType->Name, bodyType->Name));
	}

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

void VariableDeclaration::TypeCheck() {
	symbols.push_back(&Variable);
	symbolsCount.top()++;
}
