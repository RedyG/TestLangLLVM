#include <iostream>
#include "RedyLexer.h"
#include "RedyParser.h"
#include "CodeGenerator.h"

void main() {
	TheModule = new Module("Test", TheContext);
	std::cout << "hello world!!\n";
	RedyParser parser;
 	auto res = parser.Parse("pub int Test(double f, float e) 5.0 ");
	auto func = res.CodeGen();
	TheModule->dump();
} 