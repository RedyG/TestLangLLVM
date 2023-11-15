#include <iostream>
#include "RedyLexer.h"
#include "RedyParser.h"

#pragma warning(disable:4146)
#include <llvm/IR/IRBuilder.h>

llvm::LLVMContext Context;

void main() {
	std::cout << "hello world!!";
	RedyParser parser;
	auto res = parser.Parse("2.0 * 2.0 ");
	auto b = Context.getMDKindID("hey");
}