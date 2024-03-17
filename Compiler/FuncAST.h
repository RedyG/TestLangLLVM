#pragma once
#include "ProtoAST.h"
#include "BlockStatement.h"
#include "CodeGenCtx.h"
#include "ExprPtr.h"
#include "TypeCheckCtx.h"

using BlockOrExpr = std::variant<ExprPtr, std::unique_ptr<BlockStatement>>;

class FuncAST {
public:
	NodeAST Node;
	llvm::Function* LLVMFunc = nullptr;
	ProtoAST Proto;
	BlockOrExpr Body;

	FuncAST(ProtoAST proto, BlockOrExpr body, NodeAST node) : Proto(std::move(proto)), Body(std::move(body)), Node(node) {}

	void TypeCheck(TypeCheckCtx ctx);
	void CodeGen(CodeGenCtx ctx);
	void Register(RedyModule& redyModule, llvm::Module& module);
};