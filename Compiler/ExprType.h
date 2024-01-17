#pragma once
namespace llvm {
	class Type;
}
class TypeDeclAST;


struct ExprType {
public:
	llvm::Type* LLVMType;

	bool IsUnknown(); 
	TypeDeclAST* GetDecl();

	ExprType(TypeDeclAST* decl, llvm::Type* llvmType);

	bool operator == (ExprType that);
private:
	TypeDeclAST* Decl;
};
extern ExprType UnknownType;