#pragma once
#include "ExprType.h"
#include "TypeDeclAST.h"
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h"


bool ExprType::IsUnknown() {
	return LLVMType == nullptr;
}

ExprType::ExprType(TypeDeclAST* decl, llvm::Type* llvmType) : Decl(decl), LLVMType(llvmType) {}

bool ExprType::operator == (ExprType that) {
	return Decl->Name == that.Decl->Name && LLVMType == that.LLVMType;
}

TypeDeclAST* UnknownDecl = new TypeDeclAST(VisibilityAST::Public, "Unknown", {});
ExprType UnknownType = ExprType(UnknownDecl, nullptr);