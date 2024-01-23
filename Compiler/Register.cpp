#include "AST.h"
#include "Logger.h"
#include <format>
#include "TypeDeclAST.h"
using namespace llvm;

void RedyModule::Register(Module& module) {
	for (auto& typeEntry : m_typeDecls) {
		for (auto& methodEntry : typeEntry.second->Methods) {
			methodEntry.second.Proto.Register(*this, module);
		}
	}
}



void ProtoAST::Register(RedyModule& redyModule, Module& module) {
	if (module.getFunction(Name)) {
		Logger::Error(std::format("Function with name \"{0}\" already exists", Name));
		return;
	}

	std::vector<llvm::Type*> types;
	types.reserve(Params.size());
	for (auto& param : Params) {
		types.push_back(redyModule.GetType(param.Variable.Type, module.getContext())->LLVMType);
	}
	FunctionType* functionType =
		FunctionType::get(redyModule.GetType(Type, module.getContext())->LLVMType, types, false);
	Function* function =
		Function::Create(functionType, Function::ExternalLinkage, Name, module);
	// Set names for all arguments.
	unsigned i = 0;
	for (auto& Arg : function->args())
		Arg.setName(Params[i++].Variable.Name);
}

/*pub trait Iterator<T>
* {
*	pub T Next();
* }



for i in Range(0, i => i < 10, ref i => i++)
for (int i = 0; i < 10; i++) {
}



*/