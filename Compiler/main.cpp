#include <iostream>
#include "RedyParser.h"
#include "TypeTable.h"
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/ExecutionEngine/Orc/ThreadSafeModule.h>
using namespace llvm;

std::unique_ptr<LLVMContext> TheContext = std::make_unique<LLVMContext>();
IRBuilder<> Builder(*TheContext);
std::unique_ptr<Module> TheModule;

void InitModule() {
	llvm::InitializeNativeTarget();
	llvm::InitializeNativeTargetAsmPrinter();
	llvm::InitializeNativeTargetAsmParser();
	TheModule = std::make_unique<Module>("Test", *TheContext);
}

StructAST* f64Decl = new StructAST({}, VisibilityAST::Public, "f64", {});
StructAST* boolDecl = new StructAST({}, VisibilityAST::Public, "bool", {});
StructAST* i32Decl = new StructAST({}, VisibilityAST::Public, "i32", {});

void InitDefaultTypes() {
	TypeTable::AddExprType(TypeAST("f64"), ExprType(f64Decl, Type::getDoubleTy(*TheContext)));
	TypeTable::AddExprType(TypeAST("bool"), ExprType(boolDecl, Type::getInt1Ty(*TheContext)));
	TypeTable::AddExprType(TypeAST("i32"), ExprType(i32Decl, Type::getInt32Ty(*TheContext)));
}

void RunCode(std::unique_ptr<Module> module, std::unique_ptr<LLVMContext> context) {

	module->dump();

	auto JITMaybe = llvm::orc::LLJITBuilder().create();
	auto& JIT = JITMaybe.get(); // DO NOT COMBINE WITH THE LINE BEFORE

	// Add the module.
	if (auto err = JIT->addIRModule(llvm::orc::ThreadSafeModule(std::move(module), std::move(context))))
		return;

	// Look up the JIT'd code entry point.
	auto& entrySym = JIT->lookup("main").get();

	// Cast the entry point address to a function pointer.
	auto* entry = entrySym.toPtr<double()>();

	// Call into JIT'd code.
	double res = entry();
	std::cout << "result: " << res;
}

void main() {
	InitModule();
	InitDefaultTypes();
	try {
		RedyParser parser;
		/*auto defaultTypes = parser.Parse(R"(
			pub struct f64 {}
			pub struct bool {} 
			pub struct i32 {}
		)");*/

		auto module = parser.Parse(R"(
			pub struct TestStruct {
				i32 Test;

				f64 main() {
					f64 a = 2.0;
					return 1.0 + a + other(a);
				}
				f64 other(f64 a) => a + 2.0;
			} 
		)");

		//defaultTypes.Register(*TheModule);
		module.Register(*TheModule);
		module.TypeCheck(*TheContext);
		module.CodeGen(CodeGenCtx(*TheModule, Builder));
		RunCode(std::move(TheModule), std::move(TheContext));
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		throw;
	}
}


/*

void other(Vec<int> a) {
	

}




*/