// pass to add type check and add symbols to ast nodes

#include "AST.h"
#include "Logger.h"
#include <format>
#include <stack>
#include "TypeDeclAST.h"
#include "UnknownType.h"

std::vector<VariableDeclStatement*> symbols;
std::stack<int> symbolsCount;
TypeDeclAST* ReturnType = UnknownType;

VariableDeclStatement* FindSymbol(std::string_view name) {
	for (unsigned int i = symbols.size() - 1; i != -1; i--) {
		auto symbol = symbols[i];
		if (symbol->Variable.Name == name) {
			return symbol; std::move(2);
		}
	}

	return nullptr;
}

TypeDeclAST* IntExpr::OnTypeCheck(RedyModule& module, llvm::LLVMContext& context) {
	return module.GetType(TypeAST("i32"), context);
}

TypeDeclAST* FloatExpr::OnTypeCheck(RedyModule& module, llvm::LLVMContext& context) {
	return module.GetType(TypeAST("f64"), context);
}

std::string_view GetOpName(TokenType op) {
	switch (op) {
		case TokenType::Add: return "add";
		case TokenType::Div: return "divide";
		case TokenType::Mul: return "multiply";
		case TokenType::Sub: return "substract";
	}
}

TypeDeclAST* BinOpExpr::OnTypeCheck(RedyModule& module, llvm::LLVMContext& context) {
	auto lhs = LHS->TypeCheck(module, context);
	auto rhs = RHS->TypeCheck(module, context);

	if (lhs->IsUnknown() || rhs->IsUnknown()) {
		return UnknownType;
	}

	if (lhs != rhs) {
		Logger::Error(std::format("Couldn't {0} values of type {1} and {2}", GetOpName(Op), lhs->Name, rhs->Name));
		return UnknownType;
	}

	return lhs;
}

TypeDeclAST* VariableExpr::OnTypeCheck(RedyModule& module, llvm::LLVMContext& context) {
	Symbol = FindSymbol(Name);
	if (Symbol == nullptr) {
		Logger::Error(std::format("Tried to use undeclared variable {0}", Name));
		return UnknownType;
	}
	return module.GetType(Symbol->Variable.Type, context);
}

TypeDeclAST* CallExpr::OnTypeCheck(RedyModule& module, llvm::LLVMContext& context) {
	auto func = module.GetType(TypeAST("TestStruct"), context)->GetMethod(dynamic_cast<VariableExpr*>(Callee.get())->Name);
	for (auto& param : Params) {
		param->TypeCheck(module, context);
		// todo
	}
	return module.GetType(func->Proto.Type, context);
}

TypeDeclAST* UnaryExpr::OnTypeCheck(RedyModule& module, llvm::LLVMContext& context) {
	auto type = Expr->TypeCheck(module, context);
	if (type != module.GetType(TypeAST("bool"), context)) {
		Logger::Error(std::format("Operator ! expected operand of type bool and got type {0}", type->Name));
	}
	return type;
}

void FuncAST::TypeCheck(RedyModule& module, llvm::LLVMContext& context) {
	ReturnType = module.GetType(Proto.Type, context);

	for (auto& symbol : Proto.Params) {
		symbols.push_back(&symbol);
	}

	if (std::holds_alternative<ExprPtr>(Body)) {
		auto& expr = std::get<ExprPtr>(Body);
		auto bodyType = expr->TypeCheck(module, context);

		if (bodyType->IsUnknown())
			return;

		if (bodyType != ReturnType)
			Logger::Error(std::format("Expected type {0} and got type {1}", ReturnType->Name, bodyType->Name));
	} else {
		auto& block = std::get<std::unique_ptr<BlockStatement>>(Body);
		block->TypeCheckStatement(module, context);
	}

	symbols.resize(symbols.size() - Proto.Params.size());
}


void RedyModule::TypeCheck(llvm::LLVMContext& context) {
	for (auto& typeEntry : m_typeDecls) {
		for (auto& methodEntry : typeEntry.second->Methods) {
			methodEntry.second.TypeCheck(*this, context);
		}
	}
}

void VariableDeclStatement::TypeCheckStatement(RedyModule& module, llvm::LLVMContext& context) {
	symbols.push_back(this);
	symbolsCount.top()++;
}

void BlockStatement::TypeCheckStatement(RedyModule& module, llvm::LLVMContext& context) {
	symbolsCount.push(0);

	for (auto& statement : Statements) {
		statement->TypeCheckStatement(module, context);
	}

	symbols.resize(symbols.size() - symbolsCount.top());
	symbolsCount.pop();
}

void ReturnStatement::TypeCheckStatement(RedyModule& module, llvm::LLVMContext& context) {
	auto exprType = Expr->TypeCheck(module, context);
	if (exprType->IsUnknown())
		return;

	if (exprType->LLVMType != ReturnType->LLVMType)
		Logger::Error(std::format("Expected type {0} and got type {1}", ReturnType->Name, exprType->Name));
}
