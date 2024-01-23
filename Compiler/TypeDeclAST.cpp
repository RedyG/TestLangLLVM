#include "TypeDeclAST.h"

TypeDeclAST::TypeDeclAST(VisibilityAST visibility, std::string_view name, std::unordered_map<std::string_view, FuncAST> methods)
	: Visibility(visibility), Name(name), Methods(std::move(methods)) {}

bool TypeDeclAST::IsUnknown() {
	return LLVMType == nullptr;
}

FuncAST* TypeDeclAST::GetMethod(std::string_view name) {
	auto method = Methods.find(name);
	if (method == Methods.end()) {
		return nullptr;
	}
	return &method->second;
}