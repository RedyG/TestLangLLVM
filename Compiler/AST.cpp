#include "AST.h"
#include <map>
#include <llvm/IR/Verifier.h>
#include "CodeGenerator.h"

Value* DoubleExpr::CodeGen() {
	std::cout << "inside double\n";

	return ConstantFP::get(TheContext, APFloat(Value));
}

Value* BinOpExpr::CodeGen() {
	std::cout << "inside BinOp " << (int)Op << '\n';

	auto lhs = LHS->CodeGen();
	auto rhs = RHS->CodeGen();

	/*switch (Op) {
		case TokenType::Add: return lhs + rhs;
		case TokenType::Sub: return lhs - rhs;
		case TokenType::Mul: return lhs * rhs;
		case TokenType::Div: return lhs / rhs;
		default: throw std::exception("invalid op");
	}*/
	throw std::exception("no binop yet");
}

Value* VariableExpr::CodeGen() {
	std::cout << "inside Variable\n";


	return NamedValues[Name];
}

Value* CallExpr::CodeGen() {
	std::cout << "inside Call\n";

	return Callee->CodeGen();

	//throw std::exception("no call yet");
}

Value* UnaryExpr::CodeGen() {
	std::cout << "inside Unary\n";

	return Expr->CodeGen();
}

Type* TypeAST::CodeGen() {
	return Type::getDoubleTy(TheContext);
}

Function* ProtoAST::CodeGen() {
	std::vector<llvm::Type*> types;
	types.reserve(Params.size());
	for (auto& param : Params) {
		types.push_back(param.Type.CodeGen());
	}
	FunctionType* functionType =
		FunctionType::get(Type::getDoubleTy(TheContext), types, false);

	// this line causes the following error when the program has finished executing:
	// A breakpoint instruction (__debugbreak() statement or a similar call) was executed in Compiler.exe
	Function* function =
		Function::Create(functionType, Function::ExternalLinkage, Name, TheModule);

	// Set names for all arguments.
	unsigned i = 0;
	for (auto& Arg : function->args())
		Arg.setName(Params[i++].Name);

	return function;
}

Function* FuncAST::CodeGen() {
	Function* TheFunction = TheModule->getFunction(Proto.Name);

	if (!TheFunction)
		TheFunction = Proto.CodeGen();

	if (!TheFunction)
		return nullptr;

	// Create a new basic block to start insertion into.
	BasicBlock* BB = BasicBlock::Create(TheContext, "entry", TheFunction);
	Builder.SetInsertPoint(BB);

	// Record the function arguments in the NamedValues map.
	NamedValues.clear();
	for (auto& Arg : TheFunction->args())
		NamedValues[Arg.getName()] = &Arg;

	if (Value* RetVal = Body->CodeGen()) {
		// Finish off the function.
		Builder.CreateRet(RetVal);

		// Validate the generated code, checking for consistency.
		verifyFunction(*TheFunction);

		return TheFunction;
	}

	// Error reading body, remove function.
	TheFunction->eraseFromParent();
	return nullptr;
}