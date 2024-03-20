#include "TypeCheckCtx.h"
#include "FuncAST.h"

TypeCheckCtx::TypeCheckCtx(RedyModule& redyModule, llvm::LLVMContext& llvmContext, std::string_view file, FuncAST& func)
	: RedyMod(redyModule), LLVMCtx(llvmContext), File(file), Func(func) {}