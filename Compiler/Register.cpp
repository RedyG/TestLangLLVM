#include "AST.h"
#include "Logger.h"
#include <format>
#include "TypeDeclAST.h"
#include "Project.h"
using namespace llvm;

void RedyModule::Register(Module& module) {
	RegisterImports(module.getContext());

	for (auto& typeEntry : m_typeDecls) {
		for (auto& methodEntry : typeEntry.second->Methods) {
			methodEntry.second.Register(*this, module);
		}
	}
	for (auto& funcEntry : m_funcs) {
		funcEntry.second.Register(*this, module);
	}
}

void RedyModule::RegisterImports(LLVMContext& context) {
	for (auto useDecl : m_useDecls) {
		RedyModule* module = nullptr;

		for (auto moduleName : useDecl.Path) {
			if (module == nullptr)
				module = m_project->GetModule(moduleName);
			else
				module = module->GetChild(moduleName);
		}

		if (module) {
			for (auto import : useDecl.Imports) {
				if (auto type = module->GetPubType(import, context, false))
					m_importedTypes.emplace(TypeAST(type->Name), type);
				else if (auto func = module->GetPubFunc(import, false))
					m_importedFuncs.emplace(func->Proto.Name, func);
				else if (auto child = module->GetChild(import, false))
					m_importedModules.emplace(child->Name, child);
			}
		}
	}
	// TODO: potentially set m_usesDecl to an empty vector to free memory
}


void FuncAST::Register(RedyModule& redyModule, Module& module) {
	if (module.getFunction(Proto.Name)) {
		Logger::Error(std::format("Function with name \"{0}\" already exists", Proto.Name));
		return;
	}

	std::vector<llvm::Type*> types;
	types.reserve(Proto.Params.size());
	for (auto& param : Proto.Params) {
		types.push_back(redyModule.GetType(param.Symbol.Variable.ReturnType, module.getContext())->LLVMType);
	}
	FunctionType* functionType =
		FunctionType::get(redyModule.GetType(Proto.ReturnType, module.getContext())->LLVMType, types, false);
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

void Project::Register(llvm::Module& llvmModule) {
	for (auto& module : m_modules)
		module.second.Register(llvmModule);
}