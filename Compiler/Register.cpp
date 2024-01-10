#include "AST.h"
#include "TypeTable.h"
#include "Logger.h"
#include <format>
#include "TypeDeclAST.h"
#include "ExprType.h"
using namespace llvm;

void ModuleAST::Register(Module& module) {
	for (auto& decl : TypeDecls) {
		for (auto& method : decl->Methods) {
			method.Proto.Register(module);
		}
	}
}



void ProtoAST::Register(Module& module) {
	if (module.getFunction(Name)) {
		Logger::Error(std::format("Function with name \"{0}\" already exists", Name));
		return;
	}

	std::vector<llvm::Type*> types;
	types.reserve(Params.size());
	for (auto& param : Params) {
		types.push_back(TypeTable::GetExprType(param.Variable.Type, module.getContext()).LLVMType);
	}
	FunctionType* functionType =
		FunctionType::get(TypeTable::GetExprType(Type, module.getContext()).LLVMType, types, false);
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