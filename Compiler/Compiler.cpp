#include <iostream>
#include "RedyParser.h"
#include "CodeGen.h"
#include "TypeTable.h"

void main() {
	InitModule();
	try {
		RedyParser parser;
		auto defaultTypes = parser.Parse(R"(
			pub struct double {}
			pub struct bool {} 
		)");

		auto module = parser.Parse(R"(
			pub struct TestStruct {
				pub double TestFunc() 2.0;
				pub string SomeField;
				double comFunc(bool arg2, double secondd) arg2 + arg2;
			} 
		)");

		TypeTable::AddModuleTypes(defaultTypes);
		TypeTable::AddModuleTypes(module);
		module.TypeCheck();
		module.CodeGen();
		DumpIr();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		throw;
	}
}