#include "AST.h"
#include <llvm/IR/Verifier.h>
#include <fstream>
using namespace llvm;

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
	auto func = ctx.RedyMod.GetFunc(dynamic_cast<VariableExpr&>(*Callee).Name);
	std::vector<Value*> params;
	for (auto& param : Params) {
		params.push_back(param->CodeGen(ctx));
	}
	return ctx.Builder.CreateCall(func->LLVMFunc, params, "calltmp");
}

Value* UnaryExpr::CodeGen(CodeGenCtx ctx) {
	return Expr->CodeGen(ctx);
}

void ReturnStatement::CodeGenStatement(CodeGenCtx ctx) {
	ctx.Builder.CreateRet(Expr->CodeGen(ctx));
}

void BlockStatement::CodeGenStatement(CodeGenCtx ctx) {
	BasicBlock* basicBlock = BasicBlock::Create(ctx.GetLLVMCtx(), "entry", ctx.Func);
	ctx.Builder.SetInsertPoint(basicBlock);

	for (auto& statement : Statements) {
		statement->CodeGenStatement(ctx);
	}
}

void BlockStatement::CodeGenWithParams(CodeGenCtx ctx, std::vector<ParamAST>& params) {
	BasicBlock* basicBlock = BasicBlock::Create(ctx.GetLLVMCtx(), "entry", ctx.Func);
	ctx.Builder.SetInsertPoint(basicBlock);

	for (auto& param : params) {
		param.CodeGen(ctx);
	}

	for (auto& statement : Statements) {
		statement->CodeGenStatement(ctx);
	}
}

void VariableDeclStatement::CodeGenStatement(CodeGenCtx ctx) {
	Symbol.Alloca = ctx.Builder.CreateAlloca(ctx.RedyMod.GetType(Symbol.Variable.Type, ctx.GetLLVMCtx())->LLVMType);
	if (Symbol.Variable.DefaultValue) {
		auto value = Symbol.Variable.DefaultValue->CodeGen(ctx);
		ctx.Builder.CreateStore(value, Symbol.Alloca);
	}
}

void ParamAST::CodeGen(CodeGenCtx ctx) {
	Symbol.Alloca = ctx.Builder.CreateAlloca(LLVMArg->getType());
	ctx.Builder.CreateStore(LLVMArg, Symbol.Alloca);
}

void FuncAST::CodeGen(CodeGenCtx ctx) {
	ctx.Func = LLVMFunc;
	if (!ctx.Func)
		throw std::exception("Function not found");

	if (std::holds_alternative<ExprPtr>(Body)) {
		BasicBlock* BB = BasicBlock::Create(ctx.GetLLVMCtx(), "entry", ctx.Func);
		ctx.Builder.SetInsertPoint(BB);

		for (auto& param : Proto.Params) {
			param.CodeGen(ctx);
		}

		if (Value* RetVal = std::get<ExprPtr>(Body)->CodeGen(ctx)) {
			ctx.Builder.CreateRet(RetVal);
		}
		else {
			ctx.Func->eraseFromParent();
			throw std::exception("Invalid codegen");
		}
	}
	else {
		std::get<std::unique_ptr<BlockStatement>>(Body)->CodeGenWithParams(ctx, Proto.Params);
	}

	verifyFunction(*ctx.Func);
}

void StructAST::CodeGen(CodeGenCtx ctx) {
	for (auto& methodEntry : Methods) {
		methodEntry.second.CodeGen(ctx);
	}
}

void RedyModule::CodeGen(CodeGenCtx ctx) {
	for (auto& entry : m_typeDecls) {
		// temporary fix
		if (auto structAST = dynamic_cast<StructAST*>(entry.second.get())) {
			structAST->CodeGen(ctx);
		}
	}

	for (auto& funcEntry : m_funcs) {
		funcEntry.second.CodeGen(ctx);
	}
}