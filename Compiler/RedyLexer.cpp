#pragma once
#include "RedyLexer.h"
#include <string>
#include <cwctype>

MatchResult MatchKeyword(std::string_view input, std::string_view keyword) {
	if (input.starts_with(keyword) && std::isspace(input[keyword.length()]))
		return MatchResult(true, keyword.length() + 1);

	return MatchResult();
}

MatchResult MatchChar(std::string_view input, char c) {
	if (input.at(0) == c)
		return MatchResult(true, 1);

	return MatchResult();
}

MatchResult MatchIdentifier(std::string_view input) {
	if (!std::isalpha(input[0]) && input[0] != '_')
		return MatchResult();

	int i = 1;
	while (std::iswalnum(input[i]) || input[i] == '_') { i++; }

	return MatchResult(true, i, input.substr(0, i));
}


MatchResult MatchFloatLiteral(std::string_view input) {
	// 1515_15.51_5
	bool decimal = false;
	for (int i = 0; i < input.length(); i++) {
		char c = input[i];
		if (!std::isdigit(c) && c != '_' && c != '.') {
			return MatchResult(i > 0, i, input.substr(0, i));
		}
		else if (c == '.') {
			if (decimal) {
				throw std::exception("Couldn't parse float literal");
			}

			decimal = true;
		}
	}
}

std::vector<TokenMatcher<TokenType>> tokenMatchers {
	TokenMatcher<TokenType>(TokenType::Add, [](std::string_view input) -> MatchResult { return MatchChar(input, '+'); }),
		TokenMatcher<TokenType>(TokenType::Sub, [](std::string_view input) -> MatchResult { return MatchChar(input, '-'); }),
		TokenMatcher<TokenType>(TokenType::Mul, [](std::string_view input) -> MatchResult { return MatchChar(input, '*'); }),
		TokenMatcher<TokenType>(TokenType::Div, [](std::string_view input) -> MatchResult { return MatchChar(input, '/'); }),
		TokenMatcher<TokenType>(TokenType::Equal, [](std::string_view input) -> MatchResult { return MatchChar(input, '='); }),
		TokenMatcher<TokenType>(TokenType::Dot, [](std::string_view input) -> MatchResult { return MatchChar(input, '.'); }),
		TokenMatcher<TokenType>(TokenType::Coma, [](std::string_view input) -> MatchResult { return MatchChar(input, ','); }),
		TokenMatcher<TokenType>(TokenType::LParen, [](std::string_view input) -> MatchResult { return MatchChar(input, '('); }),
		TokenMatcher<TokenType>(TokenType::RParen, [](std::string_view input) -> MatchResult { return MatchChar(input, ')'); }),
		TokenMatcher<TokenType>(TokenType::LCurly, [](std::string_view input) -> MatchResult { return MatchChar(input, '{'); }),
		TokenMatcher<TokenType>(TokenType::RCurly, [](std::string_view input) -> MatchResult { return MatchChar(input, '}'); }),
		TokenMatcher<TokenType>(TokenType::Pub, [](std::string_view input) -> MatchResult { return MatchKeyword(input, "pub"); }),
		TokenMatcher<TokenType>(TokenType::Class, [](std::string_view input) -> MatchResult { return MatchKeyword(input, "class"); }),
		TokenMatcher<TokenType>(TokenType::Identifier, MatchIdentifier),
		TokenMatcher<TokenType>(TokenType::FloatLiteral, MatchFloatLiteral),
};

RedyLexer CreateRedyLexer(std::string_view input)
{
	return RedyLexer(input, tokenMatchers);
}