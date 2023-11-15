#include <iostream>
#include "RedyLexer.h"

void main() {
	std::cout << "hello world!!";
	RedyLexer lexer = CreateRedyLexer("2.05151 0251951_519851.2 ");
	std::cout << lexer.Consume().Content;
	std::cout << lexer.Consume().Content;
}