#pragma once
#include <unordered_map>
#include <string_view>
#include "RedyModule.h"

struct string_hash {
	using is_transparent = void;
	[[nodiscard]] size_t operator()(const char* txt) const {
		return std::hash<std::string_view>{}(txt);
	}
	[[nodiscard]] size_t operator()(std::string_view txt) const {
		return std::hash<std::string_view>{}(txt);
	}
	[[nodiscard]] size_t operator()(const std::string& txt) const {
		return std::hash<std::string>{}(txt);
	}
};

class Project {
public:
	// Maps from file name to file content

	std::unordered_map<std::string, std::string, string_hash, std::equal_to<>> InputFiles;

	void TypeCheck(llvm::LLVMContext& context);
	void CodeGen(llvm::Module& module, llvm::IRBuilder<>& builder);
	void Register(llvm::Module& module);

	RedyModule* GetModule(std::string_view name);
	void AddModule(RedyModule module);

private:
	std::unordered_map<std::string_view, RedyModule> m_modules;
};