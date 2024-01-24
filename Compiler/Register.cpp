#include "AST.h"
#include "Logger.h"
#include <format>
#include "TypeDeclAST.h"
using namespace llvm;

void RedyModule::Register(Module& module) {
	for (auto& typeEntry : m_typeDecls) {
		for (auto& methodEntry : typeEntry.second->Methods) {
			methodEntry.second.Register(*this, module);
		}
	}
	for (auto& funcEntry : m_funcs) {
		funcEntry.second.Register(*this, module);
	}
}



void FuncAST::Register(RedyModule& redyModule, Module& module) {
	if (module.getFunction(Proto.Name)) {
		Logger::Error(std::format("Function with name \"{0}\" already exists", Proto.Name));
		return;
	}

	std::vector<llvm::Type*> types;
	types.reserve(Proto.Params.size());
	for (auto& param : Proto.Params) {
		types.push_back(redyModule.GetType(param.Symbol.Variable.Type, module.getContext())->LLVMType);
	}
	FunctionType* functionType =
		FunctionType::get(redyModule.GetType(Proto.Type, module.getContext())->LLVMType, types, false);
	LLVMFunc =
		Function::Create(functionType, Function::ExternalLinkage, Proto.Name, module);
	// Set names for all arguments.
	unsigned i = 0;
	for (auto& Arg : LLVMFunc->args()) {
		auto& param = Proto.Params[i++];
		Arg.setName(param.Symbol.Variable.Name);
		param.LLVMArg = &Arg;

	}
}

/*pub trait Iterator<T>
* {
*	pub T Next();
* }



for i in Range(0, i => i < 10, ref i => i++)
for (int i = 0; i < 10; i++) {
}



*/