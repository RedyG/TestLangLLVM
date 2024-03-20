#include <iostream>
#include "BuiltInTypes.h"
#include "Project.h"
#include "Logger.h"
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
		Project project;
		Logger::RedyProject = &project;
		RedyModule fibModule = parser.Parse(&project, R"(
			mod Fib;

			pub f64 fib(f64 n) {
				if n == 0.0 or n == 1.0 {
					return n;
				}

				return fib(n - 1.0) + fib(n - 2.0);
			}
		)", "fib.redy");

 		RedyModule module = parser.Parse(&project, R"(
mod main;
use Fib:fib;

f64 main() {
	return fib(30.0);
}

f64 will_error() {
	return 1;
}

)", "main.redy");

		project.AddModule(std::move(fibModule));
		project.AddModule(std::move(module));

		BuiltInTypes::Init(*TheContext);

		project.Register(*TheModule);
		project.TypeCheck(*TheContext);
		project.CodeGen(*TheModule, Builder);

		RunCode(std::move(TheModule), std::move(TheContext));
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		throw;
	}
}