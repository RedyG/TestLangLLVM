#include "AST.h"
#include "TypeTable.h"
#include <llvm/IR/Verifier.h>
#include <fstream>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/ExecutionEngine/Orc/ThreadSafeModule.h>

std::unique_ptr<LLVMContext> TheContext = std::make_unique<LLVMContext>();
IRBuilder<> Builder(*TheContext);
std::unique_ptr<Module> TheModule;
std::map<std::string_view, Value*> NamedValues;
Function* CurrentFunc;

void InitModule() {
	llvm::InitializeNativeTarget();
	llvm::InitializeNativeTargetAsmPrinter();
	llvm::InitializeNativeTargetAsmParser();
	TheModule = std::make_unique<Module>("Test", *TheContext);
}


void RunCode() {
	TheModule->dump();

	auto JITMaybe = llvm::orc::LLJITBuilder().create();
	auto& JIT = JITMaybe.get(); // DO NOT COMBINE WITH THE LINE BEFORE

	// Add the module.
	if (auto Err = JIT->addIRModule(llvm::orc::ThreadSafeModule(std::move(TheModule), std::move(TheContext))))
		return;

	// Look up the JIT'd code entry point.
	auto& EntrySym = JIT->lookup("main").get();

	// Cast the entry point address to a function pointer.
	auto* Entry = EntrySym.toPtr<double()>();

	// Call into JIT'd code.
	double res = Entry();
	std::cout << "result: " << res;
}

void DumpIr() {
	TheModule->dump();
}

Value* IntExpr::CodeGen() {
	return ConstantInt::get(IntegerType::getInt32Ty(*TheContext), Value, true);
}

Value* FloatExpr::CodeGen() {
	return ConstantFP::get(*TheContext, APFloat(Value));
}

Value* BinOpExpr::CodeGen() {
	auto lhs = LHS->CodeGen();
	auto rhs = RHS->CodeGen();
	switch (Op) {
	case TokenType::Add: return Builder.CreateFAdd(lhs, rhs, "addtmp");;
	case TokenType::Sub: return nullptr;
	case TokenType::Mul: return nullptr;
	case TokenType::Div: return nullptr;
	default: throw std::exception("invalid op");
	}
	throw std::exception("no binop yet");
}

Value* VariableExpr::CodeGen() {
	return NamedValues[Name];
}

Value* CallExpr::CodeGen() {
	return Callee->CodeGen();
	//throw std::exception("no call yet");
}

Value* UnaryExpr::CodeGen() {
	return Expr->CodeGen();
}

Type* TypeAST::CodeGen() {
	if (Name == "f64") {
		return Type::getDoubleTy(*TheContext);
	}
	if (Name == "bool") {
		return Type::getInt1Ty(*TheContext);
	}
	if (Name == "i32") {
		return Type::getInt32Ty(*TheContext);
	}

	throw std::exception("invalid type");
}

Function* ProtoAST::CodeGen() {
	std::vector<llvm::Type*> types;
	types.reserve(Params.size());
	for (auto& param : Params) {
		types.push_back(param.Type.CodeGen());
	}
	FunctionType* functionType =
		FunctionType::get(Type.CodeGen(), types, false);
	Function* function =
		Function::Create(functionType, Function::ExternalLinkage, Name, *TheModule);
	// Set names for all arguments.
	unsigned i = 0;
	for (auto& Arg : function->args())
		Arg.setName(Params[i++].Name);
	return function;
}

void ReturnStatement::CodeGen() {
	Builder.CreateRet(Expr->CodeGen());
}

void BlockStatement::CodeGen() {
	BasicBlock* basicBlock = BasicBlock::Create(*TheContext, "entry", CurrentFunc);
	Builder.SetInsertPoint(basicBlock);

	for (auto& statement : Statements) {
		statement->CodeGen();
	}
}

Function* FuncAST::CodeGen() {
	CurrentFunc = TheModule->getFunction(Proto.Name);
	if (!CurrentFunc)
		CurrentFunc = Proto.CodeGen();
	if (!CurrentFunc)
		return nullptr;

	NamedValues.clear();
	for (auto& Arg : CurrentFunc->args())
		NamedValues[Arg.getName()] = &Arg;
	if (std::holds_alternative<ExprPtr>(Body)) {
		if (Value* RetVal = std::get<ExprPtr>(Body)->CodeGen()) {

			BasicBlock* BB = BasicBlock::Create(*TheContext, "entry", CurrentFunc);
			Builder.SetInsertPoint(BB);

			Builder.CreateRet(RetVal);
		} else {
			CurrentFunc->eraseFromParent();
			return nullptr;
		}
	} else {
		std::get<std::unique_ptr<BlockStatement>>(Body)->CodeGen();
	}

	verifyFunction(*CurrentFunc);
	return CurrentFunc;
}

void StructAST::CodeGen() {
	std::vector<Type*> types;
	for (auto& field : Fields) {
		types.push_back(field.Variable.Type.CodeGen());
	}

	auto type = StructType::create(types, TypeDecl.Name);

	for (auto& method : TypeDecl.Methods) {
		method.CodeGen();
	}
}

void ModuleAST::CodeGen() {
	for (auto& structAST : Structs) {
		structAST.CodeGen();
	}
}