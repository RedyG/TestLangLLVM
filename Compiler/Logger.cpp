#include "Logger.h"
#include "TextPos.h"
#include <algorithm>


namespace Logger {
	Project* RedyProject = nullptr;

	constexpr std::string_view CYAN = "\033[96m";
	constexpr std::string_view RED = "\033[91m";
	constexpr std::string_view ERROR_SUFFIX = "\033[91merror\033[0m";
	constexpr std::string_view WARNING_SUFFIX = "\033[93mwarning\033[0m";
	constexpr std::string_view BOLD = "\033[1m";
	constexpr std::string_view NORMAL = "\033[0m";

	void Error(Log log) {
		auto lastLine = log.Cause.End.LineY;
		for (auto hint : log.Hints)
			lastLine = std::max(lastLine, hint.End.LineY);

		std::string_view content = RedyProject->InputFiles.find(log.File)->second;

		auto margin = std::string(std::min(std::to_string(lastLine).length(), (size_t)2), ' ');

		std::cout
			<< ERROR_SUFFIX << BOLD << ": " << log.Title << std::endl // error: title
			<< margin.substr(0, margin.length() - 1) << CYAN << "--> " << NORMAL << log.File << ':' << log.Cause.Start.LineY << ':' << log.Cause.Start.LineX << std::endl  //  --> file:x:y
			<< margin << CYAN << '|' << std::endl;
	}

	void Error(std::string_view error) {
		std::cout << ERROR_SUFFIX << ": " << error << std::endl;
	}

	void Warning(std::string_view warning) {
		std::cout << WARNING_SUFFIX << ": " << warning << std::endl;
	}
}