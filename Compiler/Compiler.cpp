#include <iostream>
#include "RedyLexer.h"
#include "RedyParser.h"
#include "CodeGenerator.h"

LLVMContext TheContext;
IRBuilder<> Builder(TheContext);
std::unique_ptr<Module> TheModule;
std::map<std::string_view, Value*> NamedValues;

void main() {
	TheModule = std::make_unique<Module>("Test", TheContext);
	std::cout << "hello world!!\n";
	RedyParser parser;
	auto res = parser.Parse("pub int Test(double f, float e) 2.5 + 9.1 ");
	auto func = res.CodeGen();
	TheModule->dump();
} 