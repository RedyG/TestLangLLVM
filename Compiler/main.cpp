#include <iostream>
#include "Modules.h"
#include "BuiltInTypes.h"
#include "RedyParser.h"
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
	try {
		RedyParser parser;
		/*auto defaultTypes = parser.Parse(R"(
			pub struct f64 {}
			pub struct bool {} 
			pub struct i32 {}
		)");*/

		RedyModule module(parser.Parse(R"(
			pub struct TestStruct {
				i32 Test;

				f64 main() {
					f64 a = 2.0;
					if a == 2.0 {
						return 0.0;
					}
					return 3.0;
				}
			} 

			f64 other(f64 a) => a + 2.0;
		)"));

		//AddModule("Main", std::move(module));

		BuiltInTypes::Init(*TheContext);

		//defaultTypes.Register(*TheModule);
		module.Register(*TheModule);
		module.TypeCheck(*TheContext);
		module.CodeGen(CodeGenCtx(*TheModule, module, Builder));
		RunCode(std::move(TheModule), std::move(TheContext));
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		throw;
	}
}