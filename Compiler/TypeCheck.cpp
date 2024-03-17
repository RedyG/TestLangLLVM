// pass to add type check and add symbols to ast nodes

#include "AST.h"
#include "Logger.h"
#include <format>
#include <stack>
#include "TypeDeclAST.h"
#include "Project.h"
#include "BuiltInTypes.h"

std::vector<SymbolAST*> symbols;
std::stack<int> symbolsCount;
TypeDeclAST* ReturnType = nullptr;

SymbolAST* FindSymbol(std::string_view name) {
	for (unsigned int i = symbols.size() - 1; i != -1; i--) {
		auto symbol = symbols[i];
		if (symbol->Variable.Name == name) {
			return symbol;
		}
	}

	return nullptr;
}

TypeDeclAST* BoolExpr::OnTypeCheck(TypeCheckCtx ctx) {
	return BuiltInTypes::BoolDecl;
}


TypeDeclAST* IntExpr::OnTypeCheck(TypeCheckCtx ctx) {
	return BuiltInTypes::I32Decl;
}

TypeDeclAST* FloatExpr::OnTypeCheck(TypeCheckCtx ctx) {
	return BuiltInTypes::F64Decl;
}

std::string_view GetOpName(TokenType op) {
	switch (op) {
		case TokenType::Add: return "add";
		case TokenType::Div: return "divide";
		case TokenType::Mul: return "multiply";
		case TokenType::Sub: return "substract";
	}
}

TypeDeclAST* BinOpExpr::OnTypeCheck(TypeCheckCtx ctx) {
	auto lhs = LHS->TypeCheck(ctx);
	auto rhs = RHS->TypeCheck(ctx);


	// temporary:
	if (Op == TokenType::Or || Op == TokenType::And)
		return BuiltInTypes::BoolDecl;

	if (lhs == nullptr || rhs == nullptr) {
		return nullptr;
	}

	if (lhs != rhs) {
		Logger::Error(std::format("Couldn't {0} values of type {1} and {2}", GetOpName(Op), lhs->Name, rhs->Name));
		return nullptr;
	}

	return lhs;
}

TypeDeclAST* VariableExpr::OnTypeCheck(TypeCheckCtx ctx) {
	Symbol = FindSymbol(Name);
	if (Symbol == nullptr) {
		Logger::Error(std::format("Tried to use undeclared variable {0}", Name));
		return nullptr;
	}
	return ctx.RedyMod.GetType(Symbol->Variable.Type, ctx.LLVMCtx);
}

TypeDeclAST* CallExpr::OnTypeCheck(TypeCheckCtx ctx) {
	Logger::Warning("The field `test` is cool");
	Logger::Error(Log("Mismatched types", ctx.File, UnderlinedText("Here", TextPos(1, 2, 2), TextPos(2, 2, 3)), {}));
	auto func = ctx.RedyMod.GetFunc(dynamic_cast<VariableExpr*>(Callee.get())->Name);
	for (auto& param : Params) {
		param->TypeCheck(ctx);
		// todo
	}
	return ctx.RedyMod.GetType(func->Proto.Type, ctx.LLVMCtx);
}

TypeDeclAST* UnaryExpr::OnTypeCheck(TypeCheckCtx ctx) {
	auto type = Expr->TypeCheck(ctx);
	if (type != BuiltInTypes::BoolDecl) {
		Logger::Error(std::format("Operator ! expected operand of type bool and got type {0}", type->Name));
	}
	return type;
}

void FuncAST::TypeCheck(TypeCheckCtx ctx) {
	ReturnType = ctx.RedyMod.GetType(Proto.Type, ctx.LLVMCtx);

	for (auto& param : Proto.Params) {
		symbols.push_back(&param.Symbol);
	}

	if (std::holds_alternative<ExprPtr>(Body)) {
		auto& expr = std::get<ExprPtr>(Body);
		auto bodyType = expr->TypeCheck(ctx);

		if (bodyType == nullptr)
			return;

		if (bodyType != ReturnType)
			Logger::Error(std::format("Expected type {0} and got type {1}", ReturnType->Name, bodyType->Name));
	} else {
		auto& block = std::get<std::unique_ptr<BlockStatement>>(Body);
		block->TypeCheckStatement(ctx);
	}

	symbols.resize(symbols.size() - Proto.Params.size());
}


void RedyModule::TypeCheck(llvm::LLVMContext& context) {
	for (auto& typeEntry : m_typeDecls) {
		for (auto& methodEntry : typeEntry.second->Methods) {
			methodEntry.second.TypeCheck(TypeCheckCtx(*this, context, methodEntry.second.Proto.File));
		}
	}

	for (auto& funcEntry : m_funcs) {
		funcEntry.second.TypeCheck(TypeCheckCtx(*this, context, funcEntry.second.Proto.File));
	}
}

void VariableDeclStatement::TypeCheckStatement(TypeCheckCtx ctx) {
	symbols.push_back(&Symbol);
	symbolsCount.top()++;
}

void IfStatement::TypeCheckStatement(TypeCheckCtx ctx) {
	auto condType = Condition->TypeCheck(ctx);
	if (condType != BuiltInTypes::BoolDecl) {
		Logger::Error("The condition of an if statement must be of type bool");
		return;
	}
	ThenBlock.TypeCheckStatement(ctx);
	if (ElseBlock) {
		ElseBlock->TypeCheckStatement(ctx);
	}
}

void BlockStatement::TypeCheckStatement(TypeCheckCtx ctx) {
	symbolsCount.push(0);

	for (auto& statement : Statements) {
		statement->TypeCheckStatement(ctx);
	}

	symbols.resize(symbols.size() - symbolsCount.top());
	symbolsCount.pop();
}

void ReturnStatement::TypeCheckStatement(TypeCheckCtx ctx) {
	auto exprType = Expr->TypeCheck(ctx);
	if (exprType == nullptr)
		return;

	if (exprType->LLVMType != ReturnType->LLVMType)
		Logger::Error(std::format("Expected type {0} and got type {1}", ReturnType->Name, exprType->Name));
}

void Project::TypeCheck(llvm::LLVMContext& context) {
	for (auto& module : m_modules)
		module.second.TypeCheck(context);
}