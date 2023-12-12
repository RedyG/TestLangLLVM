#pragma once
#include <string>
#include <iostream>

namespace Logger {
	void Error(std::string_view error);
	void Warning(std::string_view warning);
}
