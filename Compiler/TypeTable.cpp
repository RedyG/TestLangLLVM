#include "TypeTable.h"
#include <format>
#include "Logger.h"
using namespace llvm;

std::unordered_map<TypeAST, ExprType> typeTable;
std::unordered_map<std::string_view, int> a;

namespace TypeTable {
	ExprType GetExprType(TypeAST type, LLVMContext& context) {
		auto exprType = typeTable.find(type);
		if (exprType == typeTable.end()) {
			Logger::Error(std::format("The type {0} doesn't exist.", type.Name));
			return UnknownType;
		}

		if (exprType->second.LLVMType == nullptr) {
			exprType->second.LLVMType = exprType->second.GetDecl()->GenLLVMType(context);
		}
		return exprType->second;
	}

	void AddExprType(TypeAST type, ExprType exprType) {
		typeTable.insert({ type, exprType });
	}
}