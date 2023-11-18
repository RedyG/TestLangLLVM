#include <iostream>
#include "RedyLexer.h"
#include "RedyParser.h"

#pragma warning(disable:4146)
#include <llvm/IR/IRBuilder.h>

llvm::LLVMContext Context;

void main() {
	std::cout << "hello world!!\n";
	RedyParser parser;
	auto res = parser.Parse("2_0.0 * -(-90.0)()()()()() + 5.0 / -5.0 + -5.15 / (25.5215 * 2.09) -- 2.21 ");
	std::cout << "result: " << res->CodeGen();

} 