#include "TypeCheckCtx.h"

TypeCheckCtx::TypeCheckCtx(RedyModule& redyModule, llvm::LLVMContext& llvmContext, std::string_view file)
	: RedyMod(redyModule), LLVMCtx(llvmContext), File(file) {}