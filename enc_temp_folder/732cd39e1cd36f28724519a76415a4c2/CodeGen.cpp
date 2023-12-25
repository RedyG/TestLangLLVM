#include "AST.h"
#include "TypeTable.h"
#include <llvm/IR/Verifier.h>
#include <fstream>

Function* CurrentFunc;

Value* IntExpr::CodeGen(CodeGenCtx ctx) {
	return ConstantInt::get(IntegerType::getInt32Ty(ctx.GetContext()), Value, true);
}

Value* FloatExpr::CodeGen(CodeGenCtx ctx) {
	return ConstantFP::get(ctx.GetContext(), APFloat(Value));
}

Value* BinOpExpr::CodeGen(CodeGenCtx ctx) {
	auto lhs = LHS->CodeGen(ctx);
	std::cout << LHS->Type->Name;
	auto rhs = RHS->CodeGen(ctx);
	std::cout << RHS->Type->Name;
	switch (Op) {
	case TokenType::Add: return ctx.Builder.CreateFAdd(lhs, rhs, "addtmp");;
	case TokenType::Sub: return nullptr;
	case TokenType::Mul: return nullptr;
	case TokenType::Div: return nullptr;
	default: throw std::exception("invalid op");
	}

	throw std::exception("no binop yet");
}

Value* VariableExpr::CodeGen(CodeGenCtx ctx) {
	return ctx.Builder.CreateLoad(Symbol->Alloca->getType(), Symbol->Alloca);
}

Value* CallExpr::CodeGen(CodeGenCtx ctx) {
	Function* func = ctx.Module.getFunction(dynamic_cast<VariableExpr&>(*Callee).Name);
	return ctx.Builder.CreateCall(func, {}, "calltmp");
}

Value* UnaryExpr::CodeGen(CodeGenCtx ctx) {
	return Expr->CodeGen(ctx);
}

Type* TypeAST::CodeGen(LLVMContext& context) {
	if (Name == "f64") {
		return Type::getDoubleTy(context);
	}
	if (Name == "bool") {
		return Type::getInt1Ty(context);
	}
	if (Name == "i32") {
		return Type::getInt32Ty(context);
	}

	throw std::exception("invalid type");
}

void ReturnStatement::CodeGenStatement(CodeGenCtx ctx) {
	ctx.Builder.CreateRet(Expr->CodeGen(ctx));
}

void BlockStatement::CodeGenStatement(CodeGenCtx ctx) {
	BasicBlock* basicBlock = BasicBlock::Create(ctx.GetContext(), "entry", CurrentFunc);
	ctx.Builder.SetInsertPoint(basicBlock);

	for (auto& statement : Statements) {
		statement->CodeGenStatement(ctx);
	}
}

void VariableDeclStatement::CodeGenStatement(CodeGenCtx ctx) {
	auto value = Variable.DefaultValue->CodeGen(ctx);
	Alloca = ctx.Builder.CreateAlloca(value->getType());
}



void FuncAST::CodeGen(CodeGenCtx ctx) {
	CurrentFunc = ctx.Module.getFunction(Proto.Name);
	if (!CurrentFunc)
		throw std::exception("Function not found");

	if (std::holds_alternative<ExprPtr>(Body)) {
		if (Value* RetVal = std::get<ExprPtr>(Body)->CodeGen(ctx)) {

			BasicBlock* BB = BasicBlock::Create(ctx.GetContext(), "entry", CurrentFunc);
			ctx.Builder.SetInsertPoint(BB);

			ctx.Builder.CreateRet(RetVal);
		}
		else {
			CurrentFunc->eraseFromParent();
			throw std::exception("Invalid codegen");
		}
	}
	else {
		std::get<std::unique_ptr<BlockStatement>>(Body)->CodeGenStatement(ctx);
	}

	verifyFunction(*CurrentFunc);
}

void StructAST::CodeGen(CodeGenCtx ctx) {
	for (auto& method : TypeDecl.Methods) {
		method.CodeGen(ctx);
	}
}

void ModuleAST::CodeGen(CodeGenCtx ctx) {
	for (auto& structAST : Structs) {
		structAST.CodeGen(ctx);
	}
}