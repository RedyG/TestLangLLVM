#include <iostream>
#include "RedyLexer.h"

void main() {
	std::cout << "hello world!!";
	RedyLexer lexer = CreateRedyLexer("pub class ");
	std::cout << (int)lexer.Consume().Type;
	std::cout << (int)lexer.Consume().Type;
}