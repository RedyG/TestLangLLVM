#include <iostream>
#include "RedyParser.h"
#include "CodeGen.h"
#include "TypeTable.h"

void main() {
	InitModule();
	try {
		RedyParser parser;
		auto defaultTypes = parser.Parse(R"(
			pub struct f64 {}
			pub struct bool {} 
		)");

		auto module = parser.Parse(R"(
			pub struct TestStruct {
				pub f64 TestFunc() => 2.0;
				pub f64 SomeField;
				f64 comFunc(bool arg2, f64 secondd) {
					return secondd + secondd;
				}
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