#include "TypeDeclAST.h"

TypeDeclAST::TypeDeclAST(VisibilityNodeAST visibility, std::string_view name, std::unordered_map<std::string_view, FuncAST> methods, std::string_view file, NodeAST node)
	: Visibility(visibility), Name(name), Methods(std::move(methods)), Node(node), File(file) {}

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