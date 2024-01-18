#include "AST.h"
#include <llvm/IR/Verifier.h>
#include <fstream>
using namespace llvm;

Function* CurrentFunc;

Value* IntExpr::CodeGen(CodeGenCtx ctx) {
	return ConstantInt::get(IntegerType::getInt32Ty(ctx.GetLLVMCtx()), Value, true);
}

Value* FloatExpr::CodeGen(CodeGenCtx ctx) {
	return ConstantFP::get(ctx.RedyMod.GetType(TypeAST("f64"), ctx.GetLLVMCtx())->LLVMType, APFloat(Value));
}

Value* BinOpExpr::CodeGen(CodeGenCtx ctx) {
	auto lhs = LHS->CodeGen(ctx);
	auto rhs = RHS->CodeGen(ctx);
	std::cout << lhs->getType() << "\n";
	std::cout << rhs->getType() << "\n";
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
	return ctx.Builder.CreateLoad(ctx.RedyMod.GetType(Symbol->Variable.Type, ctx.GetLLVMCtx())->LLVMType, Symbol->Alloca);
}

Value* CallExpr::CodeGen(CodeGenCtx ctx) {
	Function* func = ctx.Mod.getFunction(dynamic_cast<VariableExpr&>(*Callee).Name);
	std::vector<Value*> params;
	for (auto& param : Params) {
		params.push_back(param->CodeGen(ctx));
	}
	return ctx.Builder.CreateCall(func, params, "calltmp");
}

Value* UnaryExpr::CodeGen(CodeGenCtx ctx) {
	return Expr->CodeGen(ctx);
}

void ReturnStatement::CodeGenStatement(CodeGenCtx ctx) {
	ctx.Builder.CreateRet(Expr->CodeGen(ctx));
}

void BlockStatement::CodeGenStatement(CodeGenCtx ctx) {
	BasicBlock* basicBlock = BasicBlock::Create(ctx.GetLLVMCtx(), "entry", CurrentFunc);
	ctx.Builder.SetInsertPoint(basicBlock);

	for (auto& statement : Statements) {
		statement->CodeGenStatement(ctx);
	}
}

void VariableDeclStatement::CodeGenStatement(CodeGenCtx ctx) {
	Alloca = ctx.Builder.CreateAlloca(ctx.RedyMod.GetType(Variable.Type, ctx.GetLLVMCtx())->LLVMType);
	if (Variable.DefaultValue) {
		auto value = Variable.DefaultValue->CodeGen(ctx);
		ctx.Builder.CreateStore(value, Alloca);
	}
}



void FuncAST::CodeGen(CodeGenCtx ctx) {
	CurrentFunc = ctx.Mod.getFunction(Proto.Name);
	if (!CurrentFunc)
		throw std::exception("Function not found");

	if (std::holds_alternative<ExprPtr>(Body)) {
		if (Value* RetVal = std::get<ExprPtr>(Body)->CodeGen(ctx)) {

			BasicBlock* BB = BasicBlock::Create(ctx.GetLLVMCtx(), "entry", CurrentFunc);
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
	for (auto& method : Methods) {
		method.CodeGen(ctx);
	}
}

void RedyModule::CodeGen(CodeGenCtx ctx) {
	for (auto& entry : TypeDecls) {
		// temporary fix
		if (auto structAST = dynamic_cast<StructAST*>(entry.second.get())) {
			structAST->CodeGen(ctx);
		}
	}
}