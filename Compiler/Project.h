#pragma once
#include <unordered_map>
#include <string_view>
#include "RedyModule.h"

class Project {
public:
	void TypeCheck(llvm::LLVMContext& context);
	void CodeGen(llvm::Module& module, llvm::IRBuilder<>& builder);
	void Register(llvm::Module& module);

	RedyModule* GetModule(std::string_view name);
	void AddModule(RedyModule module);

private:
	std::unordered_map<std::string_view, RedyModule> m_modules;
};