#include "Logger.h"
#include "TextPos.h"
#include <algorithm>
#include <format>

namespace Logger {
	Project* RedyProject = nullptr;

	constexpr std::string_view CYAN = "\033[96m";
	constexpr std::string_view RED = "\033[91m";
	constexpr std::string_view ERROR_SUFFIX = "\033[91merror\033[0m";
	constexpr std::string_view WARNING_SUFFIX = "\033[93mwarning\033[0m";
	constexpr std::string_view BOLD = "\033[1m";
	constexpr std::string_view NORMAL = "\033[0m";
	constexpr int SKIP_THRESHOLD = 2;

	std::string ReplaceAll(std::string str, const std::string_view from, const std::string_view to) {
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
		}
		return str;
	}

	void Error(Log log) {

		std::vector<std::tuple<bool, UnderlinedText>> underlines = { {true, log.Cause} };

		auto lastLine = log.Cause.Node.End.LineY;
		for (auto hint : log.Hints) {
			lastLine = std::max(lastLine, hint.Node.End.LineY);
			underlines.push_back({ false, hint });
		}

		std::string_view fileContent = RedyProject->InputFiles.find(log.File)->second;

		auto margin = std::string(std::max(std::to_string(lastLine).length(), (size_t)2), ' ');
		
		std::cout
			<< ERROR_SUFFIX << BOLD << ": " << log.Title << std::endl // error: title
			<< margin.substr(0, margin.length() - 1) << CYAN << "--> " << NORMAL << log.File << ':' << log.Cause.Node.Start.LineY << ':' << log.Cause.Node.Start.LineX << std::endl  //  --> file:x:y
			<< margin << CYAN << '|' << std::endl;

		std::sort(underlines.begin(), underlines.end(), [](const auto& lhs, const auto& rhs) {
			return std::get<UnderlinedText>(lhs).Node.Start.LineY < std::get<UnderlinedText>(rhs).Node.Start.LineY;
		});

		for (auto [isCause, underline] : underlines) {
			if (underline.Node.Start.LineY != underline.Node.End.LineY)
				throw std::exception("UnderlinedText must be on the same line for the moment");

			auto color = isCause ? RED : CYAN;
			auto underlineChar = isCause ? '^' : '-';
			auto numChars = underline.Node.End.Pos - underline.Node.Start.Pos;

			auto line = underline.Node.Start.LineY;
			auto lineStart = underline.Node.Start.Pos - underline.Node.Start.LineX;
			auto centeredLine = std::format("{:^{}}", std::to_string(line), margin.length());

			size_t charsOnRight = 0;
			while (true) {
				auto c = fileContent[underline.Node.End.Pos + charsOnRight];
				if (c == '\n' || c == '\r')
					break;

				charsOnRight++;
			}

			auto b = underline.Node.End.Pos - lineStart + charsOnRight;
			auto lineContent = std::string(fileContent.substr(lineStart,
				b));
			auto tabsCount = std::count(lineContent.begin(), lineContent.end(), '\t');
			lineContent = ReplaceAll(lineContent, "\t", "    ");

			auto underlineMargin = std::string(underline.Node.Start.LineX + tabsCount * 3, ' ');

			std::cout << centeredLine << CYAN << "| " << NORMAL << lineContent << std::endl;
			std::cout << margin << CYAN << "| " << underlineMargin << color << std::string(numChars, underlineChar) << ' ' << underline.Text << std::endl;

		}	

		std::cout << margin << CYAN << "|" << NORMAL << std::endl;
	}

	void Error(std::string_view error) {
		std::cout << ERROR_SUFFIX << ": " << error << std::endl;
	}

	void Warning(std::string_view warning) {
		std::cout << WARNING_SUFFIX << ": " << warning << std::endl;
	}
}