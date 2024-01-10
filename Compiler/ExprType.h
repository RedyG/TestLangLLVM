#pragma once
namespace llvm {
	class Type;
}
class TypeDeclAST;


struct ExprType {
public:

	TypeDeclAST* Decl;
	llvm::Type* LLVMType;

	bool IsUnknown();

	ExprType(TypeDeclAST* decl, llvm::Type* llvmType);

	bool operator == (ExprType that);
};
extern ExprType UnknownType;