#include "AST.h"
#include "TypeTable.h"
#include "Logger.h"
#include <format>

void ModuleAST::Register(Module& module) {
	for (auto& structAST : Structs) {
		structAST.Register(module);
	}
}


void StructAST::Register(Module& module) {
	if (Fields.size() > 0) {
		std::vector<Type*> types;
		for (auto& field : Fields) {
			types.push_back(field.Variable.Type.CodeGen(module.getContext()));
		}
		
		auto type = StructType::create(types, TypeDecl.Name);
	}

	TypeTable::AddTypeDecl(TypeAST(TypeDecl.Name), &TypeDecl);

	for (auto& method : TypeDecl.Methods) {
		method.Proto.Register(module);
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
		types.push_back(param.Variable.Type.CodeGen(module.getContext()));
	}
	FunctionType* functionType =
		FunctionType::get(Type.CodeGen(module.getContext()), types, false);
	Function* function =
		Function::Create(functionType, Function::ExternalLinkage, Name, module);
	// Set names for all arguments.
	unsigned i = 0;
	for (auto& Arg : function->args())
		Arg.setName(Params[i++].Variable.Name);
}