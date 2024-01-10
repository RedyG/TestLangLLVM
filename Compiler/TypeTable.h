#pragma once
#include <map>
#include "AST.h" 
namespace llvm {
	class LLVMContext;
}

namespace TypeTable {
	ExprType GetExprType(TypeAST type, llvm::LLVMContext& context);
	void AddExprType(TypeAST type, ExprType exprType);
}