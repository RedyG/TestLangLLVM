#include <iostream>
#include "RedyParser.h"
#include "CodeGen.h"

void main() {
	InitModule();

	RedyParser parser;
	auto res = parser.Parse("pub int Test(double f, float e) 200.5 + 9.1 ");
	auto func = res.CodeGen();

	DumpIr();
} 