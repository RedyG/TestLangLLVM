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

ExprType IntExpr::OnTypeCheck(llvm::LLVMContext& context) {
	return TypeTable::GetExprType(TypeAST("i32"), context);
}

ExprType FloatExpr::OnTypeCheck(llvm::LLVMContext& context) {
	return TypeTable::GetExprType(TypeAST("f64"), context);
}

std::string_view GetOpName(TokenType op) {
	switch (op) {
		case TokenType::Add: return "add";
		case TokenType::Div: return "divide";
		case TokenType::Mul: return "multiply";
		case TokenType::Sub: return "substract";
	}
}

ExprType BinOpExpr::OnTypeCheck(llvm::LLVMContext& context) {
	auto lhs = LHS->TypeCheck(context);
	auto rhs = RHS->TypeCheck(context);

	if (lhs.IsUnknown() || rhs.IsUnknown()) {
		return UnknownType;
	}

	if (lhs != rhs) {
		Logger::Error(std::format("Couldn't {0} values of type {1} and {2}", GetOpName(Op), lhs.GetDecl()->Name, rhs.GetDecl()->Name));
		return UnknownType;
	}

	return lhs;
}

ExprType VariableExpr::OnTypeCheck(llvm::LLVMContext& context) {
	Symbol = FindSymbol(Name);
	if (Symbol == nullptr) {
		Logger::Error(std::format("Tried to use undeclared variable {0}", Name));
		return UnknownType;
	}
	return TypeTable::GetExprType(Symbol->Variable.Type, context);
}

ExprType CallExpr::OnTypeCheck(llvm::LLVMContext& context) {
	for (auto& param : Params) {

	}
}

ExprType UnaryExpr::OnTypeCheck(llvm::LLVMContext& context) {
	auto type = Expr->TypeCheck(context);
	if (type != TypeTable::GetExprType(TypeAST("bool"), context)) {
		Logger::Error(std::format("Operator ! expected operand of type bool and got type {0}", type.GetDecl()->Name));
	}
	return type;
}

void FuncAST::TypeCheck(llvm::LLVMContext& context) {
	ReturnType = TypeTable::GetExprType(Proto.Type, context);

	for (auto& symbol : Proto.Params) {
		symbols.push_back(&symbol);
	}

	if (std::holds_alternative<ExprPtr>(Body)) {
		auto& expr = std::get<ExprPtr>(Body);
		auto bodyType = expr->TypeCheck(context);

		if (bodyType.IsUnknown())
			return;

		if (bodyType != ReturnType)
			Logger::Error(std::format("Expected type {0} and got type {1}", ReturnType.GetDecl()->Name, bodyType.GetDecl()->Name));
	} else {
		auto& block = std::get<std::unique_ptr<BlockStatement>>(Body);
		block->TypeCheckStatement(context);
	}

	symbols.resize(symbols.size() - Proto.Params.size());
}


void ModuleAST::TypeCheck(llvm::LLVMContext& context) {
	for (auto& decl : TypeDecls) {
		for (auto& method : decl->Methods) {
			method.TypeCheck(context);
		}
	}
}

void VariableDeclStatement::TypeCheckStatement(llvm::LLVMContext& context) {
	symbols.push_back(this);
	symbolsCount.top()++;
}

void BlockStatement::TypeCheckStatement(llvm::LLVMContext& context) {
	symbolsCount.push(0);

	for (auto& statement : Statements) {
		statement->TypeCheckStatement(context);
	}

	symbols.resize(symbols.size() - symbolsCount.top());
	symbolsCount.pop();
}

void ReturnStatement::TypeCheckStatement(llvm::LLVMContext& context) {
	auto exprType = Expr->TypeCheck(context);
	if (exprType.IsUnknown())
		return;

	if (exprType != ReturnType)
		Logger::Error(std::format("Expected type {0} and got type {1}", ReturnType.GetDecl()->Name, exprType.GetDecl()->Name));
}
