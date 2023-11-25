#include <iostream>
#include "RedyParser.h"
#include "CodeGen.h"

void main() {
	InitModule();

	//std::cout << sizeof(std::variant<FieldAST, FuncAST, ProtoAST>) << std::endl;
	//std::cout << sizeof(FieldAST) << std::endl;
	//std::cout << sizeof(FuncAST) << std::endl;
	//std::cout << sizeof(ProtoAST) << std::endl;

	RedyParser parser;
	auto myStruct = parser.Parse(R"(
struct TestStruct {
	pub int TestFunc() 2.0;
	pub string SomeField;
	Type comFunc(int arg2, string secondd) 2.0 + 20.2();
})");
	//auto func = res.CodeGen();

	DumpIr();
} 