#pragma once
#include <string>
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h"
#include "TypeDeclAST.h"

extern TypeDeclAST* UnknownDecl;

struct ExprType {
public:

	TypeDeclAST* Decl;
	llvm::Type* LLVMType;

	bool IsUnknown() {
		return LLVMType == nullptr;
	}

	ExprType(TypeDeclAST* decl, llvm::Type* llvmType) : Decl(decl), LLVMType(llvmType) {}

	bool operator == (ExprType that) {
		return Decl->Name == that.Decl->Name && LLVMType == that.LLVMType;
	}
};

const ExprType UnknownType = ExprType(UnknownDecl, nullptr);