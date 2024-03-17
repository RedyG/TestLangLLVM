#include "BuiltInTypes.h"
#include "StructAST.h"
#include "TypeDeclAST.h"
#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h"
using namespace llvm;

namespace BuiltInTypes {
	TypeDeclAST* F64Decl;
	TypeDeclAST* BoolDecl;
	TypeDeclAST* I32Decl;


	void Init(LLVMContext& context) {
		F64Decl = new StructAST({}, VisibilityNodeAST(VisibilityAST::Public, NodeAST()), "f64", {}, "std.redy", NodeAST());
		F64Decl->LLVMType = Type::getDoubleTy(context);
		BoolDecl = new StructAST({}, VisibilityNodeAST(VisibilityAST::Public, NodeAST()), "bool",{}, "std.redy", NodeAST());
		BoolDecl->LLVMType = Type::getInt1Ty(context);
		I32Decl = new StructAST({}, VisibilityNodeAST(VisibilityAST::Public, NodeAST()), "i32", {}, "std.redy", NodeAST());
		I32Decl->LLVMType = Type::getInt32Ty(context);
	}

	TypeDeclAST* Get(std::string_view name) {
		if (name == "f64")
			return F64Decl;
		if (name == "bool")
			return BoolDecl;
		if (name == "i32")
			return I32Decl;
		return nullptr;
	}
}