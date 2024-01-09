#include "TypeTable.h"
#include <format>
#include "Logger.h"

std::map<TypeAST, ExprType, TypeASTComparer> typeDeclMap;

namespace TypeTable {
	ExprType GetExprType(TypeAST type) {
		auto exprType = typeDeclMap.find(type);
		if (exprType == typeDeclMap.end()) {
			Logger::Error(std::format("The type {0} doesn't exist.", type.Name));
			return UnknownType;
		}

		return exprType->second;
	}

	void AddExprType(TypeAST type, ExprType exprType) {
		typeDeclMap.insert({ type, exprType });
	}
}