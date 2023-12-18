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
			pub struct i32 {}
		)");

		auto module = parser.Parse(R"(
			pub struct TestStruct {
				i32 Test;
				f64 main() {
					return 1.0 + 2.0 + 5.0;
				}
			} 
		)");

		TypeTable::AddModuleTypes(defaultTypes);
		TypeTable::AddModuleTypes(module);
		module.TypeCheck();
		module.CodeGen();
		RunCode();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		throw;
	}
}
