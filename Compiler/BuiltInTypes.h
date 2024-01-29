 #pragma once
#include <string>;
class TypeDeclAST;
namespace llvm {
	class LLVMContext;
}

namespace BuiltInTypes {
	extern TypeDeclAST* F64Decl;
	extern TypeDeclAST* BoolDecl;
	extern TypeDeclAST* I32Decl;
	void Init(llvm::LLVMContext& context);
	TypeDeclAST* Get(std::string_view name);
}