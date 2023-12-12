#include "Logger.h"

namespace Logger {
	void Error(std::string_view error) {
		std::cout << "ERROR: " << error << std::endl;
	}

	void Warning(std::string_view warning) {
		std::cout << "WARNING: " << warning << std::endl;
	}
}